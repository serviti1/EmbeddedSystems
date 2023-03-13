/*******************************************************************************/
/**
\file       Uart.h
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

#ifndef _UART_H        /*prevent duplicated includes*/
	#define _UART_H

	/** Standard Types */
  #include "Uart_Types.h"
	#include "Uart_Cfg.h"

  // /* UART Masks definitions */
  // uint32_t UART_MASK_RXRDY   = 1u;
  // uint32_t UART_MASK_TXRDY   = 2u;
  // uint32_t UART_MASK_OVRE    = 32u;
  // uint32_t UART_MASK_FRAME   = 64u;
  // uint32_t UART_MASK_PARE    = 128u;
  // uint32_t UART_MASK_TXEMPTY = 512u;

  void Uart_Init(UartConfigType* UartConfig);   

  Std_ReturnType Uart_SendBuffer(uint8_t Channel, uint8_t *Buffer, uint16_t Length);

  uint8_t Uart_GetByte(uint8_t Channel);

  uint32_t Uart_GetStatus(uint8_t Channel);



/*============================================================================*/
#endif /* _UART_H */