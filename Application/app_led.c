#include "app_led.h"
#include "pwm.h"
#include "led_drv.h"
#include "key_drv.h"
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
static const uint32_t base_color[3] = {0xff0000,0x00ff00,0x0000ff};
static uint32_t pattern3_color[7] = 
{0xff0000,0x00ff00,0x0000ff,0xffff00,0xff00ff,0x00ffff,0xffffff};
const uint8_t step_value = WS2812B_STEP_VALUE;
static uint8_t pattern_flag;
static Ws2812b_Pattern_Func_t Ws2812b_Pattern_Cb_Array[]=
{
  App_Ws2812b_Pattern0,
	App_Ws2812b_Pattern1,
	App_Ws2812b_Pattern2,
	App_Ws2812b_Pattern3,
	App_Ws2812b_Pattern4,
	App_Ws2812b_Pattern5,
	App_Ws2812b_Pattern6,
	App_Ws2812b_Pattern7
};
static ws2812b_mode_t ws2812b_mode = WS2812B_MODE_CLOSE;

#define       WS2812B_PATTERN_COUNT     (sizeof(Ws2812b_Pattern_Cb_Array)/sizeof(Ws2812b_Pattern_Cb_Array[0]))



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

#define BASE_COLOR_DISPLAY_DURATION    (300)
#define RANDOM_COLOR_DISPLAY_DURATION  (200)
#define BASE_COLOR_SWITCH_PERIOD       (2000)

/**
  * @brief  Ws2812b_Pattern2，switch base color per 2000ms,random color is too,
	*         in the period of 2000ms,@BASE_COLOR_DISPLAY_DURATION ms display 
	          base color,and (2000-@BASE_COLOR_DISPLAY_DURATION) ms display 
						random color.
  * @param  None.
  * @retval None
  */
 #if 1
void App_Ws2812b_Pattern2(void)
{
	static uint8_t base_color_index = 0;
  static uint64_t last_switch_time = 0;
	static uint32_t random_color = 0;
	uint64_t current_time = Sys_RunTime_Get();
	uint64_t elapsed_time = current_time-last_switch_time;
  if(elapsed_time >= BASE_COLOR_SWITCH_PERIOD)
	{
		uint8_t r = rand()%0xE0 + 0x20;
		uint8_t g = rand()%0xE0 + 0x20;
		uint8_t b = rand()%0xE0 + 0x20;
		random_color = (g<<16)|(r<<8)|b;
		base_color_index = (base_color_index+1)%3;
		last_switch_time = current_time;
	}

	if(elapsed_time < BASE_COLOR_DISPLAY_DURATION)
	{
		Ws2812b_SetBuf(base_color[base_color_index]);
	}
	else
	{
		Ws2812b_SetBuf(random_color);
	}
	Ws2812b_UpdateBuf();
}
#else
void App_Ws2812b_Pattern2(void)
{
	static uint8_t base_color_index = 0;
	static uint64_t last_switch_time = 0;
	static uint64_t last_random_switch_time = 0;
	static uint32_t current_random_color = 0xFFFFFF;
	uint64_t current_time = Sys_RunTime_Get();
	uint64_t elapsed_switch = current_time-last_switch_time;
	uint64_t elapsed_random = current_time-last_random_switch_time;
  if (elapsed_switch >= BASE_COLOR_SWITCH_PERIOD)
	{
		Ws2812b_SetBuf(base_color[base_color_index]);
		Ws2812b_UpdateBuf();
		last_switch_time = current_time;
		base_color_index = (base_color_index+1)%3;
		return;
	}
  if(elapsed_random >= RANDOM_COLOR_DISPLAY_DURATION)
	{
		uint8_t r = rand()%0xE0 + 0x20;
		uint8_t g = rand()%0xE0 + 0x20;
		uint8_t b = rand()%0xE0 + 0x20;
		current_random_color = (g<<16)|(r<<8)|b;
		last_random_switch_time = current_time;
	}
	Ws2812b_SetBuf(current_random_color);
	Ws2812b_UpdateBuf();
}
#endif

/**
  * @brief  Ws2812b_Pattern3，waterflow led of random color,which switch color
	*         per 100ms.
  * @param  None.
  * @retval None
  */
void App_Ws2812b_Pattern3(void)
{
	static uint8_t led_cnt,color_index;
	static uint64_t last_setled_time = 0;
	uint64_t current_time = Sys_RunTime_Get();
	uint64_t between_twoled_time = current_time-last_setled_time;
	if(between_twoled_time >= 100)
	{
    uint32_t randnum3 = rand()%0x1000000;
		if(color_index < 7)
		{
			Ws2812b_SetLed(led_cnt,pattern3_color[color_index]);
		}
		else
		{
			Ws2812b_SetLed(led_cnt,randnum3);
		}
		Ws2812b_UpdateBuf();
		led_cnt++;
		if(led_cnt % 60 == 0)
		{
			led_cnt = 0;
			color_index++;
			if(color_index >= 20)
			  color_index = 0;
		}
		last_setled_time = current_time;
	}
}

#define    PATTERN4_RUN_INTERVAL_TIME        (500)
/**
  * @brief  Ws2812b_Pattern4，random color in every led of ws2812b,run 
	*         per @PATTERN4_RUN_INTERVAL_TIME ms.
  * @param  None.
  * @retval None
  */
void App_Ws2812b_Pattern4(void)
{
	static uint64_t last_randombuf_time = 0;
	uint64_t current_time = Sys_RunTime_Get();
	uint64_t interval_time = current_time-last_randombuf_time;
	if(interval_time >= PATTERN4_RUN_INTERVAL_TIME)
	{
    Ws2812b_SetRandomBuf();
	  Ws2812b_UpdateBuf();
		last_randombuf_time = current_time;
	}
}

#define    PATTERN5_RUN_INTERVAL_TIME        (50)
/**
  * @brief  Ws2812b_Pattern5，wave effect backward,run 
	*         per @PATTERN5_RUN_INTERVAL_TIME ms.
  * @param  None.
  * @retval None
  */
void App_Ws2812b_Pattern5(void)
{
	static uint64_t last_record_time = 0;
	uint64_t current_time = Sys_RunTime_Get();
	uint64_t interval_time = current_time-last_record_time;
	if(interval_time >= PATTERN5_RUN_INTERVAL_TIME)
	{
		Ws2812b_BackwardWave();
		Ws2812b_UpdateBuf();
		last_record_time = current_time;
	}
}

#define    PATTERN6_RUN_INTERVAL_TIME        (50)
/**
  * @brief  Ws2812b_Pattern6，snake effect,run 
	*         per @PATTERN6_RUN_INTERVAL_TIME ms.
  * @param  None.
  * @retval None
  */
void App_Ws2812b_Pattern6(void)
{
	static uint64_t last_pattern6_time = 0;
	uint64_t current_time = Sys_RunTime_Get();
	uint64_t interval_time = current_time-last_pattern6_time;
	if(interval_time >= PATTERN6_RUN_INTERVAL_TIME)
	{
    Ws2812b_Snake();
	  Ws2812b_UpdateBuf();
		last_pattern6_time = current_time;
	}
}

#define    PATTERN7_RUN_INTERVAL_TIME        (20)
/**
  * @brief  Meteor trail effect
  * @param  None.
  * @retval None
  */
void App_Ws2812b_Pattern7(void)
{
	static uint64_t last_pattern7_time = 0;
	uint64_t current_time = Sys_RunTime_Get();
	uint64_t interval_time = current_time-last_pattern7_time;
	if(interval_time >= PATTERN7_RUN_INTERVAL_TIME)
	{
		Ws2812b_MeteorTrail();
		Ws2812b_UpdateBuf();
		last_pattern7_time = current_time;
	}
}

/*     WS2812B LED effect End  */
static void App_Ws2812b_WhiteLight(void)
{
	static uint8_t lightness = 0;
	uint8_t g,r,b;
	g = ((pattern3_color[6]>>16)&0xff);
	r = ((pattern3_color[6]>>8)&0xff);
	b = (pattern3_color[6]&0xff);
	if(Key_Event_Check(KEY1,KEY_SINGLE_CLICK))
	{
		lightness++;
		if(lightness >= 4)
		  lightness = 0;
	}
	if(0 == lightness)
	{
		g /= 8;
		r /= 8;
		b /= 8;
	}
	else if(1 == lightness)
	{
		g /= 4;
		r /= 4;
		b /= 4;
	}
	else if(2 == lightness)
	{
		g /= 2;
		r /= 2;
		b /= 2;
	}
	App_Ws2812b_Color_Set(g,r,b);
}
static void App_Ws2812b_Pattern_Switch(void)
{
  if(Key_Event_Check(KEY1,KEY_SINGLE_CLICK))
	{
		App_Ws2812b_Close();
		pattern_flag++;
		if(pattern_flag >= WS2812B_PATTERN_COUNT)
		  pattern_flag = 0;
	}
	
	if(pattern_flag < WS2812B_PATTERN_COUNT)
	{
		Ws2812b_Pattern_Cb_Array[pattern_flag]();
	}
	else
	{
		pattern_flag = 0;
		App_Ws2812b_Pattern0();
	}
}

void App_Ws2812b_Mode_Switch(void)
{
	if(Key_Event_Check(KEY1,KEY_DOUBLE_CLICK))
	{
		ws2812b_mode++;
		if(ws2812b_mode >= WS2812B_MODE_MAX)
		  ws2812b_mode = WS2812B_MODE_CLOSE;
	}
	if(Key_Event_Check(KEY1,KEY_LONG_PRESS))
	  ws2812b_mode = WS2812B_MODE_CLOSE;
	switch(ws2812b_mode)
	{
		case WS2812B_MODE_CLOSE:
		{
			App_Ws2812b_Close();
			break;
		}
		case WS2812B_MODE_WHITELIGHT:
		{
			App_Ws2812b_WhiteLight();
			break;
		}
		case WS2812B_MODE_COLORFUL:
		{
			App_Ws2812b_Pattern_Switch();
			break;
		}
		default:
		  App_Ws2812b_Close();
		  break;
	}
}

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
