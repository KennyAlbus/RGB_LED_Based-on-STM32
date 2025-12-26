#ifndef __PWM_DRV_H_
#define __PWM_DRV_H_

#include <stdint.h>




void Timx_CNT_Get(void);
void Rgb_Duty_Set(uint16_t r,uint16_t b,uint16_t g);
void Rgb_Pwm_Init(void);

#endif
