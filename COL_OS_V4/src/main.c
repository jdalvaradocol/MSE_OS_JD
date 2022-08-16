/*==================[inclusions]=============================================*/

#include "../../COL_RTOS/inc/main.h"

#include "board.h"
#include "COL_OS_Core.h"
#include "Teclas.h"


/*==================[macros and definitions]=================================*/

#define MILISEC		1000

/*==================[definiciones de datos internos]=========================*/

const gpioMap_t leds_t[] = {LEDB, LED1, LED2, LED3};
uint32_t led_state[4];
#define LEDS_COUNT   sizeof(leds_t)/sizeof(leds_t[0])

/*==================[Global data declaration]==============================*/

tarea_t leds[LEDS_COUNT];
tarea_t teclas_Blocked[LEDS_COUNT];
tarea_t teclas_unBlocked[LEDS_COUNT];

/*==================[internal functions declaration]=========================*/

int AB[4][2] = { 0, 0,	0, 3,	0, 6,	0, 9};

int AU[4][2] = { 3, 9,	3, 0,	3, 3,	3, 6};

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

/*==================[Definicion de tareas para el OS]==========================*/

// Implementacion de funcion de la tarea

void tarea_led(void *parameter )
{
	int id = (int) parameter;

	uint32_t volatile dato;

     while( TRUE )
    {
    	 gpioToggle( leds_t[id]);

    	 while(Os_delay_ms(500,AB[id][2]))
    	 {

    	 }
    }
}

//int index;

void tarea_tecla_Blocked(void *parameter )
{
 	int index  = (int) parameter;

	int tarea = 0, id  = 0 ;
	uint32_t event = 0;

	while( TRUE )
	{

	    id = AB[index][0];
	    tarea = AB[index][1];

		event = keys_update(id);

		 if(event == ON)
		 {
			 //index++;
			 Os_Blocked_tarea(tarea + 0);
			 Os_Blocked_tarea(tarea + 1);
			 Os_Blocked_tarea(tarea + 2);
		 }
	}
}

void tarea_tecla_unBlocked(void *parameter )
{
	int index  = (int) parameter;

	int tarea = 0, id  =0 ;

	uint32_t event = 0;

	while( TRUE )
	{
		 id = AU[index][0];
		tarea = AU[index][1];

		event = keys_update(id);

		 if(event == ON)
		 {
			 //index--;
			 Os_unBlocked_tarea(tarea + 0);
			 Os_unBlocked_tarea(tarea + 1);
			 Os_unBlocked_tarea(tarea + 2);
		 }

	 }
}

/*============================================================================*/

int main(void)  {

	initHardware();
	keys_init(LEDS_COUNT, led_state);

	for(int i= 0; i < LEDS_COUNT; i++)
	{
		leds[i].parametros = (void*) i;
		leds[i].prioridad =  i;

		teclas_Blocked[i].parametros = (void*) i;
		teclas_Blocked[i].prioridad = i;

		teclas_unBlocked[i].parametros = (void*) i;
		teclas_unBlocked[i].prioridad = i;

		Os_crear_tarea(             tarea_led, &leds[i]             );
		Os_crear_tarea(   tarea_tecla_Blocked, &teclas_Blocked[i]   );
		Os_crear_tarea( tarea_tecla_unBlocked, &teclas_unBlocked[i] );
	}

	Os_Init();

	while (1)
	{}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/



