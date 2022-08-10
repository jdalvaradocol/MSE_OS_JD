/*
 * COL_RTOS_C0re.c
 *
 */

#include "../inc/COL_OS_Core.h"

static control_t control;

/*==================[definicion de hooks debiles]=================================*/

/*************************************************************************************************
 * *	Hook de retorno de tareas
***************************************************************************************************/
void __attribute__((weak)) returnHook(void)
{
	while(1);
}

void __attribute__((weak)) tickHook(void)
{
	__asm volatile( "nop" );
}

void __attribute__((weak)) errorHook(void *caller)
{
	/*
	 * Revisar el contenido de control_OS.error para obtener informacion. Utilizar os_getError()
	 */
	while(1);
}

void __attribute__((weak)) idleTask(void)
{
	while(1)  {
		__WFI();
	}
}

void Os_Ini_Critica(void)
{
	__disable_irq();
}
void Os_Fin_Critica(void)
{
	__enable_irq();
}

void Os_crear_tarea(void *ptr_funct, tarea_t *tarea)
{
	static int index_tarea = 0;

	tarea->stack_tarea[STACK_R0]            = tarea->parametros;
	tarea->stack_tarea[STACK_XPSR]          = INIT_XPSR;									// Necesaria para bit thumb
	tarea->stack_tarea[STACK_PC_REG]        = (uint32_t)ptr_funct;							// Direccion de la tarea (ENTRY_POINT)
	tarea->stack_tarea[STACK_LR_PREV_VALUE] = EXEC_RETURN;									// Se asigna EXEC_RETURN en el stack

	tarea->stack_tarea[STACK_LR] = (uint32_t)returnHook;									//Retorno de la tarea (no deberia darse)
	tarea->stack_pointer  = (uint32_t) (tarea->stack_tarea + STACK_FULL_STACKING_SIZE); 	// Definicion del FULL_STACKING_SIZE
	tarea->id = index_tarea;

	control.estado_tareas[0][index_tarea] = READY ;
	control.stack_tareas[0][index_tarea] = (void*)tarea->stack_pointer ;
	control.num_act_tareas[0]++;
	index_tarea++;

}

void Os_Blocked_tarea(int id)
{
	int index=0;
	int contador=0;

	Os_Ini_Critica();

	control.tarea_actual = control.tarea_siguiente;
	control.tarea_siguiente = control.tarea_actual + 1;

	if(control.tarea_siguiente >= (control.num_ini_tareas[0]))
	{
		control.tarea_siguiente = 0;
	}

	control.estado = RTOS_BLOCKED;
	control.estado_tareas[0][id] = BLOCKED ;
	control.num_act_tareas[0]--;

	Os_Fin_Critica();

}

void Os_unBlocked_tarea(int id)
{

	control.tarea_actual = control.tarea_siguiente;
	control.tarea_siguiente = control.tarea_actual + 1;

	if(control.tarea_siguiente >= (control.num_ini_tareas[0]))
	{
		control.tarea_siguiente = 0;
	}

	control.num_act_tareas[0]++;
	control.estado_tareas[0][id] = READY ;
}


void Os_Init(void)
{
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);

	control.num_ini_tareas[0] = control.num_act_tareas[0];

	control.estado = RTOS_RESET;
	control.contexto = TRUE;
}

static void scheduler(void)
{

	 int index = 0;

	 index = control.tarea_actual;

	 if(index == TAREA_INIT)
	 {
		 index = 0 ;
	 }

	 if(control.estado == RTOS_RUNNING)
	 {
		if( (control.estado_tareas[0][index] == READY) && (control.estado_tareas[0][index] == BLOCKED))
		{
			control.contexto = TRUE;
		}

		control.tarea_actual+=1;
		index = control.tarea_actual;

		if(control.tarea_actual >= (control.num_ini_tareas[0]))
		{
			control.tarea_actual = 0;

			if(control.estado_tareas[0][0] == BLOCKED)
			{
				control.tarea_actual+=1;
			}
		}

		if(control.estado_tareas[0][index] == BLOCKED)
		{
			control.tarea_actual+=1;
		}

		control.tarea_siguiente = control.tarea_actual + 1;

		if(control.tarea_siguiente >= (control.num_ini_tareas[0]))
		{
			control.tarea_siguiente = 0;
		}

	    if(control.estado_tareas[0][control.tarea_siguiente] == BLOCKED)
		{
			control.tarea_siguiente+=1;
		}
	 }
	 else if(control.estado == BLOCKED)
	 {
		 control.tarea_actual+=1;

		if(control.tarea_actual >= (control.num_ini_tareas[0]))
		{
			control.tarea_actual = 0;
		}

		control.tarea_siguiente = control.tarea_actual + 1;

	 }

	 else
	 {
		 // ERROR
	 }

}

void SysTick_Handler(void)
{

	scheduler();

	if (control.contexto == TRUE)
	{
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
		__ISB();
		__DSB();
	}
}

uint32_t getContextoSiguiente(uint32_t stack_pointer_actual)
{
	uint32_t 		stack_pointer_siguiente  ;
	int  volatile 	index_sig = 0;
	int  volatile 	index_act = 0;

	index_act = control.tarea_actual;
	index_sig = control.tarea_siguiente;

	if(control.estado == RTOS_RUNNING)
	{
		control.stack_tareas[0][index_act] = (void*) stack_pointer_actual;
		stack_pointer_siguiente = (uint32_t) control.stack_tareas[0][index_sig];
	}
	else if(control.estado == RTOS_RESET)
	{
		stack_pointer_siguiente = (uint32_t) control.stack_tareas[0][0];
		control.estado       = RTOS_RUNNING;
		control.tarea_actual = TAREA_INIT;
		control.tarea_siguiente = control.tarea_actual + 1;
	}
	else if(control.estado == RTOS_BLOCKED)
	{
		control.stack_tareas[0][control.tarea_actual] = (void*) stack_pointer_actual;
		stack_pointer_siguiente = (uint32_t)control.stack_tareas[0][index_sig];
		control.estado       = RTOS_RUNNING;
	}
	else
	{
		// ERROR
	}
	return stack_pointer_siguiente;

}



