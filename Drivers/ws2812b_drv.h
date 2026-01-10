#ifndef __WS2812B_DRV_H_
#define __WS2812B_DRV_H_

#include <stdint.h>



void Ws2812b_ClearBuf(void);
void Ws2812b_SetBuf(uint32_t color);
void Ws2812b_UpdateBuf(void);
void Ws2812b_Init(void);


#endif
