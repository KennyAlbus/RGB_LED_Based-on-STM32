#ifndef __APP_LED_H_
#define __APP_LED_H_

#include <stdint.h>


void App_Ws2812b_Pattern1(void);
void App_Brightness_Timer_handler(void);
void App_Ws2812b_Color_Set(uint8_t green,uint8_t red,uint8_t blue);
void App_Ws2812b_Pattern0(void);
void App_Ws2812b_Mode0(void);
void App_Ws2812b_Close(void);
void Bsp_Led_Timer_Handler(void);
void Duty_Cycle_Set(void);

#endif
