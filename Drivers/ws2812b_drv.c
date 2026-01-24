#include "stm32f10x.h"
#include "pwm.h"
#include "dma.h"
#include <stdio.h>
#include "stdlib.h"



#define          WS2812B_LED_QUANTITY          (60)
#define          TRANSFER_DATA_LENGTH          (WS2812B_LED_QUANTITY*24+1)
uint32_t g_ws2812b_buf[WS2812B_LED_QUANTITY];
uint16_t g_ws2812b_bit[TRANSFER_DATA_LENGTH];
uint8_t g_ws2812b_Tc_flag;
static uint32_t meteor_color[7] = 
{0xff0000,0x00ff00,0x0000ff,0xffff00,0xff00ff,0x00ffff,0xffffff};



/**
  * @brief  called by DMA1_Channel6 Interrupt after data transfer complete one time.
  * @param  None.
  * @retval None
  */
void Ws2812b_Irq_Handler(void)
{
  Pwm_Duty_Set(0);
	Pwm_Cmd(PWM_CLOSE);
	g_ws2812b_Tc_flag = 1;
}
	
/**
  * @brief  Turn off all ws2812b led.
  * @param  None.
  * @retval None
  */
void Ws2812b_ClearBuf(void)
{
  for(uint8_t i = 0;i < WS2812B_LED_QUANTITY;i++)
	{
	  g_ws2812b_buf[i] = 0x000000;
	}
}

/**
  * @brief  Set GRB led color.
  * @param  color:This parameter can be like FF00FF,FFFF00...
  * @retval None
  */
void Ws2812b_SetBuf(uint32_t color)
{
  for(uint8_t i = 0;i < WS2812B_LED_QUANTITY;i++)
	{
	  g_ws2812b_buf[i] = color;
	}
}

/**
  * @brief  Put Random value in every buffer.
  * @retval None
  */
void Ws2812b_SetRandomBuf(void)
{
  for(uint8_t i = 0;i < WS2812B_LED_QUANTITY;i++)
	{
	  g_ws2812b_buf[i] = rand()%0x1000000;
	}
}

void Ws2812b_ForwardWave(void)
{
	static uint8_t wave_round;
	wave_round %= 10;
	for(uint8_t i = WS2812B_LED_QUANTITY-1;i > 0;i--)
	{
		g_ws2812b_buf[i] = g_ws2812b_buf[i-1];
	}
	if(wave_round == 0)
	{
		g_ws2812b_buf[0] = rand()%0x1000000;
	}
	else
		g_ws2812b_buf[0] = g_ws2812b_buf[1];
	wave_round++;
}

void Ws2812b_BackwardWave(void)
{
	static uint8_t wave_cnt = 5;
	static uint8_t wave,counter;
	counter++;
	if(counter >= 4*WS2812B_LED_QUANTITY)
	{
		counter = 0;
		wave_cnt++;
		wave = 0;
		if(wave_cnt%WS2812B_LED_QUANTITY == 0)
		  wave_cnt = 5;
	}
	wave %= wave_cnt;
	for(uint8_t i = 0;i < WS2812B_LED_QUANTITY-1;i++)
	{
		g_ws2812b_buf[i] = g_ws2812b_buf[i+1];
	}
	if(wave == 0)
	{
		g_ws2812b_buf[WS2812B_LED_QUANTITY-1] = rand()%0x1000000;
	}
	wave++;
}

void Ws2812b_Snake(void)
{
	static uint8_t len;
	len %= WS2812B_LED_QUANTITY;
	for(uint8_t i = WS2812B_LED_QUANTITY-1;i > 0;i--)
	{
		g_ws2812b_buf[i] = g_ws2812b_buf[i-1];
	}
	if(len == 0)
	{
		g_ws2812b_buf[0] = rand()%0x1000000;
	}
	else if(len >= 10)
	{
		g_ws2812b_buf[0] = 0x00000000;
	}
	len++;
}

void Ws2812b_MeteorTrail(void)
{
	static uint8_t meteor,color_ctrl;
	uint32_t random_color = 0;
	uint8_t g,r,b;
	meteor %= 50;
  for(uint8_t i = WS2812B_LED_QUANTITY-1;i > 0;i--)
	{
		g_ws2812b_buf[i] = g_ws2812b_buf[i-1];
	}
	if(meteor == 0)
	{
		if(!color_ctrl)
		{
      random_color = rand()%7;
			g_ws2812b_buf[0] = meteor_color[random_color];
			color_ctrl = !color_ctrl;
		}
		else
		{
      g_ws2812b_buf[0] = rand()%0x1000000;
			color_ctrl = !color_ctrl;
		}
		
	}
	else if(meteor <= 15)
	{
		g = (g_ws2812b_buf[1] >> 16)&0xff;  //g_ws2812b_buf[1]/65536%256  g_ws2812b_buf[1]/0x10000%0x100
		r = (g_ws2812b_buf[1] >> 8)&0xff;   //g_ws2812b_buf[1]/256%256  g_ws2812_buf[1]/0x100%0x100
		b = g_ws2812b_buf[1]&0xff;          //g_ws2812b_buf[1]%256   g_ws2812b_buf[1]%0x100
		if(g>50)g-=50;
		if(r>50)r-=50;
		if(b>50)b-=50;
		g_ws2812b_buf[0] = (g<<16)|(r<<8)|b;
	}
	else
	{
		g_ws2812b_buf[0] = 0x00000000;
	}
	meteor++;
}

/**
  * @brief  Set specified LED on Ws2812B to specified color.
	* @param  index:This parameter can be 0~WS2812B_LED_QUANTITY;
  * @param  color:This parameter can be like 0xFF00FF,0xFFFF00...
  * @retval None
  */
void Ws2812b_SetLed(uint8_t index,uint32_t color)
{
	if(index >= WS2812B_LED_QUANTITY)
	  return;
	for(uint8_t i = 0;i < WS2812B_LED_QUANTITY;i++)
	{
		if(index == i)
		{
			g_ws2812b_buf[i] = color;
		}
		else
		{
			g_ws2812b_buf[i] = 0x00000000;
		}
	}
}


/**
  * @brief  Transfer data format from g_ws2812b_buf[](01010101...) to 
	          g_ws2812b_bit[](30 60 60 30 60...),then start transmitting 
						through DMA peripheral.
  * @param  None.
  * @retval None
  */
void Ws2812b_UpdateBuf(void)
{
  uint8_t i,j;
	for(i = 0;i < WS2812B_LED_QUANTITY;i++)
	{
	  for(j = 0;j < 24;j++)
		{
		  if(g_ws2812b_buf[i]&(0x800000>>j))
			{
			  g_ws2812b_bit[i*24+j] = 60;
			}
			else
			{
			  g_ws2812b_bit[i*24+j] = 30;
			}
		}
	}
	Dma_Transfer_Trigger(WS2812B_LED_QUANTITY*24+1);
	Pwm_Cmd(PWM_START);
	while(!g_ws2812b_Tc_flag);
	g_ws2812b_Tc_flag = 0;
}

/**
  * @brief  Initialization of WS2812B Driver.
  * @param  None.
  * @retval None
  */
void Ws2812b_Init(void)
{
	Dma_IrqCb_set(Ws2812b_Irq_Handler);
  Led_Pwm_Init();
	Dma_Configuration((uint32_t)(&g_ws2812b_bit));
}
