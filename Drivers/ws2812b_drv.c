#include "stm32f10x.h"
#include "pwm.h"
#include "dma.h"


#define          WS2812B_LED_QUANTITY          (60)
#define          TRANSFER_DATA_LENGTH          (WS2812B_LED_QUANTITY*24+1)
uint32_t g_ws2812b_buf[WS2812B_LED_QUANTITY];
uint16_t g_ws2812b_bit[TRANSFER_DATA_LENGTH];
uint8_t g_ws2812b_Tc_flag;



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
  * @brief  Transfer data format in g_ws2812b_buf[](01010101...) to 
	          g_ws2812b_bit[](30 60 60 30 60...),then start transmit 
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
			  g_ws2812b_bit[i*24+j+1] = 60;
			}
			else
			{
			  g_ws2812b_bit[i*24+j+1] = 30;   //count period equal 90,the duty of code0 is 3060,code1 is 6030.
			}
		}
	}
	Dma_Transfer_Trigger(TRANSFER_DATA_LENGTH);
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
