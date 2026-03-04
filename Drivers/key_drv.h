#ifndef __KEY_DRV_H
#define __KEY_DRV_H

#include <stdint.h>
#include "stm32f10x.h"

/* Configuration information in Key.c */

#define        KEY_MAX_NUM               (3)
#define        KEY1                      (0)
#define        KEY2                      (1)
#define        KEY3                      (2)

#define        KEY_DEBOUNCE_MS           (20)
#define        KEY_LONG_PRESS_MS         (2000)
#define        KEY_DOUBLE_CLICK_MS       (200)
#define        KEY_REPEAT_HOLD_MS        (100)


typedef enum
{
	KEY_UNPRESS = 0,
  KEY_PRESSED,
}key_action_t;

typedef enum
{
  KEY_IS_IDLE,
	KEY_IS_PRESSED,
	KEY_IS_RELEASED,
	KEY_IS_DOUBLECLICK,
	KEY_IS_LONGPRESS,
}key_state_t;

typedef enum
{
	KEY_HOLD         = 0x01,
	KEY_DOWN_EDGE    = 0x02,
	KEY_UP_EDGE      = 0x04,
	KEY_SINGLE_CLICK = 0x08,
	KEY_DOUBLE_CLICK = 0x10,
	KEY_LONG_PRESS   = 0x20,
	KEY_REPEAT_HOLD  = 0x40,
	KEY_UNDEFINE     = 0x80,
}key_event_t;

typedef struct
{
	uint32_t clock;
  GPIO_TypeDef *port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
}key_list_t;

void Key_Drv_Init(void);
uint8_t Key_Event_Check(uint8_t key_index,uint8_t flag);
void Key_Tick(void);

#endif
