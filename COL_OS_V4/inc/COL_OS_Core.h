/*
 * COL_RTOS_Core.h
 */

#ifndef COL_OS_CORE_H_
#define COL_OS_CORE_H_

#include <stdint.h>
#include "board.h"


/************************************************************************************
 * 			Tamaño del stack predefinido para cada tarea expresado en bytes
 ***********************************************************************************/

#define STACK_SIZE 	256
#define MAX_TAREAS 	4

//----------------------------------------------------------------------------------

/************************************************************************************
 * 	Posiciones dentro del stack de los registros que lo conforman
 ***********************************************************************************/

#define XPSR			1
#define PC_REG			2
#define LR				3
#define R12				4
#define R3				5
#define R2				6
#define R1				7
#define R0				8
#define LR_PREV_VALUE	9
#define R4				10
#define R5				11
#define R6				12
#define R7				13
#define R8				14
#define R9				15
#define R10 			16
#define R11 			17

#define STACK_R0					STACK_SIZE/4 - R0
#define STACK_XPSR					STACK_SIZE/4 - XPSR
#define STACK_PC_REG				STACK_SIZE/4 - PC_REG
#define STACK_LR					STACK_SIZE/4 - LR
#define STACK_LR_PREV_VALUE			STACK_SIZE/4 - LR_PREV_VALUE
#define STACK_FULL_STACKING_SIZE	STACK_SIZE/4 - FULL_STACKING_SIZE

#define TAREA_INIT					0
#define TAREA_IDLE					1
#define NUMERO_PRIORIDADES			4
#define SIZE						NUMERO_PRIORIDADES + TAREA_IDLE


/************************************************************************************
 * 		Estructruras de datos para RTOS
 ***********************************************************************************/

typedef struct
{
	uint32_t 	stack_pointer;					// Puntero a la variable que almacena el stack pointer de la tarea.
	uint32_t 	stack_tarea[STACK_SIZE];		// Puntero al espacio reservado como stack para la tarea.
	void	   *parametros;						// Parametros de tarea
	uint32_t 	prioridad;						// Prioridad de la tarea.
	uint32_t 	id;								// id Tarea.
	uint32_t 	Tick_ini;						// id Tarea.
	uint32_t 	Tick_act;						// id Tarea.
	uint32_t 	estado_delay;							// id Tarea.

} 	tarea_t;

struct _control_t
{
	 int    	estado;								// Variable del estado del RTOS
	 int        contexto;							// Variable del estado del RTOS
	 int 	 	prioridad_actual;					// Variable de la tarea sigueinte.
	 int 	 	prioridad_siguiente;				// Variable de la tarea sigueinte.
	 int  	 	tarea_actual;						// Variable de la tarea actual.
	 int 	 	tarea_siguiente;					// Variable de la tarea sigueinte.
	 int	 	ticks;								// Variable de los tick del sistema.
	 tarea_t   *id_tarea[MAX_TAREAS*SIZE];			// Variable de identificacion de la tarea
	 uint8_t 	num_act_tareas[SIZE];				// Numero de tareas activas definidas por el usuario.
	 uint8_t 	num_ini_tareas[SIZE];				// Numero de tareas iniciales definidas por el usuario.
	 uint32_t   stack_tareas [SIZE][MAX_TAREAS];	// Arreglo con los stack_pointer para relizar el cambio de tarea.
	 int 		estado_tareas[SIZE][MAX_TAREAS];		// Estado de la tarea (RUNNING, READY y BLOCKED).
};
typedef struct  _control_t control_t;

enum _estado  {
	RTOS_RUNNING,
	RTOS_RESET,
	RTOS_IDLE
};

enum _tarea  {
	RUNNING,
	READY,
	BLOCKED
};

//----------------------------------------------------------------------------------


/************************************************************************************
 * 			Valores necesarios para registros del stack frame inicial
 ***********************************************************************************/

#define INIT_XPSR 	1 << 24				//xPSR.T = 1
#define EXEC_RETURN	0xFFFFFFF9			//retornar a modo thread con MSP, FPU no utilizada

//----------------------------------------------------------------------------------

/************************************************************************************
 * 						Definiciones varias
 ***********************************************************************************/
#define STACK_FRAME_SIZE			8
#define FULL_STACKING_SIZE 			17	//16 core registers + valor previo de LR

/*==================[definicion de datos externa]=================================*/

extern tarea_t stack[];

/*==================[definicion de prototipos]=================================*/

void Os_Blocked_tarea(int id);
void Os_unBlocked_tarea(int id);
void Os_crear_tarea(void *tarea, tarea_t *stack);


void delay_ciclo(int delay);
uint32_t Os_delay_ms(uint32_t delay, uint32_t id);
void Os_Ini_Critica(void);
void Os_Fin_Critica(void);
void Os_Init(void);

#endif /* ISO_I_2020_MSE_OS_INC_MSE_OS_CORE_H_ */
