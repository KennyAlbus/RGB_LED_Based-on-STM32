#ifndef __PWM_H_
#define __PWM_H_

#include <stdint.h>


enum
{
	PWM_CLOSE = 0,
  PWM_START = 1,
};

/*Timer3 pwm function*/
void Pwm_Cmd(uint8_t state);
void Pwm_Duty_Set(uint16_t duty);
void Led_Pwm_Init(void);

/*Timer2 pwm function*/
void Rgb_Duty_Set(uint16_t r,uint16_t b,uint16_t g);
void Rgb_Pwm_Init(void);

#endif
