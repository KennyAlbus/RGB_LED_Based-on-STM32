#ifndef  __DMA_H_
#define  __DMA_H_

#include <stdint.h>


void Dma_IrqCb_set(void(*p_func0)(void));
void Dma_Transfer_Trigger(uint16_t NumberOfData);
void Dma_Configuration(uint32_t src_addr);

#endif
