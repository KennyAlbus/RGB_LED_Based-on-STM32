#ifndef  __UART_H_
#define  __UART_H_

#include <stdint.h>


void Received_Data_Via_Serial(uint16_t *dat);
void Usart_IrqCb_Set(void(*p_func0)(void));
void uart_drv_init(uint32_t baudrate);

#endif
