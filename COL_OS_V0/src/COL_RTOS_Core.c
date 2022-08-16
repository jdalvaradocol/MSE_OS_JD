/*
 * COL_RTOS_C0re.c
 *
 */

#include "COL_RTOS_Core.h"

static control_t control;

// void crear_tarea(void *tarea, uint32_t *stack_tarea, uint32_t *stack_pointer)  {

void crear_tarea(void *tarea, tarea_t *stack)
{

	stack->stack_tarea[STACK_SIZE/4 -          XPSR] = INIT_XPSR;				// Necesaria para bit thumb
	stack->stack_tarea[STACK_SIZE/4 -        PC_REG] = (uint32_t)tarea;			// Direccion de la tarea (ENTRY_POINT)
	stack->stack_tarea[STACK_SIZE/4 - LR_PREV_VALUE] = EXEC_RETURN;				// Se asigna EXEC_RETURN en el stack

	stack->stack_pointer  = (uint32_t) (stack->stack_tarea + STACK_SIZE/4 - FULL_STACKING_SIZE); // Definicion del FULL_STACKING_SIZE

	control.num_Tareas++;

}

void RTOS_Init(void)
{
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);

	control.estado = RTOS_RESET;
	control.num_Tareas = 0;
	control.tarea_actual = -1;


}

static void scheduler(void)
{

	if(control.estado == RTOS_RUNNING)
	{

		if (control.tarea_actual < (control.num_Tareas) )
		{
			control.tarea_actual++;
		}
		else
		{
			control.tarea_actual = 0;
		}


	}
	else if(control.estado == RTOS_RESET)
	{
		control.estado = RTOS_RUNNING;
		control.tarea_actual = 0;

		control.Tarea_siguiente[control.num_Tareas-1] = 0;

		for(int i=0 ;i < control.num_Tareas-1 ; i++ )
		{
			control.Tarea_siguiente[i] = i + 1;
		}
	}

	else
	{
		// ERROR
	}

}

void SysTick_Handler(void)
{

	scheduler();

	/**
	 * Se setea el bit correspondiente a la excepcion PendSV
	 */
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;

	/**
	 * Instruction Synchronization Barrier; flushes the pipeline and ensures that
	 * all previous instructions are completed before executing new instructions
	 */
	__ISB();

	/**
	 * Data Synchronization Barrier; ensures that all memory accesses are
	 * completed before next instruction is executed
	 */
	__DSB();
}


uint32_t getContextoSiguiente(uint32_t stack_pointer_actual)
{

	static int32_t tarea_actual = -1;

	uint32_t 	stack_pointer_siguiente  ;

	if(control.estado == RTOS_RUNNING)
	{
		stack[tarea_actual].stack_pointer = stack_pointer_actual;

		if (tarea_actual < (control.num_Tareas - 1))
		{
			tarea_actual++;
		}
		else
		{
			tarea_actual = 0;
		}

		stack_pointer_siguiente = stack[tarea_actual].stack_pointer;

	}
	else if(control.estado == RTOS_RESET)
	{
		stack_pointer_siguiente = stack[0].stack_pointer;
		tarea_actual = 0;
	}
	else
	{
		// ERROR
	}
	return stack_pointer_siguiente;

}
