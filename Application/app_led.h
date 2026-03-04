#ifndef __APP_LED_H_
#define __APP_LED_H_

#include <stdint.h>


typedef void(*Ws2812b_Pattern_Func_t)(void);
typedef enum
{
  WS2812B_MODE_CLOSE = 0,
  WS2812B_MODE_WHITELIGHT,
  WS2812B_MODE_COLORFUL,
  WS2812B_MODE_MAX
}ws2812b_mode_t;

void App_Ws2812b_Mode_Switch(void);
void App_Ws2812b_Pattern7(void);
void App_Ws2812b_Pattern6(void);
void App_Ws2812b_Pattern5(void);
void App_Ws2812b_Pattern4(void);
void App_Ws2812b_Pattern3(void);
void App_Ws2812b_Pattern2(void);
void App_Ws2812b_Pattern1(void);
void App_Ws2812b_Pattern0(void);
void App_Brightness_Timer_handler(void);
void App_Ws2812b_Color_Set(uint8_t green,uint8_t red,uint8_t blue);
void App_Ws2812b_Mode0(void);
void App_Ws2812b_Close(void);
void Bsp_Led_Timer_Handler(void);
void Duty_Cycle_Set(void);

#endif
