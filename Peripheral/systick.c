#include "stm32f10x.h"
#include <stdio.h>


typedef void (*p_key_scan_callback_t)(void);
static uint64_t m_sys_run_time;

static void (*p_TaskSchedule_Update)(void);
static p_key_scan_callback_t key_scan_timer_handler = NULL;

void Task_Schedule_Callback(void(*p_func)(void))
{
  p_TaskSchedule_Update = p_func;
}

void Key_Scan_CbRegister(void(*p_func0)(void))
{
  key_scan_timer_handler = p_func0;
}

/**
  * @brief  This function handles systick overflow interrupt.
            Current setting:interrupt every 1 milisecond.
  * @param  None.
  * @retval None
  */
void SysTick_Handler(void)
{
  m_sys_run_time++;
	p_TaskSchedule_Update();
	if(key_scan_timer_handler != NULL)
	{
	  key_scan_timer_handler();
	}
}

/**
  * @brief  Initialization of System core clock.
  * @param  None.
  * @retval None
  */
void Systick_Init(void)
{
  if(SysTick_Config(SystemCoreClock/1000))
	{
	  while(1);
	}
}

/**
  * @brief  Get system run time through systick.
  * @param  None.
  * @retval The current run time of system. unit:ms
  */
uint64_t Sys_RunTime_Get(void)
{
  return m_sys_run_time;
}
