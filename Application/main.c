#include <stdio.h>
#include "stm32f10x.h"   
#include "systick.h"
#include "pwm.h"
#include "dma.h"
#include "timer_drv.h"
#include "uart_drv.h"
#include "led_drv.h"
#include "app_led.h"


#define     UART_BAUDRATE             (115200)
#define     TASK_NUM_MAX              (2)
#define     BUFFER_SIZE_MAX           (256)

/*****   Global Varible    *****/
//uint16_t g_array0[BUFFER_SIZE_MAX];
//uint16_t g_array1[BUFFER_SIZE_MAX];


typedef struct
{
  uint16_t period;   //run task per (period) ms/s,if 0,run all the time.
	uint16_t reload;   //reload time_value of running task.
	uint8_t run;       //0:stop      1:run    /*flag of running task*/
	void (*p_func)(void);
}task_info_t;

static task_info_t task_list[TASK_NUM_MAX] = 
{
  {500,500,0,Bsp_Led_Timer_Handler},
	{50,50,0,Duty_Cycle_Set},
	//add your task here.
};


/**
  * @brief  Callback by systick interrupt per 1ms,
            it is used to update flag of run.
  * @param  None.
  * @retval None
  */
static void Task_Flag_Update(void)
{
  for(uint8_t i = 0;i < TASK_NUM_MAX;i++)
	{
		if(task_list[i].period > 0)
			task_list[i].period--;
		if(task_list[i].period == 0)
		{
		  task_list[i].period = task_list[i].reload;
			task_list[i].run = 1;
		}
	}
}

/**
  * @brief  Function of task_handler,run task when time's up.
  * @param  None.
  * @retval None
  */
static void Task_Schedule_Handler(void)
{
  for(uint8_t i = 0;i < TASK_NUM_MAX;i++)
	{
		if(task_list[i].period == 0)
		{
		  task_list[i].p_func();
			continue;
		}
	  if(task_list[i].run)
		{
		  task_list[i].run = 0;
			task_list[i].p_func();
		}
	}
}

/**
  * @brief  Initialization of some value,callback function etc..
  * @param  None.
  * @retval None
  */
static void Init_Before_Driver(void)
{
  Task_Schedule_Callback(Task_Flag_Update);
}

/**
  * @brief  Initialization of all drivers,include peripheral on MCU and external device.
  * @param  None.
  * @retval None
  */
static void Driver_Init(void)
{
	uart_drv_init(UART_BAUDRATE);
	Systick_Init();
	Timer_Drv_Init();
  Led_Drv_Init();
	Rgb_Pwm_Init();
	Led_Pwm_Init();
	//Dma_Drv_Init((uint32_t )g_array0,(uint32_t )g_array1,BUFFER_SIZE_MAX);
	
	printf("%s\n",__func__);
}

//static void App_Init(void)
//{
//  
//}

int main(void)
{
	Init_Before_Driver();
  Driver_Init();
	//App_Init();
	while(1)
	{
	  Task_Schedule_Handler();
	}
}
