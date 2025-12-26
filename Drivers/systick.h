#ifndef __SYSTICK_H_
#define __SYSTICK_H_

#include <stdint.h>


void Task_Schedule_Callback(void(*p_func)(void));
uint64_t Sys_RunTime_Get(void);
void Systick_Init(void);


#endif
