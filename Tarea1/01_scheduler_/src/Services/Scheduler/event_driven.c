/** \file
 *
 *  This file contains all the specific code for the event driven homework
 *
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"
#include "pio_it.h"
#include "pio.h"
#include "event_driven.h"
#include <stdbool.h>
#include <stdio.h>

#define BUTTON_1_PRIORITY 5
/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/
const Pin pinPB1 = PIN_PUSHBUTTON_0;

const Pin MeasurementGPIO[] = {(1<<5), ((Pio *)0x400E0E00U), 10, 6, (0<<0)};

/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO 0

/**
 *  \brief Process Buttons Events
 *
 *  Change active states of LEDs when corresponding button events happened.
 */
static void ProcessButtonEvt(uint8_t taskPriority)
{
    if (taskPriority == 5)
    {
        if (ptrTask_1ms == NULL)
        {
            ptrTask_1ms = vfnLedCtrl_ToggleLed0;
        }
    }
    else if (taskPriority == 4)
    {
        if (ptrTask_2ms == NULL)
        {
            ptrTask_2ms = vfnLedCtrl_ToggleLed0;
        }
    }
    else if (taskPriority == 2)
    {
        if (ptrTask_50ms == NULL)
        {
            ptrTask_50ms = vfnLedCtrl_ToggleLed0;
        }
    }
}


/**
 *  \brief Handler for Button 1 rising edge interrupt.
 *
 *  Handle process led1 status change.
 */
static void _Button1_Handler(const Pin *pPin)
{
    if (pPin == &pinPB1)
    {
        ProcessButtonEvt(2);
        PIO_Set(&MeasurementGPIO[0]);
    }
}

/**
 *  \brief Configure the Push-buttons
 *
 *  Configure the PIO as inputs and generate corresponding interrupt when
 *  pressed or released.
 */
void EVT_vfnConfigureButtons(void)
{
    /* Configure PIO as inputs. */
    PIO_Configure(&pinPB1, 1);

    /* Adjust PIO denounce filter parameters, uses 10 Hz filter. */
    PIO_SetDebounceFilter(&pinPB1, 10);

    /* Initialize PIO interrupt handlers, see PIO definition in board.h. */
    PIO_ConfigureIt(&pinPB1, _Button1_Handler); /* Interrupt on rising edge  */

    /* Enable PIO controller IRQs. */
    NVIC_EnableIRQ((IRQn_Type)pinPB1.id);

    /* Enable PIO line interrupts. */
    PIO_EnableIt(&pinPB1);
}

void EVT_vfnConfigureGPIO(void)
{                     
  PIO_Configure(&MeasurementGPIO, 1); 
  PIO_Clear(&MeasurementGPIO[0]);

}



