#include "stm32f10x.h" 
#include <stdio.h>




void Dma_Transfer_Trigger(uint16_t buffer_size)
{
  DMA_Cmd(DMA1_Channel4,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4,buffer_size);
	DMA_Cmd(DMA1_Channel4,ENABLE);
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
	DMA_ClearFlag(DMA1_FLAG_TC4);
}

/**
  * @brief  Initialization of the DMA Perpherial on the board.
  * @param  None.
  * @retval None
  */
void Dma_Drv_Init(uint32_t src_addr,uint32_t dst_addr,uint16_t buffer_size)
{
  //dma configuration
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Channel6);
	DMA_InitTypeDef dma_initstructure;
	DMA_StructInit(&dma_initstructure);
	dma_initstructure.DMA_PeripheralBaseAddr = (uint32_t)src_addr;
	dma_initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma_initstructure.DMA_MemoryBaseAddr = (uint32_t )dst_addr;
	dma_initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma_initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma_initstructure.DMA_BufferSize = buffer_size;
	dma_initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	dma_initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_initstructure.DMA_Mode = DMA_Mode_Normal;
	dma_initstructure.DMA_Priority = DMA_Priority_Low;
	dma_initstructure.DMA_M2M = DMA_M2M_Enable;
	
	DMA_Init(DMA1_Channel4,&dma_initstructure);
	DMA_Cmd(DMA1_Channel4,ENABLE);
	printf("DMA Init\r\n");
}
