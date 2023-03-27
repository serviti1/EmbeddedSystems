/****************************************************************************************************/
/**
\file       timer.c
\brief      MCAL abstraction level - Timer Controller configuration and interrupt handling.
\author     Abraham Tezmol
\version    1.0
\project    Tau 
\date       26/September/2016
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
/** Dac function prototypes and definitions */
#include "timer.h"
#include "dac.h"

#include "chip.h"
#include "board.h"

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/

/** Pins Configuration */

Pin TcPinsConfig =
{
  .mask          = PIO_PA0A_PWMC0_PWMH0,
  .pio           = PIOA,
  .id            = ID_PIOA,
  .type          = PIO_PERIPH_A,
  .attribute     = PIO_DEFAULT     
};

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

/****************************************************************************************************/
/**
* \brief Configure TC timer.
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
void TcCtrl_Configure (void)
{
  uint32_t Div = 0;
  uint32_t TcClk = 0;

  /* Enable Timer Counter Controller */
  PMC_EnablePeripheral(ID_TC0);

  /* Configure TC for a 1000Hz frequency and trigger on RC compare. */ 
  TC_FindMckDivisor( 1000, BOARD_MCK, &Div, &TcClk, BOARD_MCK );

  /* Configure a TC channel with the given parameters, basic configure function */
  TC_Configure( TC0, TC_CHANNEL0, TcClk | TC_CMR_CPCTRG | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET );

  TC0 -> TC_CHANNEL[TC_CHANNEL0].TC_RC = ( BOARD_MCK / Div ) / 1000;
  TC0 -> TC_CHANNEL[TC_CHANNEL0].TC_RA = ( BOARD_MCK / Div ) / 2;
}

/**
* \brief    Start.
* \author   Abraham Tezmol
* \param    void
* \return   void
*/

void TcCtrl_Start(void)
{
  TC_Start(TC0, TC_CHANNEL0);
}

