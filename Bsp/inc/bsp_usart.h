#ifndef BSP_USART_H__
#define BSP_USART_H__

#ifdef __cplusplus
extern "C" {
#endif

void usart_sbus_init(void);
void usart_debug_printf(const char* fmt, ...);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
