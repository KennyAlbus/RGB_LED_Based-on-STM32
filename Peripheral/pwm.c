#include "stm32f10x.h" 
#include "led_drv.h"
#include <stdio.h>




/**
  * @brief  Configuration of timer3.
  * @param  None.
  * @retval None
  */
static void Tim3_Config(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_DeInit(TIM3);
	TIM_TimeBaseInitTypeDef timebase_initstructure;
	TIM_TimeBaseStructInit(&timebase_initstructure);
	timebase_initstructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timebase_initstructure.TIM_CounterMode = TIM_CounterMode_Up;
	timebase_initstructure.TIM_Period = 90 - 1;
	TIM_TimeBaseInit(TIM3,&timebase_initstructure);
}

/**
  * @brief  GPIO configuration of strip lights.
  * @param  None.
  * @retval None
  */
static void Led_Gpio_config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef gpio_initstructure;
	gpio_initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstructure.GPIO_Pin = GPIO_Pin_6;
	gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_initstructure);
}

/**
  * @brief  PWM initialization of ws2812b strip lights.
  * @param  None.
  * @retval None
  */
void Led_Pwm_Init(void)
{
  Led_Gpio_config();
	Tim3_Config();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	TIM_OCInitTypeDef tim_ocinitstructure;
	TIM_OCStructInit(&tim_ocinitstructure);
	tim_ocinitstructure.TIM_OCMode = TIM_OCMode_PWM1;
	tim_ocinitstructure.TIM_OCPolarity = TIM_OCPolarity_High;
	tim_ocinitstructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM3,&tim_ocinitstructure);
	//TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_DMAConfig(TIM3,TIM_DMABase_CCR1,TIM_DMABurstLength_1Transfer);
	TIM_DMACmd(TIM3,TIM_DMA_CC1,ENABLE);
	TIM_Cmd(TIM3,DISABLE);
}

/**
  * @brief  Enable or Disable PWM in Tim3.
  * @param  state:This parameter can be one of the following values:
    * @arg    0  :close timer3.
              1  :start timer3
  * @retval None
  */
void Pwm_Cmd(uint8_t state)
{
  if(state)
	{
	  TIM_Cmd(TIM3,ENABLE);
	}
	else
	{
	  TIM_Cmd(TIM3,DISABLE);
	}
}

/**
  * @brief  Specified duty cycle on PA6.
  * @param  index:This parameter can be one of the following values:
    * @arg    LED_GREEN  :select the green led.
  * @retval None
  */
void Pwm_Duty_Set(uint16_t duty)
{
  TIM_SetCompare1(TIM3,duty);
}





/**
  * @brief  Specified duty_cycle of led.
  * @param  r,g,b:This parameter can be specified by user:
	* @arg    r  :red led duty cycle,range: 0~PMW_DUTY_MAX .
	* @arg		g  :green led duty cycle,range: 0~PMW_DUTY_MAX .
	* @arg		b  :blue led duty cycle,range: 0~PMW_DUTY_MAX .
  * @retval None
  */
void Rgb_Duty_Set(uint16_t r,uint16_t b,uint16_t g)
{
  TIM_SetCompare2(TIM2,g);
	TIM_SetCompare3(TIM2,b);
	TIM_SetCompare4(TIM2,r);
}

/**
  * @brief  Initialization of the RGB circular lamp head on the board.
  * @param  None.
  * @retval None
  */
void Rgb_Pwm_Init(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef gpio_initstructure;
	gpio_initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_initstructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	
	//TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	//Green Led
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	//Blue Led
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	//Blue Led
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
