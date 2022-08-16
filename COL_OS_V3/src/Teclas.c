/*=============================================================================
 * Copyright (c) 2021, Franco Bucafusco <franco_bucafusco@yahoo.com.ar>
 * 					   Martin N. Menendez <mmenendez@fi.uba.ar>
 * All rights reserved.
 * License: Free
 * Date: 2021/10/03
 * Version: v1.0
 *===========================================================================*/

/*==================[ Inclusions ]============================================*/
#include "../inc/Teclas.h"

keys_ButtonState_t state[4];   	//state of the key

const gpioMap_t tecla[4]={TEC1,TEC2,TEC3,TEC4};

void keys_init(int  NUMERO_LEDS, uint32_t led_state[4])
{

	for(int i=0;i<NUMERO_LEDS;i++)
	{
		state[i]     = BUTTON_UP;  			// Set initial state
		led_state[i] = STATE_OFF;
	}

}

// keys_ Update State Function
/**
   @brief Keys SM for polled operation

 */
uint32_t  keys_update(int i)
{
    uint32_t event = KEYS_EVENT_NONE ;

    switch(state[i])
    {
        case STATE_BUTTON_UP:
            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( tecla[i] ) )
            {
                state[i] = STATE_BUTTON_FALLING;
            }
            break;

        case STATE_BUTTON_FALLING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */
            if( !gpioRead( tecla[i] ) )
            {
                state[i] = STATE_BUTTON_DOWN;

                /* ACCION DEL EVENTO !*/
                event = KEYS_EVENT_KEY_DOWN;

            }
            else
            {
                state[i] = STATE_BUTTON_UP;
            }

            /* LEAVE */
            break;

        case STATE_BUTTON_DOWN:
            /* CHECK TRANSITION CONDITIONS */
            if( gpioRead( tecla[i] ) )
            {
                state[i] = STATE_BUTTON_RISING;
            }
            break;

        case STATE_BUTTON_RISING:
            /* ENTRY */

            /* CHECK TRANSITION CONDITIONS */

            if( gpioRead( tecla[i] ) )
            {
                state[i] = STATE_BUTTON_UP;

                /* ACCION DEL EVENTO ! */
                event = KEYS_EVENT_KEY_UP;

            }
            else
            {
                state[i] = STATE_BUTTON_DOWN;
            }

            /* LEAVE */
            break;
    }

    return event;

}

/*=====[Implementations of private functions]================================*/

/*=====[Implementations of private functions]=================================*/

