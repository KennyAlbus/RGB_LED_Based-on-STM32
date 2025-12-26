#ifndef __LED_DRV_H_
#define __LED_DRV_H_


#include "stm32f10x_gpio.h"

#define         BOARD_LED_CLOCK            RCC_APB2Periph_GPIOC
#define         BOARD_LED_PORT             GPIOC
#define         BOARD_LED_PIN              GPIO_Pin_13
#define         LED_NUM                    (3)
#define         LED_GREEN                  (0)
#define         LED_BLUE                   (1)
#define         LED_RED                    (2)
#define         RGB_LED_ON                 (1)
#define         RGB_LED_OFF                (0)


typedef enum
{
  RGB_LED_GREEN = 0,
	RGB_LED_BLUE,
	RGB_LED_RED,
	RGB_LED_CYAN,
	RGB_LED_YELLOW,
	RGB_LED_PINK,
	RGB_LED_WHITE,
	RGB_LED_MAX
}led_color_t;

typedef struct
{
  uint32_t clock;
	GPIO_TypeDef *port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
}led_gpio_t;


void Rgb_Led_Turn(led_color_t color,uint8_t state);
void Led_TurnOn(uint8_t index);
void Led_TurnOff(uint8_t index);
void Bsp_Led_On(void);
void Bsp_Led_Off(void);
void Led_Drv_Init(void);

#endif
