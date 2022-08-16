/*
 * COL_RTOS_Core.h
 */

#ifndef COL_RTOS_CORE_H_
#define COL_RTOS_CORE_H_

#include <stdint.h>
#include "board.h"


/************************************************************************************
 * 			Tamaño del stack predefinido para cada tarea expresado en bytes
 ***********************************************************************************/

#define STACK_SIZE 	256
#define MAX_TAREAS 	16

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

/************************************************************************************
 * 		Estructruras de datos para RTOS
 ***********************************************************************************/

typedef struct
{
	uint32_t 	stack_tarea[STACK_SIZE];		// Puntero al espacio reservado como stack para la tarea.
	uint32_t 	stack_pointer;					// Puntero a la variable que almacena el stack pointer de la tarea.
	void		*parametros_tarea;				// Parametros de tarea
	uint32_t 	prioridad_tarea;				// Prioridad de la tarea.
	uint32_t 	id;								// id Tarea.
} 	tarea_t;


struct _control_t
{
	uint32_t 	 estado;						// Variable del estado del RTOS
	 int32_t 	 tarea_actual;					// Variable de la tarea actual.
	uint32_t 	 *stack_pointer_actual;			// Stack pointer para la sigueinte tareaParametros de tarea
	uint32_t 	 *stack_pointer_siguiente;		// Stack pointer para la sigueinte tareaParametros de tarea
	uint32_t 	 Tarea_siguiente[MAX_TAREAS];	// Arreglo para relizar el cambio de tarea.
	uint32_t 	 num_Tareas;					// Numero de tareas definidas por el usuario.
};
typedef struct  _control_t control_t;

enum _estado  {
	RTOS_RUNNING,
	RTOS_RESET
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

void crear_tarea(void *tarea, tarea_t *stack);
void RTOS_Init(void);

#endif /* ISO_I_2020_MSE_OS_INC_MSE_OS_CORE_H_ */
