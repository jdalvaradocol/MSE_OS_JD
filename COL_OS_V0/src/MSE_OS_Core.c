/*
 * MSE_OS_Core.c
 *
 *  Created on: 26 mar. 2020
 *      Author: gonza
 */

#include "MSE_OS_Core.h"


/*************************************************************************************************
	 *  @brief Inicializa las tareas que correran en el OS.
     *
     *  @details
     *   Inicializa una tarea para que pueda correr en el OS implementado.
     *   Es necesario llamar a esta funcion para cada tarea antes que inicie
     *   el OS.
     *
	 *  @param *tarea			Puntero a la tarea que se desea inicializar.
	 *  @param *stack			Puntero al espacio reservado como stack para la tarea.
	 *  @param *stack_pointer   Puntero a la variable que almacena el stack pointer de la tarea.
	 *  @return     None.
***************************************************************************************************/
void os_InitTarea(void *tarea, uint32_t *stack_tarea, uint32_t *stack_pointer)  {

	stack_tarea[STACK_SIZE/4 - XPSR] = INIT_XPSR;								//necesari para bit thumb
	stack_tarea[STACK_SIZE/4 - PC_REG] = (uint32_t)tarea;		//direccion de la tarea (ENTRY_POINT)


	/**
	 * El valor previo de LR (que es EXEC_RETURN en este caso) es necesario dado que
	 * en esta implementacion, se llama a una funcion desde dentro del handler de PendSV
	 * con lo que el valor de LR se modifica por la direccion de retorno para cuando
	 * se termina de ejecutar getContextoSiguiente
	 */
	stack_tarea[STACK_SIZE/4 - LR_PREV_VALUE] = EXEC_RETURN;

	/**
	 * Notar que ahora, al agregar un registro mas al stack, la definicion de FULL_STACKING_SIZE
	 * paso de ser 16 a ser 17
	 */

	*stack_pointer = (uint32_t) (stack_tarea + STACK_SIZE/4 - FULL_STACKING_SIZE);

}


/*************************************************************************************************
	 *  @brief Inicializa el OS.
     *
     *  @details
     *   Inicializa el OS seteando la prioridad de PendSV como la mas baja posible. Es necesario
     *   llamar esta funcion antes de que inicie el sistema. Es recomendable llamarla luego de
     *   inicializar las tareas
     *
	 *  @param 		None.
	 *  @return     None.
***************************************************************************************************/
void os_Init(void)  {
	/*
	 * Todas las interrupciones tienen prioridad 0 (la maxima) al iniciar la ejecucion. Para que
	 * no se de la condicion de fault mencionada en la teoria, debemos bajar su prioridad en el
	 * NVIC. La cuenta matematica que se observa da la probabilidad mas baja posible.
	 */
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);
}


/*************************************************************************************************
	 *  @brief SysTick Handler.
     *
     *  @details
     *   El handler del Systick no debe estar a la vista del usuario. Dentro se setea como
     *   pendiente la excepcion PendSV.
     *
	 *  @param 		None.
	 *  @return     None.
***************************************************************************************************/
void SysTick_Handler(void)  {

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




/*************************************************************************************************
	 *  @brief Funcion para determinar el proximo contexto.
     *
     *  @details
     *   Esta funcion en este momento hace las veces de scheduler y tambien obtiene el siguiente
     *   contexto a ser cargado. El cambio de contexto se ejecuta en el handler de PendSV, dentro
     *   del cual se llama a esta funcion
     *
	 *  @param 		sp_actual	Este valor es una copia del contenido de MSP al momento en
	 *  			que la funcion es invocada.
	 *  @return     El valor a cargar en MSP para apuntar al contexto de la tarea siguiente.
***************************************************************************************************/
uint32_t getContextoSiguiente(uint32_t sp_actual)  {
	static int32_t tarea_actual = -1;
	uint32_t sp_siguiente;

	if(tarea_actual >= 0)
	{
		sp_tarea[tarea_actual] = sp_actual;

		if (tarea_actual < (TAREAS -1) )
		{
			tarea_actual++;
		}
		else
		{
			tarea_actual = 0;
		}

		sp_siguiente = sp_tarea[tarea_actual];
	}

	else
	{
		tarea_actual = 0;
		sp_siguiente = sp_tarea[0];
	}

	return sp_siguiente;
}
