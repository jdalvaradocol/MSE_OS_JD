/*=============================================================================
 * Copyright (c) 2021, Franco Bucafusco <franco_bucafusco@yahoo.com.ar>
 * 					   Martin N. Menendez <mmenendez@fi.uba.ar>
 * All rights reserved.
 * License: Free
 * Date: 2021/10/03
 * Version: v1.0
 *===========================================================================*/

/*==================[ Inclusions ]============================================*/
#ifndef Teclas_H_
#define Teclas_H_
#include "sapi.h"

/* public macros ================================================================= */
#define KEYS_INVALID_TIME   -1
#define DEBOUNCE_TIME       40

/* types ================================================================= */

typedef enum {
	STATE_OFF,
	STATE_ON
} led_state_t;

typedef enum
{
    STATE_BUTTON_UP,
    STATE_BUTTON_DOWN,
    STATE_BUTTON_FALLING,
    STATE_BUTTON_RISING
} keys_ButtonState_t;


typedef struct
{
    keys_ButtonState_t state;   //variables
} t_key_data;

#define KEYS_EVENT_NONE     0
#define KEYS_EVENT_KEY_DOWN 1
#define KEYS_EVENT_KEY_UP   2

/* methods ================================================================= */

uint32_t  keys_update(int i);
void keys_init(int  NUMERO_LEDS, uint32_t led_state[4]);

#endif /* PDM_ANTIRREBOTE_MEF_INC_DEBOUNCE_H_ */
