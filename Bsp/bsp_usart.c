#include "bsp_usart.h"
#include "main.h"
#include "string.h"
#include "remoter.h"

// /******** 串口接收数组定义 ********/
// uint8_t usart5_buf[USART5_BUFLEN];
//
// /******** 数据结构体定义 ********/
// imudata_t imudata;
// rc_sbus_t rc_sbus_receive;
//
//
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart5) {
// 		uart_receive_handler(&huart5);
// 	}
// }
//
// void usart_init(void)
// {
// 	uart_receive_init(&huart5);
// }
//
// /******** 串口空闲中断处理函数 ********/
// void usart5_callback_handler(uint8_t *buff)
// {
// 	sbus_frame_parse(&remoter, buff);
// }
// /**
//   * @brief      返回当前DMA通道中剩余的数据个数
//   * @param[in]  dma_stream: DMA通道
//   * @retval     DMA通道中剩余的数据个数
//   */
// uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
// {
//   return ((uint16_t)(dma_stream->NDTR));
// }
//
// /**
//   * @brief	在接收到一帧数据之后空闲一帧数据时间之后无数据
// 	*					再来则进入此回调函数,此函数会清除空闲中断标志位
//   * @param	huart: UART句柄指针
//   * @retval
//   */
// static void uart_rx_idle_callback(UART_HandleTypeDef *huart)
// {
//   if (huart == &huart5)
//   {
//     //判断数据是否为期望的长度 如不是则不进入回调函数 直接开启下一次接收
//     if ((USART5_MAX_LEN - dma_current_data_counter(huart->hdmarx->Instance)) == USART5_BUFLEN)
//     {
//       /* 进入空闲中断回调函数 */
//       usart5_callback_handler(usart5_buf);
//     }
//
//     /* 设置DMA接收数据的长度 */
//     __HAL_DMA_SET_COUNTER(huart->hdmarx, USART5_MAX_LEN);
//   }
//
// }
//
// /**
//   * @brief	当串口发生中断的时候进此函数
//   * @param	huart: UART句柄指针
//   * @retval	在stm32f4xx_it.c中添加
//   */
// void uart_receive_handler(UART_HandleTypeDef *huart)
// {
//   /* __HAL_UART_GET_FLAG	检查指定的UART空闲标志位是否触发 */
//   /* __HAL_UART_GET_IT_SOURCEG	检查指定的UART空闲中断是否触发 */
//   if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
//       __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
//   {
//     /* 清除空闲中断标志位 */
//     __HAL_UART_CLEAR_IDLEFLAG(huart);
//
//     /* 关掉DMA */
//     __HAL_DMA_DISABLE(huart->hdmarx);
//
//     /* 进入空闲中断处理函数 */
//     uart_rx_idle_callback(huart);
//
//     /* 重启DMA传输 */
//     __HAL_DMA_ENABLE(huart->hdmarx);
//   }
// }
//
// /**
//   * @brief      配置使能DMA接收(而不是中断接收)
//   * @param[in]  huart: UART句柄指针
//   * @param[in]  pData: receive buff
//   * @param[in]  Size:  buff size
//   * @retval     set success or fail
//   */
// static int uart_receive_dma_no_it(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
// {
//   uint32_t tmp = 0;
//   tmp = huart->RxState;
//
//   /* 判断串口是否已经初始化完成 */
//   if (tmp == HAL_UART_STATE_READY)
//   {
//     /* 检测用户输入的数据是否正确 */
//     if ((pData == NULL) || (Size == 0))
//       return HAL_ERROR;
//
//     huart->pRxBuffPtr = pData;
//     huart->RxXferSize = Size;
//     huart->ErrorCode = HAL_UART_ERROR_NONE;
//
//     /* 使能DMA通道 */
//     HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->RDR, (uint32_t)pData, Size);
//
//     /* 开启DMA传输 将UART CR3 寄存器中的 DMAR位 置高 */
//     SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);
//
//     return HAL_OK;
//   }
//   else
//     return HAL_BUSY;
// }
//
// /**
//   * @brief	空闲中断初始化函数
//   * @param	huart:UART句柄指针
//   * @retval	none
//   */
// void uart_receive_init(UART_HandleTypeDef *huart)
// {
//   if (huart == &huart5)
//   {
//     /* 清除空闲中断标志位 */
//     __HAL_UART_CLEAR_IDLEFLAG(&huart5);
//     /* 开启串口空闲中断 */
//     __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
//     /* 开启DMA接收 指定接收长度和数据地址 */
//     uart_receive_dma_no_it(&huart5, usart5_buf, USART5_MAX_LEN);
//   }
// }
//
//

