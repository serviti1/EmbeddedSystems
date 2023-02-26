/*******************************************************************************/
/**
\file       app_tasks.h
\brief      Tasks to be assigned to each execution thread of Task scheduler.
\author     Jesus Cinco
\author     Snaider Bautista
\version    0.1
\date       04/Feb/2023
*/

#ifndef __EVENT_DRIVEN
#define __EVENT_DRIVEN

/*-- Includes ----------------------------------------------------------------*/
#include "compiler.h"
#include "typedefs.h"

/* LED control definitions */
#include "app_tasks.h"
/* LED control definitions */
#include "led_ctrl.h"

void EVT_vfnConfigureButtons(void);

void EVT_vfnConfigureGPIO(void);
#endif