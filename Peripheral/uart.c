#include "stm32f10x.h"                  // Device header
#include "timer.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>




#ifndef   UART_LOG_ENABLE
#define   UART_LOG_ENABLE    (1)
#endif

#if  UART_LOG_ENABLE
#define  LOG_UART_INFO  log_debug
#else
#define  LOG_UART_INFO(...)
#endif


typedef struct
{
  USART_TypeDef* uart_no;
	uint16_t tx_pin;
	uint16_t rx_pin;
}uart_HwInfo_t;


static uart_HwInfo_t m_uart_info = {USART1,GPIO_Pin_9,GPIO_Pin_10};
static void (*p_serial_irq_handler)(void);


void Usart_IrqCb_Set(void(*p_func0)(void))
{
  p_serial_irq_handler = p_func0;
}

static void uart_gpio_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitTypeDef gpio_initStruct;
	gpio_initStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initStruct.GPIO_Pin = m_uart_info.tx_pin;
	gpio_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_initStruct);
	
	gpio_initStruct.GPIO_Mode = GPIO_Mode_IPU;
	gpio_initStruct.GPIO_Pin = m_uart_info.rx_pin;
	gpio_initStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&gpio_initStruct);
}

static void uart_init(uint32_t brate)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef usart_initstruct;
	USART_StructInit(&usart_initstruct);
	usart_initstruct.USART_BaudRate = brate;
	USART_Init(m_uart_info.uart_no,&usart_initstruct);
	
	NVIC_InitTypeDef nvic_initstructure;
	nvic_initstructure.NVIC_IRQChannel = USART1_IRQn;
	nvic_initstructure.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initstructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic_initstructure);
	
	USART_ITConfig(m_uart_info.uart_no,USART_IT_RXNE,ENABLE);
	USART_Cmd(m_uart_info.uart_no,ENABLE);
}

void Received_Data_Via_Serial(uint16_t *dat)
{
	*dat = USART_ReceiveData(m_uart_info.uart_no);
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(m_uart_info.uart_no,USART_IT_RXNE) == SET)
	{
		p_serial_irq_handler();
		USART_ClearITPendingBit(m_uart_info.uart_no,USART_IT_RXNE);
	}
}

void uart_drv_init(uint32_t baudrate)
{
  uart_gpio_init();
	uart_init(baudrate);
	printf("uart init success:%d\r\n",baudrate);
	//Callback_timer_handler(Print_Info_timer_handler);
}

int fputc(int ch,FILE *f)
{
  USART_SendData(m_uart_info.uart_no,(uint16_t)ch);
	while(SET != USART_GetFlagStatus(m_uart_info.uart_no,USART_FLAG_TXE));
	return ch;
}

