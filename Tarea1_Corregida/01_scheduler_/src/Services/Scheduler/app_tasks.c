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
#include "app_scheduler.h"
#include "board.h"
#include "event_driven.h"

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
  vfnSchedulePoint();
}

/* List of tasks to be executed @ 2ms, first group */
void TASKS_LIST_2MS_A(void)
{
  vfnSchedulePoint();
}
/* List of tasks to be executed @ 2ms, second group */
void TASKS_LIST_2MS_B(void)
{
  vfnSchedulePoint();
}

/* List of tasks to be executed @ 10ms */
void TASKS_LIST_10MS(void)
{
  vfnSchedulePoint();
}
/* List of tasks to be executed @ 50ms */
void TASKS_LIST_50MS(void)
{
  vfnSchedulePoint();
}
/* List of tasks to be executed @ 100ms */
void TASKS_LIST_100MS(void)
{
  vfnSchedulePoint();
}

void TASK_BUTTON0_EVENT(void)
{
  PIO_Clear(&MeasurementGPIO[0]);
  vfnLedCtrl_ToggleLed0();
}
