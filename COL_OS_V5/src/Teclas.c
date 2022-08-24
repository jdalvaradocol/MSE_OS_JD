
/*==================[ Inclusions ]============================================*/
#include "Teclas.h"

/*=====[ Definitions of private data types ]===================================*/


/*=====[Definition macros of private constants]==============================*/

/*=====[Prototypes (declarations) of private functions]======================*/


/*=====[Definitions of private global variables]=============================*/

/*=====[Definitions of public global variables]==============================*/
keys_ButtonState_t state[4];   	// state of the key
uint32_t time_down[4];			// timestamp of the last High to Low transition of the key
uint32_t time_up[4];		    // timestamp of the last Low to High transition of the key
uint32_t time_diff[4];	   		// las time difference

const gpioMap_t tecla[4]={TEC1,TEC2,TEC3,TEC4};

/*=====[prototype of private functions]=================================*/


/*=====[Implementations of public functions]=================================*/

/**
   @brief Obtiene el tiempo de la ultima pulsacion de la tecla indicada.

   @param index
   @return TickType_t
 */
uint32_t keys_get_diff(int i)
{
	return time_diff[i];
}

/**
   @brief Borra el tiempo de la ultima pulsacion de la tecla indicada.

   @param index
 */
void keys_clear_diff(int i)
{
    time_diff[i] = KEYS_INVALID_TIME;

}

/**
   @brief Inicializa el driver

 */

void keys_init(int  NUMERO_LEDS, uint32_t led_state[4])
{

	for(int i=0;i<NUMERO_LEDS;i++)
	{
		state[i]     = BUTTON_UP;  			// Set initial state
		time_down[i] = KEYS_INVALID_TIME;
		time_up[i]   = KEYS_INVALID_TIME;
		time_diff[i] = KEYS_INVALID_TIME;
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

                time_down[i] = Readtick();
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

                int current_tick_count = Readtick();

                /* calculo el tiempo de pulsacion */
                if( time_down[i] < current_tick_count )
                {
                    time_diff[i]  = current_tick_count - time_down[i];
                }
                else
                {
                    time_diff[i]  = current_tick_count - time_down[i] + 0xFFFFFFFF;
                }
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

