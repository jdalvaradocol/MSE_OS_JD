/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"
#include "board.h"

#include "../inc/COL_RTOS_Core.h"


/*==================[macros and definitions]=================================*/

#define MILISEC		1000

/*==================[definiciones de datos internos]=========================*/
const gpioMap_t leds_t[] = {LEDB, LED1, LED2, LED3};

#define LEDS_COUNT   sizeof(leds_t)/sizeof(leds_t[0])

/*==================[Global data declaration]==============================*/

tarea_t stack[LEDS_COUNT];

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
void tarea_led1(void)
{
     while( TRUE )
    {
        gpioWrite( leds_t[0], ON );
	    delay_ciclo(200);
        gpioWrite( leds_t[0], OFF );
   	    delay_ciclo(200);

    }
}
void tarea_led2(void)
{
     while( TRUE )
    {
        gpioWrite( leds_t[1], ON );
	    delay_ciclo(200);
        gpioWrite( leds_t[1], OFF );
   	    delay_ciclo(200);

    }
}
void tarea_led3(void)
{
     while( TRUE )
    {
        gpioWrite( leds_t[2], ON );
	    delay_ciclo(200);
        gpioWrite( leds_t[2], OFF );
   	    delay_ciclo(200);

    }
}
void tarea_led4(void)
{
     while( TRUE )
    {
        gpioWrite( leds_t[3], ON );
	    delay_ciclo(200);
        gpioWrite( leds_t[3], OFF );
   	    delay_ciclo(200);

    }
}

/*============================================================================*/

int main(void)  {

	initHardware();

	RTOS_Init();

	crear_tarea(tarea_led1, &stack[0]);
	crear_tarea(tarea_led2, &stack[1]);
	crear_tarea(tarea_led3, &stack[2]);
	crear_tarea(tarea_led4, &stack[3]);

	while (1)
	{}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/



