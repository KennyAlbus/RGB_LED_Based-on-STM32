#include "pwm.h"
#include "led_drv.h"
#include "systick.h"
#include "ws2812b_drv.h"
#include <stdlib.h>



#define          PMW_DUTY_MAX                      (100)
#define          DUTY_STEP_VALUE                   (5)
#define          WS2812B_BRIGHTNESS_MAX            (255)
#define          WS2812B_STEP_VALUE                (1)


static uint8_t breath_dir;
static uint8_t bright,color_mode;
static uint8_t breath_times[7] = {2,3,2,1,10,5,3};
static uint32_t base_color[3] = {0xff0000,0x00ff00,0x0000ff};
const uint8_t step_value = WS2812B_STEP_VALUE;


/*     WS2812B LED effect Start  */

void App_Ws2812b_Mode0(void)
{
  Ws2812b_SetBuf(0xffff00);
	Ws2812b_UpdateBuf();
}

void App_Ws2812b_Close(void)
{
  Ws2812b_ClearBuf();
	Ws2812b_UpdateBuf();
}

void App_Ws2812b_Color_Set(uint8_t green,uint8_t red,uint8_t blue)
{
  uint32_t buffer = 0;
	buffer |= ((green<<16)|(red<<8)|blue);
	Ws2812b_SetBuf(buffer);
	Ws2812b_UpdateBuf();
}

void App_Brightness_Timer_handler(void)
{
	color_mode %= 7;
	if(!breath_dir)  //0~255
	{
		if(bright < WS2812B_BRIGHTNESS_MAX-step_value)
		{
			bright += step_value;
		}
		else
		{
			bright = WS2812B_BRIGHTNESS_MAX;
			breath_dir = !breath_dir;
		}
	}
	else             //255~0
	{
		if(bright > step_value)
		{
			bright -= step_value;
		}
		else
		{
			bright = 0;
			breath_dir = !breath_dir;
			breath_times[color_mode]--;
			if(breath_times[color_mode] == 0)
			{
				color_mode++;
			}
		}
	}
}

void App_Ws2812b_Pattern0(void)
{
	if(color_mode == 0)
	{
	  App_Ws2812b_Color_Set(bright,0,0);
	}
	else if(color_mode == 1)
	{
	  App_Ws2812b_Color_Set(0,bright,0);
	}
	else if(color_mode == 2)
	{
	  App_Ws2812b_Color_Set(0,0,bright);
	}
	else if(color_mode == 3)
	{
	  App_Ws2812b_Color_Set(bright,bright,0);
	}
	else if(color_mode == 4)
	{
	 App_Ws2812b_Color_Set(bright,0,bright);
	}
	else if(color_mode == 5)
	{
	  App_Ws2812b_Color_Set(0,bright,bright);
	}
	else if(color_mode == 6)
	{
	  App_Ws2812b_Color_Set(bright,bright,bright);
	}
}

void App_Ws2812b_Pattern1(void)
{
	static uint8_t randnum,prev_color_mode,current_color_mode;
	prev_color_mode = current_color_mode;
	current_color_mode = color_mode;
	if(current_color_mode != prev_color_mode)
	{
    randnum = rand()%256;
	}
	switch (color_mode)
	{
	  case 0:
		  App_Ws2812b_Color_Set(bright,WS2812B_BRIGHTNESS_MAX-bright,0);
		  break;
	  case 1:
		  App_Ws2812b_Color_Set(bright,0,WS2812B_BRIGHTNESS_MAX-bright);
		  break;
		case 2:
		  App_Ws2812b_Color_Set(0,bright,WS2812B_BRIGHTNESS_MAX-bright);
		  break;
		case 3:
		  App_Ws2812b_Color_Set(bright,WS2812B_BRIGHTNESS_MAX-bright,randnum);
		  break;
	  case 4:
		  App_Ws2812b_Color_Set(bright,randnum,WS2812B_BRIGHTNESS_MAX-bright);
		  break;
		case 5:
		  App_Ws2812b_Color_Set(randnum,bright,WS2812B_BRIGHTNESS_MAX-bright);
		  break;
	  default:
		  App_Ws2812b_Color_Set(randnum,randnum,randnum);
		  break;
	}
}

void App_Ws2812b_Pattern2(void)
{
	static uint8_t base_color_index;
  uint32_t random_color = rand()%0x1000000; //(0,1677 7216)
	if(Sys_RunTime_Get()% 5000 == 0)
	{
		Ws2812b_SetBuf(base_color[base_color_index]);
		base_color_index++;
		base_color_index %= 3;
	}
	else
	{
    Ws2812b_SetBuf(random_color);
	}
	Ws2812b_UpdateBuf();
}

/*     WS2812B LED effect End  */

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
