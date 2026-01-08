#ifndef  __DMA_H_
#define  __DMA_H_

#include <stdint.h>


void Dma_Transfer_Trigger(uint16_t buffer_size);
void Dma_Drv_Init(uint32_t src_addr,uint32_t dst_addr,uint16_t buffer_size);

#endif
