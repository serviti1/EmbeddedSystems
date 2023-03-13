/*******************************************************************************/
/**
\file       Uart_Cfg.h
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/
#ifndef _UART_CFG_H
#define _UART_CFG_H

  #include "Uart_Types.h"

  #define UART_CFG_CHANNEL0  (0)
  #define UART_CFG_CHANNEL1  (1)
  #define UART_CFG_CHANNEL2  (2)
  #define UART_CFG_CHANNEL3  (3)
  #define UART_CFG_CHANNEL4  (4)

  #define UART_CFG_INT_DISABLED     (0)
  #define UART_CFG_INT_RXRDY        (1)
  #define UART_CFG_INT_TXRDY        (2)
  #define UART_CFG_INT_OVR_ERROR    (32)
  #define UART_CFG_INT_FRAME_ERROR  (64) 
  #define UART_CFG_INT_PAR_ERROR    (128)
  #define UART_CFG_INT_TXEMPTY      (512)

  #define UART_CFG_MODE_NORMAL          (0)
  #define UART_CFG_MODE_AUTO            (1)
  #define UART_CFG_MODE_LOCAL_LOOPBACK  (2)
  #define UART_CFG_MODE_REMOTE_LOOPBACK (3)

  #define UART_CFG_PARITY_EVEN    (0)
  #define UART_CFG_PARITY_ODD     (1)
  #define UART_CFG_PARITY_SPACE   (2)
  #define UART_CFG_PARITY_MARK    (3)
  #define UART_CFG_PARITY_NO      (4)

  #define UART_CFG_PER_CLK        (0)
  #define UART_CFG_PMC_CLK        (1)

  typedef struct{
    uint8_t ChannelId;
    uint8_t IsrEn;
    uint8_t Mode;
    uint8_t Parity;
    uint32_t Baudrate;
    UartNotification TxNotification;
    UartNotification RxNotification;
    UartErrorNotification ErrorNotification;
  } UartChannelType;

  typedef struct{
    uint8_t UartNumberOfChannels;
    uint8_t ClkSrc;
    UartChannelType *UartChannel;
  }UartConfigType;



#endif /* _UART_CFG_H */