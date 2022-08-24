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

tarea_t   leds[LEDS_COUNT];
tarea_t teclas[LEDS_COUNT];
 cola_t  colas[LEDS_COUNT];

/*==================[internal functions declaration]=========================*/

int index_tareas[4] = { 0, 2, 4 ,6 };

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
	int index = (int) parameter;

	uint32_t  delay;

     while( TRUE )
    {
    	 delay = Os_cola_leer(&colas[index], index, unBLOCKED);
    	 gpioWrite( leds_t[index], ON);
    	 Os_delay_ms(delay,index_tareas[index]);
    	 gpioWrite( leds_t[index], OFF);
    }
}

//int index;

void tarea_tecla(void *parameter )
{
	int index  = (int) parameter;

	uint32_t event = 0;
	uint32_t diff = 0;

	while( TRUE )
	{
		event = keys_update(index);

		if(event == KEYS_EVENT_KEY_UP)
		{
			diff = keys_get_diff(index);
			Os_cola_escribir(&colas[index], diff, index, unBLOCKED);
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
		leds[i].prioridad =  0;

		teclas[i].parametros = (void*) i;
		teclas[i].prioridad = 0;

		Os_crear_tarea(   tarea_led,   &leds[i] );
		Os_crear_tarea( tarea_tecla, &teclas[i] );

		Os_cola_init(&colas[i],12);

	}

	Os_Init();

	while (1)
	{}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/



