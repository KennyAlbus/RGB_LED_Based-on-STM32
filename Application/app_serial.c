#include <stdio.h>
#include "uart.h"
#include "led_drv.h"
#include "app_led.h"




void TurnOnLed_ViaSerial(void)
{
  Bsp_Led_On();
	
}

void TurnOffLed_ViaSerial(void)
{
  Bsp_Led_Off();
}

void ToggleLed_ViaSerial(void)
{
  uint16_t redata = 0;
  Received_Data_Via_Serial(&redata);
  if(redata == '1')
  {
    Bsp_Led_On();
  }
  else if(redata == '0')
  {
    Bsp_Led_Off();
  }
}

void App_Serial_Init(void)
{
  Usart_IrqCb_Set(ToggleLed_ViaSerial);
}

