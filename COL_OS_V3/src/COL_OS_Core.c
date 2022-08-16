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
	static int index_tarea[SIZE];
	static int index = 0;

	tarea->stack_tarea[STACK_R0]            = tarea->parametros;
	tarea->stack_tarea[STACK_XPSR]          = INIT_XPSR;									// Necesaria para bit thumb
	tarea->stack_tarea[STACK_PC_REG]        = (uint32_t)ptr_funct;							// Direccion de la tarea (ENTRY_POINT)
	tarea->stack_tarea[STACK_LR_PREV_VALUE] = EXEC_RETURN;									// Se asigna EXEC_RETURN en el stack

	tarea->stack_tarea[STACK_LR] = (uint32_t)returnHook;									//Retorno de la tarea (no deberia darse)
	tarea->stack_pointer  = (uint32_t) (tarea->stack_tarea + STACK_FULL_STACKING_SIZE); 	// Definicion del FULL_STACKING_SIZE
	tarea->id = index_tarea[tarea->prioridad];

	control.id_tarea[index] = tarea;
	index++;

	control.estado_tareas[tarea->prioridad][index_tarea[tarea->prioridad]] = READY ;
	control.stack_tareas[tarea->prioridad][index_tarea[tarea->prioridad]]  = (void*)tarea->stack_pointer ;
	control.num_act_tareas[tarea->prioridad]++;
	index_tarea[tarea->prioridad]++;

}

void Os_Blocked_tarea(int tarea)
{
	int id = 0;
	int prioridad = 0;

	id = control.id_tarea[tarea]->id;
	prioridad = control.id_tarea[tarea]->prioridad;

	control.estado_tareas[prioridad][id] = BLOCKED ;

	if(control.num_act_tareas[prioridad] > 0)
	{
		control.num_act_tareas[prioridad]--;
	}
}

void Os_unBlocked_tarea(int tarea)
{
	int id = 0;
	int prioridad = 0;

	id = control.id_tarea[tarea]->id;
	prioridad = control.id_tarea[tarea]->prioridad;

	control.estado_tareas[prioridad][id] = READY ;
    control.estado_tareas[prioridad][control.tarea_actual] = READY;

    if(control.num_act_tareas[prioridad] <  control.num_ini_tareas[prioridad])
    {
    	control.num_act_tareas[prioridad]++;
    }
}

void Os_Init(void)
{

	for(int i=0;i < NUMERO_PRIORIDADES;i++)
	{
		control.num_ini_tareas[i] = control.num_act_tareas[i];
	}

	control.num_ini_tareas[4] = 1;	// Asigna la Tarea_IDLE en la prioridad mas baja para

	control.estado    = RTOS_RESET;
	control.contexto  = TRUE;
	control.prioridad_actual = 0;
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);
}

void Os_prioridad(void)
{
	int i = 0;

	control.prioridad_actual = control.prioridad_siguiente;

	while(control.num_act_tareas[i] == 0)
	{
		i++;
	}

	if(i >= NUMERO_PRIORIDADES)
	{
		i = NUMERO_PRIORIDADES;
	}

	control.prioridad_siguiente = i;

}


static void Os_scheduler(void)
{
	 int index = 0;
	 int prioridad;

	 prioridad = control.prioridad_siguiente;

	 if(control.estado == RTOS_RUNNING)
	 {
		control.tarea_actual = control.tarea_siguiente;
		index = control.tarea_actual + 1;

		if(index >= (control.num_ini_tareas[prioridad])){ index = 0;}

		// Busca la tarea siguiente que se encuentre en READY.

		while(control.estado_tareas[prioridad][index] == BLOCKED)
		{
			index++;
			if(index >= (control.num_ini_tareas[prioridad])) {index = 0;}
		}

		control.tarea_siguiente = index;

		if( (control.estado_tareas[prioridad][index] == READY) || (control.estado_tareas[prioridad][index] == BLOCKED))
		{
			control.contexto = TRUE;
		}
		else if (control.estado_tareas[prioridad][index] == RUNNING)
		{
			control.contexto = FALSE;
		}

	 }
	 else
	 {
		 // ERROR
	 }

}

void SysTick_Handler(void)
{

	Os_prioridad();

	Os_scheduler();

	if (control.contexto == TRUE)
	{
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
		__ISB();
		__DSB();
	}
}

uint32_t getContextoSiguiente(uint32_t stack_pointer_actual)
{
	int index_sig = 0;
	int index_act = 0;
	int prioridad_act = 0;
	int prioridad_sig = 0;
	uint32_t stack_pointer_siguiente  ;

	prioridad_act = control.prioridad_actual;
	prioridad_sig = control.prioridad_siguiente;
	index_act = control.tarea_actual;
	index_sig = control.tarea_siguiente;

	if(control.estado == RTOS_RUNNING)
	{
		control.stack_tareas[prioridad_act][index_act] = stack_pointer_actual;
		stack_pointer_siguiente = control.stack_tareas[prioridad_sig][index_sig];

		control.estado_tareas[prioridad_act][index_act] = READY;
	    control.estado_tareas[prioridad_sig][index_sig] = RUNNING;

	}
	else if(control.estado == RTOS_RESET)
	{
		stack_pointer_siguiente = control.stack_tareas[prioridad_act][0];
		control.tarea_siguiente = TAREA_INIT;
		control.estado       	= RTOS_RUNNING;
	}
	else
	{
		// ERROR
	}
	return stack_pointer_siguiente;

}



