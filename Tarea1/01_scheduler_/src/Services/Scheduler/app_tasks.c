/****************************************************************************************************/
/**
\file       app_tasks.c
\brief      Multi-thread Task scheduler - list of tasks.
\author     Abraham Tezmol
\version    1.0
\date       07/09/2013
*/
/****************************************************************************************************/

/*****************************************************************************************************
 * Include files
 *****************************************************************************************************/

/** Scheduler function prototypes definitions */
#include "app_tasks.h"

#include "board.h"

/*****************************************************************************************************
 * Definition of  VARIABLEs -
 *****************************************************************************************************/
tPtr_to_function ptrTask_1ms = NULL;
tPtr_to_function ptrTask_2ms = NULL;
tPtr_to_function ptrTask_10ms = NULL;
tPtr_to_function ptrTask_50ms = NULL;
tPtr_to_function ptrTask_100ms = NULL;

extern const Pin MeasurementGPIO[];
/*****************************************************************************************************
 * Definition of module wide (CONST-) CONSTANTs
 *****************************************************************************************************/

/*****************************************************************************************************
 * Code of module wide FUNCTIONS
 *****************************************************************************************************/

/* List of tasks to be executed @ 1ms */
void TASKS_LIST_1MS(void)
{
  if (ptrTask_1ms != NULL)
  {
    PIO_Clear(&MeasurementGPIO[0]);
    ptrTask_1ms();
  }
  ptrTask_1ms = NULL;
}

/* List of tasks to be executed @ 2ms, first group */
void TASKS_LIST_2MS_A(void)
{
  if (ptrTask_2ms != NULL)
  {
    PIO_Clear(&MeasurementGPIO[0]);
    ptrTask_2ms();
  }
  ptrTask_2ms = NULL;
}
/* List of tasks to be executed @ 2ms, second group */
void TASKS_LIST_2MS_B(void)
{
  ;
}

/* List of tasks to be executed @ 10ms */
void TASKS_LIST_10MS(void)
{
  if (ptrTask_10ms != NULL)
  {
    ptrTask_10ms();
  }
  ptrTask_10ms = NULL;
}
/* List of tasks to be executed @ 50ms */
void TASKS_LIST_50MS(void)
{
  if (ptrTask_50ms != NULL)
  {
    PIO_Clear(&MeasurementGPIO[0]);
    ptrTask_50ms();
  }
  ptrTask_50ms = NULL;
}
/* List of tasks to be executed @ 100ms */
void TASKS_LIST_100MS(void)
{
  if (ptrTask_100ms != NULL)
  {
    ptrTask_100ms();
  }
  ptrTask_100ms = NULL;
}
