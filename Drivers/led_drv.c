#include "stm32f10x.h"
#include "led_drv.h"


static led_gpio_t led_list[] = 
{
  {RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_1,GPIO_Mode_Out_PP},
	{RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_2,GPIO_Mode_Out_PP},
	{RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP},
};

/**
  * @brief  Specified Led will be turned on.
  * @param  index:This parameter can be one of the following values:
  * @arg    LED_GREEN  :select the green led.
	* @arg		LED_BLUE   :select the blue led.
	* @arg		LED_RED    :select the red led.
            which is defined on led_drv.h file.
  * @retval None
  */
void Led_TurnOn(uint8_t index)
{
  GPIO_ResetBits(led_list[index].port,led_list[index].pin);
}

/**
  * @brief  Specified Led will be turned off.
  * @param  index:This parameter can be one of the following values:
    * @arg    LED_GREEN  :select the green led.
	  * @arg		LED_BLUE   :select the blue led.
	  * @arg		LED_RED    :select the red led.
            which is defined on led_drv.h file.
  * @retval None
  */
void Led_TurnOff(uint8_t index)
{
  GPIO_SetBits(led_list[index].port,led_list[index].pin);
}

/**
  * @brief  Turn on the led color combination.
  * @param  color:See enumuration @led_color_t.
  * @param  state:This parameter can be one of the following value:
    * @arg    RGB_LED_ON  :select the green led.
	  * @arg		RGB_LED_OFF   :select the blue led.
            which is defined on led_drv.h file.
  * @retval None
  */
void Rgb_Led_Turn(led_color_t color,uint8_t state)
{
  if(!state)
	{
	  for(uint8_t i = 0;i < LED_NUM;i++)
		{
		  Led_TurnOff(i);
		}
		return;
	}
	switch(color)
	{
	  case RGB_LED_GREEN:
		{
			Led_TurnOff(LED_BLUE);
			Led_TurnOff(LED_RED);
			Led_TurnOn(LED_GREEN);
		  break;
		}
		case RGB_LED_BLUE:
		{
			Led_TurnOff(LED_GREEN);
			Led_TurnOff(LED_RED);
			Led_TurnOn(LED_BLUE);
		  break;
		}
		case RGB_LED_RED:
		{
			Led_TurnOff(LED_GREEN);
			Led_TurnOff(LED_BLUE);
			Led_TurnOn(LED_RED);
		  break;
		}
		case RGB_LED_CYAN:
		{
			Led_TurnOff(LED_RED);
			Led_TurnOn(LED_GREEN);
			Led_TurnOn(LED_BLUE);
		  break;
		}
		case RGB_LED_YELLOW:
		{
			Led_TurnOff(LED_BLUE);
			Led_TurnOn(LED_GREEN);
			Led_TurnOn(LED_RED);
		  break;
		}
		case RGB_LED_PINK:
		{
			Led_TurnOff(LED_GREEN);
			Led_TurnOn(LED_BLUE);
			Led_TurnOn(LED_RED);
		  break;
		}
		case RGB_LED_WHITE:
		{
			Led_TurnOn(LED_BLUE);
			Led_TurnOn(LED_RED);
			Led_TurnOn(LED_GREEN);
		  break;
		}
		default:
			break;
	}
}

/**
  * @brief  Turn on Led on the board with GPIOC_Pin_13.
  * @param  None.
  * @retval None
  */
void Bsp_Led_On(void)
{
  GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

/**
  * @brief  Turn off Led on the board with GPIOC_Pin_13.
  * @param  None.
  * @retval None
  */
void Bsp_Led_Off(void)
{
  GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

/**
  * @brief  Initialization of the Led on the board.
  * @param  None.
  * @retval None
  */
void Led_Drv_Init(void)
{
  RCC_APB2PeriphClockCmd(BOARD_LED_CLOCK,ENABLE);
	GPIO_InitTypeDef gpio_initstructure;
	gpio_initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_initstructure.GPIO_Pin = BOARD_LED_PIN;
	gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BOARD_LED_PORT,&gpio_initstructure);
	GPIO_SetBits(BOARD_LED_PORT,BOARD_LED_PIN);
	
//	for(uint8_t i = 0;i < LED_NUM;i++)
//	{
//	  RCC_APB2PeriphClockCmd(led_list[i].clock,ENABLE);
//		gpio_initstructure.GPIO_Mode = led_list[i].mode;
//		gpio_initstructure.GPIO_Pin = led_list[i].pin;
//		gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(led_list[i].port,&gpio_initstructure);
//		GPIO_SetBits(led_list[i].port,led_list[i].pin);
//	}
}

