#include "stm32f10x.h"                  // Device header
#include "timer.h"
#include "key_drv.h"


/*  uint8_t key_flag  Definition

7     6       5       4       3       2       1       0
NA   Repeat  Long   Double  Single    Up     Down    Hold

*/


/******Private Variation define******/
static key_list_t key_info[] = 
{
  {RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_1,GPIO_Mode_IPU},
	{RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_6,GPIO_Mode_IPU},
	{RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_4,GPIO_Mode_IPU},
	//add your key here.
};



/******Global Variation define******/
uint8_t key_flag[KEY_MAX_NUM] = {0};
uint8_t Key_Num = 0;


/**
* @brief  Get key state:pressed or release.
  * @param  None.
  * @retval KEY_PRESSED:
            KEY_UNPRESS:
  */
static uint8_t Key_State_Get(uint8_t index)
{
	switch(index)
	{
	  case KEY1:
		{
		  if(GPIO_ReadInputDataBit(key_info[index].port,key_info[index].pin) == 0)
			{
				return KEY_PRESSED;
			}
			break;
		}
		case KEY2:
		{
		  if(GPIO_ReadInputDataBit(key_info[index].port,key_info[index].pin) == 0)
			{
				return KEY_PRESSED;
			}
			break;
		}
		case KEY3:
		{
		  if(GPIO_ReadInputDataBit(key_info[index].port,key_info[index].pin) == 0)
			{
				return KEY_PRESSED;
			}
			break;
		}
		default:
			break;
	}
	return KEY_UNPRESS;
}

/**
  * @brief  Function of judging what key_event is.
            key_hold  key_down  key_up key_single key_double key_long key_repeat
  * @param  None.
  * @retval 1:Capture the key_event
            0:No key_event is captured.
  */
uint8_t Key_Event_Check(uint8_t key_index,uint8_t flag)
{
  if(key_flag[key_index] & flag)
	{
		if(flag != KEY_HOLD)
		{
		  key_flag[key_index] &= ~flag;
		}
	  return 1;
	}
	return 0;
}

/**
  * @brief  Function of key-check.
            which is called by timer2 interrupt per @BUTTON_DEBOUNCE_MS ms. /timer.c
  * @param  None.
  * @retval None
  */
void Key_Tick(void)
{
  static uint8_t key_cnt,i;
	static uint8_t curr_state[KEY_MAX_NUM];
	static uint8_t state[KEY_MAX_NUM];
	static uint16_t key_run_time[KEY_MAX_NUM];
	
	for(i = 0;i < KEY_MAX_NUM;i++)
	{
	  if(key_run_time[i] > 0)
		{
			key_run_time[i]--;
		}
	}
	
	key_cnt++;
	if(key_cnt >= KEY_DEBOUNCE_MS)
	{
		key_cnt = 0;
		for(i = 0;i < KEY_MAX_NUM;i++)
		{
			curr_state[i] = Key_State_Get(i);
//			if(curr_state[i] == KEY_PRESSED)  //key_hold
//			{
//				key_flag[i] |= KEY_HOLD;
//			}
//			else
//			{
//				key_flag[i] &= ~KEY_HOLD;
//			}
//			
//			if(curr_state[i] == KEY_PRESSED && prev_state[i] == KEY_UNPRESS)//key_down
//			{
//				key_flag[i] |= KEY_DOWN_EDGE;
//			}
//			
//			if(curr_state[i] == KEY_UNPRESS && prev_state[i] == KEY_PRESSED)//key_up
//			{
//				key_flag[i] |= KEY_UP_EDGE;
//			}
			
			//State Machine
			if(state[i] == KEY_IS_IDLE)
			{
				if(curr_state[i] == KEY_PRESSED)
				{
					key_run_time[i] = KEY_LONG_PRESS_MS;
					state[i] = KEY_IS_PRESSED;
				}
			}
			else if(state[i] == KEY_IS_PRESSED)
			{
				if(curr_state[i] == KEY_UNPRESS)
				{
					key_run_time[i] = KEY_DOUBLE_CLICK_MS;//set double time
					state[i] = KEY_IS_RELEASED;
				}
				else if(key_run_time[i] == 0)
				{
					key_run_time[i] = KEY_REPEAT_HOLD_MS;// set repeat time
					key_flag[i] |= KEY_LONG_PRESS;
					state[i] = KEY_IS_LONGPRESS;
				}
			}
			else if(state[i] == KEY_IS_RELEASED)
			{
				if(curr_state[i] == KEY_PRESSED)
				{
					key_flag[i] |= KEY_DOUBLE_CLICK;
					state[i] = KEY_IS_DOUBLECLICK;
				}
				else if(key_run_time[i] == 0)
				{
					key_flag[i] |= KEY_SINGLE_CLICK;
					state[i] = KEY_IS_IDLE;
				}
			}
			else if(state[i] == KEY_IS_DOUBLECLICK)
			{
				if(curr_state[i] == KEY_UNPRESS)
				{
					state[i] = KEY_IS_IDLE;
				}
			}
			else if(state[i] == KEY_IS_LONGPRESS)
			{
				if(curr_state[i] == KEY_UNPRESS)
				{
					state[i] = KEY_IS_IDLE;
				}
				else if(key_run_time[i] == 0)
				{
					key_run_time[i] = KEY_REPEAT_HOLD_MS;// reset repeat time
					key_flag[i] |= KEY_REPEAT_HOLD;
					state[i] = KEY_IS_LONGPRESS;
				}
			}
		}
	}
}
/**
  * @brief  Initialization of the key on the board.
  * @param  None.
  * @retval None
  */
void Key_Drv_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	for(uint8_t i = 0;i < KEY_MAX_NUM;i++)
	{
	  RCC_APB2PeriphClockCmd(key_info[i].clock,ENABLE);
		GPIO_InitStructure.GPIO_Mode = key_info[i].mode;
	  GPIO_InitStructure.GPIO_Pin = key_info[i].pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(key_info[i].port, &GPIO_InitStructure);
	}
}

