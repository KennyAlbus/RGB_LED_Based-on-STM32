#include <stdio.h>
#include "uart.h"
#include "led_drv.h"
#include "app_led.h"




void TurnOnLed_ViaSerial(void)
{
  Bsp_Led_On();
	printf("usart_irq_test\r\n");
	
}

void TurnOffLed_ViaSerial(void)
{
  Bsp_Led_Off();
}

void App_Serial_Init(void)
{
  Usart_IrqCb_Set(TurnOnLed_ViaSerial);
}

