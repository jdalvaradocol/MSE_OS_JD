/*==================[inclusions]=============================================*/

#include "main.h"
#include "sapi.h"
#include "board.h"

#include "MSE_OS_Core.h"


/*==================[macros and definitions]=================================*/

#define MILISEC		1000

#define NUM_TAREAS 		4

/*==================[Global data declaration]==============================*/

uint32_t stack1[STACK_SIZE];		//espacio reservado para el stack de la tarea 1
uint32_t stack2[STACK_SIZE];		//espacio reservado para el stack de la tarea 2
uint32_t stack3[STACK_SIZE];		//espacio reservado para el stack de la tarea 3
uint32_t stack4[STACK_SIZE];		//espacio reservado para el stack de la tarea 4

uint32_t TAREAS = NUM_TAREAS;
uint32_t sp_tarea[NUM_TAREAS];					//Stack Pointer para las tareas


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


int delay_ciclo(int delay)
{

	for (int volatile i = 0; i < delay; i++)
	{
		for (int volatile j = 0; j < 10000; j++)
		{}
	}
}


/*==================[Definicion de tareas para el OS]==========================*/
void tarea1(void)  {
	uint16_t h = 0;
	uint16_t i = 0;
	while (1)
	{

		gpioWrite(LEDR,FALSE);
	    delay_ciclo(200);
		gpioWrite(LEDR,TRUE);
	    delay_ciclo(200);

		h++;	i++;
	}
}

void tarea2(void)  {
	uint16_t j = 0;
	uint16_t k = 0;

	while (1)
	{

		gpioWrite(LED1,FALSE);
	    delay_ciclo(200);
		gpioWrite(LED1,TRUE);
	    delay_ciclo(200);

		j++;	k++;
	}
}

void tarea3(void)  {
	uint16_t j = 0;
	uint16_t k = 0;

	while (1)
	{

		gpioWrite(LED2,FALSE);
	    delay_ciclo(200);
		gpioWrite(LED2,TRUE);
	    delay_ciclo(200);

		j++;	k++;
	}
}

void tarea4(void)  {
	uint16_t j = 0;
	uint16_t k = 0;

	while (1)
	{

		gpioWrite(LED3,FALSE);
	    delay_ciclo(200);
		gpioWrite(LED3,TRUE);
	    delay_ciclo(200);

		j++;	k++;
	}
}

/*============================================================================*/

int main(void)  {

	initHardware();

	os_Init();

	os_InitTarea(tarea1, &stack1, &sp_tarea[0]);
	os_InitTarea(tarea2, &stack2, &sp_tarea[1]);
	os_InitTarea(tarea3, &stack3, &sp_tarea[2]);
	os_InitTarea(tarea4, &stack4, &sp_tarea[3]);

	while (1) {
	}
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/



