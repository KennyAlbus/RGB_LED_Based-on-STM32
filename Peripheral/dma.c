#include "stm32f10x.h" 
#include <stdio.h>


#define        DMA_TRANSFER_CHANNEL        DMA1_Channel6


static void (*p_Ws2812b_Cb)(void);

void Dma_IrqCb_set(void(*p_func0)(void))
{
  p_Ws2812b_Cb = p_func0;
}

/**
  * @brief  Specified Led will be turned off.
  * @param  NumberOfData:The number of data you want to transfer using DMA peripheral.
    * @arg    NumberOfData  :This parameter can be a value from 0 to 65535.
  * @retval None
  */
void Dma_Transfer_Trigger(uint16_t NumberOfData)
{
	printf("ready to transfer...\r\n");
  DMA_Cmd(DMA_TRANSFER_CHANNEL,DISABLE);
	DMA_SetCurrDataCounter(DMA_TRANSFER_CHANNEL,NumberOfData);
	DMA_Cmd(DMA_TRANSFER_CHANNEL,ENABLE);
//	while(!DMA_GetFlagStatus(DMA1_FLAG_TC6));
//	DMA_ClearFlag(DMA1_FLAG_TC6);
}

/**
  * @brief  Initialization of the DMA Perpherial on the board.
  * @param  None.
  * @retval None
  */
void Dma_Configuration(uint32_t src_addr)
{
  //dma configuration
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA_TRANSFER_CHANNEL);
	DMA_InitTypeDef dma_initstructure;
	DMA_StructInit(&dma_initstructure);
	dma_initstructure.DMA_PeripheralBaseAddr = (uint32_t)(&TIM3->CCR1);
	dma_initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma_initstructure.DMA_MemoryBaseAddr = (uint32_t )src_addr;
	dma_initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma_initstructure.DMA_DIR = DMA_DIR_PeripheralDST;
	dma_initstructure.DMA_BufferSize = 0;
	dma_initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_initstructure.DMA_Mode = DMA_Mode_Normal;
	dma_initstructure.DMA_Priority = DMA_Priority_Low;
	dma_initstructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA_TRANSFER_CHANNEL,&dma_initstructure);
	printf("DMA Init\r\n");
	
	NVIC_InitTypeDef nvic_initstructure;
	nvic_initstructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	nvic_initstructure.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_initstructure.NVIC_IRQChannelSubPriority = 1;
	nvic_initstructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_initstructure);
	
	DMA_ITConfig(DMA_TRANSFER_CHANNEL,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA_TRANSFER_CHANNEL,ENABLE);
}

void DMA1_Channel6_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC6))
	{
	  p_Ws2812b_Cb();
		DMA_ClearITPendingBit(DMA1_IT_TC6);
	}
}
