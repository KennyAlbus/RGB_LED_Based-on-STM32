#include "pwm.h"
#include "led_drv.h"



#define          PMW_DUTY_MAX              (100)
#define          DUTY_STEP_VALUE           (5)



void Bsp_Led_Timer_Handler(void)
{
  static uint8_t bsp_flag;
	bsp_flag = !bsp_flag;
	if(bsp_flag)
	{
	  Bsp_Led_On();
	}
	else
	{
	  Bsp_Led_Off();
	}
}

void Duty_Cycle_Set(void)
{
  static uint8_t led_breath_direction;
	static uint8_t color_turn;
	static uint16_t duty_cycle;
	const uint16_t step = DUTY_STEP_VALUE;
	if(color_turn == 0)
	{
		if(led_breath_direction)   //from off to on
		{
			if(duty_cycle > step)
			{
				duty_cycle--;
			}
			else
			{
				duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 1;
			}
		}
		else
		{
			if(duty_cycle < PMW_DUTY_MAX - step)
			{
				duty_cycle++;
			}
			else
			{
				duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		Rgb_Duty_Set(duty_cycle,PMW_DUTY_MAX,PMW_DUTY_MAX);
	}
#if 1
	else if(color_turn == 1)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 2;
			}
		}
		Rgb_Duty_Set(PMW_DUTY_MAX,duty_cycle,PMW_DUTY_MAX);
	}
	else if(color_turn == 2)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 3;
			}
		}
		Rgb_Duty_Set(PMW_DUTY_MAX,PMW_DUTY_MAX,duty_cycle);
	}
	else if(color_turn == 3)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 4;
			}
		}
		Rgb_Duty_Set(PMW_DUTY_MAX,duty_cycle,duty_cycle);
	}
	else if(color_turn == 4)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 5;
			}
		}
		Rgb_Duty_Set(duty_cycle,PMW_DUTY_MAX,duty_cycle);
	}
	else if(color_turn == 5)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 6;
			}
		}
		Rgb_Duty_Set(duty_cycle,duty_cycle,PMW_DUTY_MAX);
	}
	else if(color_turn == 6)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 7;
			}
		}
		Rgb_Duty_Set(duty_cycle,PMW_DUTY_MAX,PMW_DUTY_MAX-duty_cycle);
	}
	else if(color_turn == 7)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 8;
			}
		}
		Rgb_Duty_Set(duty_cycle,PMW_DUTY_MAX,PMW_DUTY_MAX/2);
	}
	else if(color_turn == 8)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 9;
			}
		}
		Rgb_Duty_Set(PMW_DUTY_MAX/5,duty_cycle,PMW_DUTY_MAX/2);
	}
	else if(color_turn == 9)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 10;
			}
		}
		Rgb_Duty_Set(duty_cycle,duty_cycle,PMW_DUTY_MAX/2);
	}
	else if(color_turn == 10)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 11;
			}
		}
		Rgb_Duty_Set(PMW_DUTY_MAX/2,duty_cycle,duty_cycle);
	}
	else if(color_turn == 11)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 12;
			}
		}
		Rgb_Duty_Set(duty_cycle,PMW_DUTY_MAX/2,duty_cycle);
	}
	else if(color_turn == 12)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 13;
			}
		}
		Rgb_Duty_Set(duty_cycle,duty_cycle,duty_cycle);
	}
	else if(color_turn == 13)
	{
	  if(!led_breath_direction)
		{
		  if(duty_cycle < PMW_DUTY_MAX - step)
			{
			  duty_cycle++;
			}
			else
			{
			  duty_cycle = PMW_DUTY_MAX;
				led_breath_direction = 1;
			}
		}
		else
		{
		  if(duty_cycle > step)
			{
			  duty_cycle--;
			}
			else
			{
			  duty_cycle = 0;
				led_breath_direction = 0;
				color_turn = 0;
			}
		}
		Rgb_Duty_Set(PMW_DUTY_MAX-duty_cycle,PMW_DUTY_MAX-duty_cycle,PMW_DUTY_MAX-duty_cycle);
	}
#endif
}
