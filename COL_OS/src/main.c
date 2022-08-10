/*==================[inclusions]=============================================*/

#include "../../COL_RTOS/inc/main.h"

#include "sapi.h"
#include "board.h"
#include "../inc/COL_OS_Core.h"
#include "../inc/Teclas.h"


/*==================[macros and definitions]=================================*/

#define MILISEC		1000

/*==================[definiciones de datos internos]=========================*/

const gpioMap_t leds_t[] = {LEDB, LED1, LED2, LED3};
uint32_t led_state[4];
#define LEDS_COUNT   sizeof(leds_t)/sizeof(leds_t[0])

/*==================[Global data declaration]==============================*/

tarea_t leds[LEDS_COUNT];
tarea_t teclas[LEDS_COUNT];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/** @brief hardware initialization function
 *	@return none
 */
static void initHardware(void)  {
	Board_Init();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / MILISEC);		//systick 1ms

}
/** @brief Funcion para realizar delay bloqueantes
 *	@return none
 */

void delay_ciclo(int delay)
{

	for (int volatile i = 0; i < delay; i++)
	{
		for (int volatile j = 0; j < 10000; j++)
		{}
	}
}


/*==================[Definicion de tareas para el OS]==========================*/

// Implementacion de funcion de la tarea

void tarea_led(void *parameter )
{
	int id = (int) parameter;

     while( TRUE )
    {
    	 gpioToggle( leds_t[id]);
      	 delay_ciclo(100);
    }
}

void tarea_tecla(void *parameter )
{
	int id = (int) parameter;

	uint32_t volatile event = 0;
	bool_t state = FALSE;

	while( TRUE )
	{

		event = keys_update(id);

		 if(event == ON)
		 {
			 if (state == TRUE)
			 {
				 state = FALSE;
			 }
			 else
			 {
				 state = TRUE;
			 }

			 if(state == TRUE)
			 {
				 Os_Blocked_tarea(id);
			 }
			 else if(state == FALSE)
			 {
				 Os_unBlocked_tarea(id);
			 }

		 }
	}
}


/*============================================================================*/

int main(void)  {

	initHardware();
	keys_init(LEDS_COUNT, led_state);

	for(int i= 0; i < LEDS_COUNT; i++)
	{
		  leds[i].parametros = (void*)i;
		teclas[i].parametros = (void*)i;
		Os_crear_tarea(tarea_led, &leds[i]);
		Os_crear_tarea(tarea_tecla, &teclas[i]);
	}

	Os_Init();

	while (1)
	{}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/



