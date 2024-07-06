#include <stdarg.h>
#include "bsp_usart.h"
#include "printf.h"
#include "remoter.h"
#include "compiler.h"

#define huart_debug huart7
#define huart_sbus  huart5

#define SBUS_HEAD 0X0F
#define SBUS_END  0X00

#define USART_DEBUG_TX_BUF_LEN 128
#define USART_SBUS_BUFLEN      25
#define USART_SBUS_MAX_LEN     USART_SBUS_BUFLEN * 2

typedef struct
{
    int16_t ch[10];
} rc_sbus_t;

extern UART_HandleTypeDef huart_debug;
UART_HandleTypeDef huart_sbus;
DMA_HandleTypeDef hdma_uart5_rx;
uint8_t usart_sbus_buf[USART_SBUS_BUFLEN] __dma_data;
rc_sbus_t rc_sbus_receive;

static void uart_receive_handler(UART_HandleTypeDef *huart);
static void uart_rx_idle_callback(UART_HandleTypeDef *huart);
static int uart_receive_dma_no_it(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size);

/**************** usart debug ****************/

void usart_debug_printf(const char* fmt, ...) {
    static char tx_buf[USART_DEBUG_TX_BUF_LEN] __dma_data = { 0 };// 必须使用static 
    char* ptx_buf = &tx_buf[0];
    static va_list args;
    static uint16_t len;
    va_start(args, fmt);

    if (HAL_UART_GetState(&huart_debug) != HAL_UART_STATE_READY) {
        return;
    }
        
    len = vsnprintf(ptx_buf, USART_DEBUG_TX_BUF_LEN, fmt, args);
    va_end(args);
    HAL_UART_Transmit_DMA(&huart_debug, (uint8_t*)tx_buf, len);
    __HAL_DMA_DISABLE_IT(huart_debug.hdmatx, DMA_IT_HT);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart == &huart_debug) {
        // 要改变 UART 的标志位为 READY (这可能是 HAL 库的 BUG)
        huart->gState = HAL_UART_STATE_READY;
    }
}

/**************** usart sbus ****************/

void usart_sbus_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    huart_sbus.Instance = UART5;
    huart_sbus.Init.BaudRate = 100000;
    huart_sbus.Init.WordLength = UART_WORDLENGTH_9B;
    huart_sbus.Init.StopBits = UART_STOPBITS_2;
    huart_sbus.Init.Parity = UART_PARITY_EVEN;
    huart_sbus.Init.Mode = UART_MODE_TX_RX;
    huart_sbus.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart_sbus.Init.OverSampling = UART_OVERSAMPLING_16;
    huart_sbus.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart_sbus.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart_sbus.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart_sbus) != HAL_OK)
    {
      Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart_sbus, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
      Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart_sbus, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
      Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart_sbus) != HAL_OK)
    {
      Error_Handler();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART5;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* UART5 clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**UART5 GPIO Configuration
    // PB13     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    // GPIO_InitStruct.Pin = GPIO_PIN_13;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // GPIO_InitStruct.Alternate = GPIO_AF14_UART5;
    // HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* UART5 DMA Init */
    /* UART5_RX Init */
    hdma_uart5_rx.Instance = DMA1_Stream7;
    hdma_uart5_rx.Init.Request = DMA_REQUEST_UART5_RX;
    hdma_uart5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart5_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart5_rx.Init.Mode = DMA_CIRCULAR;
    hdma_uart5_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_uart5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_uart5_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(&huart_sbus,hdmarx,hdma_uart5_rx);

    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

    /* UART5 interrupt Init */
    HAL_NVIC_SetPriority(UART5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
    
    /* 清除空闲中断标志位 */
    __HAL_UART_CLEAR_IDLEFLAG(&huart_sbus);
    /* 开启串口空闲中断 */
    __HAL_UART_ENABLE_IT(&huart_sbus, UART_IT_IDLE);
    /* 开启DMA接收 指定接收长度和数据地址 */
    uart_receive_dma_no_it(&huart_sbus, usart_sbus_buf, USART_SBUS_MAX_LEN);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart_sbus) {
		uart_receive_handler(&huart_sbus);
	}
}

/**
  * @brief	当串口发生中断的时候进此函数
  * @param	huart: UART句柄指针
  * @retval	在stm32f4xx_it.c中添加
  */
static void uart_receive_handler(UART_HandleTypeDef *huart)
{
  /* __HAL_UART_GET_FLAG	检查指定的UART空闲标志位是否触发 */
  /* __HAL_UART_GET_IT_SOURCEG	检查指定的UART空闲中断是否触发 */
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
      __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
  {
    /* 清除空闲中断标志位 */
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    /* 关掉DMA */
    __HAL_DMA_DISABLE(huart->hdmarx);

    /* 进入空闲中断处理函数 */
    uart_rx_idle_callback(huart);

    /* 重启DMA传输 */
    __HAL_DMA_ENABLE(huart->hdmarx);
  }
}

/**
  * @brief	在接收到一帧数据之后空闲一帧数据时间之后无数据
	*					再来则进入此回调函数,此函数会清除空闲中断标志位
  * @param	huart: UART句柄指针
  * @retval
  */
static void uart_rx_idle_callback(UART_HandleTypeDef *huart)
{
  if (huart == &huart_sbus)
  {
    //判断数据是否为期望的长度 如不是则不进入回调函数 直接开启下一次接收
    if ((USART_SBUS_MAX_LEN - ((DMA_Stream_TypeDef *)huart->hdmarx->Instance)->NDTR) == USART_SBUS_BUFLEN)
    {
	  sbus_frame_parse(&remoter, usart_sbus_buf);
    }

    /* 设置DMA接收数据的长度 */
    __HAL_DMA_SET_COUNTER(huart->hdmarx, USART_SBUS_MAX_LEN);
  }

}

/**
  * @brief      配置使能DMA接收(而不是中断接收)
  * @param[in]  huart: UART句柄指针
  * @param[in]  pData: receive buff
  * @param[in]  Size:  buff size
  * @retval     set success or fail
  */
static int uart_receive_dma_no_it(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
{
  uint32_t tmp = 0;
  tmp = huart->RxState;

  /* 判断串口是否已经初始化完成 */
  if (tmp == HAL_UART_STATE_READY)
  {
    /* 检测用户输入的数据是否正确 */
    if ((pData == NULL) || (Size == 0))
      return HAL_ERROR;

    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;
    huart->ErrorCode = HAL_UART_ERROR_NONE;

    /* 使能DMA通道 */
    HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->RDR, (uint32_t)pData, Size);

    /* 开启DMA传输 将UART CR3 寄存器中的 DMAR位 置高 */
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    return HAL_OK;
  }
  else
    return HAL_BUSY;
}

