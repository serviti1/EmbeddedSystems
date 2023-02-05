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
#include "event_driven.h"
#include <stdbool.h>
#include <stdio.h>

#define BUTTON_1_PRIORITY 5
#define BUTTON_2_PRIORITY 4
/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/
const Pin pinPB1 = PIN_PUSHBUTTON_0;
const Pin pinPB2 = PIN_PUSHBUTTON_1;

/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO 0

/**
 *  \brief Process Buttons Events
 *
 *  Change active states of LEDs when corresponding button events happened.
 */
static void ProcessButtonEvt(uint8_t ucButton)
{
    if (ucButton == 0)
    {
        if (ptrTask_1ms == NULL)
        {
            ptrTask_1ms = vfnLedCtrl_ToggleLed0;
        }
    }
    else
    {
        if (ptrTask_2ms == NULL)
        {
            ptrTask_2ms = vfnLedCtrl_ToggleLed1;
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
        ProcessButtonEvt(0);
    }
}

/**
 *  \brief Handler for Button 2 falling edge interrupt.
 *
 *  Handle process led2 status change.
 */
static void _Button2_Handler(const Pin *pPin)
{
    if (pPin == &pinPB2)
    {
        ProcessButtonEvt(1);
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
    PIO_Configure(&pinPB2, 1);

    /* Adjust PIO denounce filter parameters, uses 10 Hz filter. */
    PIO_SetDebounceFilter(&pinPB1, 10);
    PIO_SetDebounceFilter(&pinPB2, 10);

    /* Initialize PIO interrupt handlers, see PIO definition in board.h. */
    PIO_ConfigureIt(&pinPB1, _Button1_Handler); /* Interrupt on rising edge  */
    PIO_ConfigureIt(&pinPB2, _Button2_Handler); /* Interrupt on rising edge */

    /* Enable PIO controller IRQs. */
    NVIC_EnableIRQ((IRQn_Type)pinPB1.id);
    NVIC_EnableIRQ((IRQn_Type)pinPB2.id);

    /* Enable PIO line interrupts. */
    PIO_EnableIt(&pinPB1);
    PIO_EnableIt(&pinPB2);
}



