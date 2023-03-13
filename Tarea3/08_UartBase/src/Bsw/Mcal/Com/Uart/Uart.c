/*******************************************************************************/
/**
\file       
\brief      
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   \version       1.0
   \author        
   \description   
   \date          
*/

/** Scheduler function prototypes definitions */
#include "Uart.h"
#include "chip.h"
#include "MemAlloc.h"


/*****************************************************************************************************
* Defines - 
*****************************************************************************************************/

#define UART_CHANNEL_UNDEF (255)

#define UART_CH0  (0) 
#define UART_CH1  (1)
#define UART_CH2  (2)
#define UART_CH3  (3)
#define UART_CH4  (4)

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/

/* Uart Status Structure Example */
UartStatusType *UartStatus;
  

/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/
/* Array of Uart Register Base Address */
Uart * UartRegAddr[]={ UART0, UART1, UART2, UART3, UART4 };

/*****************************************************************************************************
* Code of module wide Private FUNCTIONS
*****************************************************************************************************/

// uint8_t Uart_GetLogChannel(uint8_t PhyChannel)
// {
//   uint8_t LogicalChannel = UART_CHANNEL_UNDEF; 
//   uint8_t LocChIdx = 0; /* LocChIdx represent the logical channel */
//   /* UART_CFG_CHANNELS represents the number of configured channels from configuration structure */
//   do
//   {
//     if (UartStatus[LocChIdx].ChannelId == PhyChannel)
//     {
//       LogicalChannel = LocChIdx; 
//     }
//     LocChIdx++;
//   }while( (UartStatus[LocChIdx-1].ChannelId != PhyChannel) && (LocChIdx < UART_CFG_CHANNELS) );
//   return (LogicalChannel);
// }

/*****************************************************************************************************
* Code of module wide Public FUNCTIONS
*****************************************************************************************************/

// void Uart_Init(  const uint8_t * ChannelConfigure )
// {
//   const Uart * LocUartReg;
//   uint8_t LocChIdx = 0; /* LocChIdx represent the logical channel */
   
//   /* Memory allocation for all Channel Status example */
//   /* UART_CFG_CHANNELS represents the number of configured channels from configuration structure */
//   UartStatus = (UartStatusType*) MemAlloc( sizeof(UartStatusType) * UART_CFG_CHANNELS  );  
  
//   for (LocChIdx = 0; LocChIdx < UART_CFG_CHANNELS; LocChIdx++)
//   {
//     /* Point to register address based of physical channel */
//     LocUartReg = UartRegAddr[ChannelConfigure[LocChIdx]];
//     /* Access to register for the configured channel with LocUartReg */
//     /* Access to channel status structure with LocChIdx */
//     UartStatus[LocChIdx].ChannelId = ChannelConfigure[LocChIdx];    
//   }
// }

void Uart_Init(UartConfigType* UartConfig) {
  uint8_t i = 0;
  UartConfigType* tmp;
  Std_ReturnType status;
  tmp = UartConfig;

  for(i = 0; i < UartConfig->UartNumberOfChannels; i++){
  }
}

Std_ReturnType Uart_SetMode(uint8_t Channel, uint8_t Mode){
  Uart * uart;
  uart = UartRegAddr[Channel];
  uint32_t UartMode;

  UartMode = uart->UART_MR;
  UartMode &=~ UART_MR_CHMODE_Msk;
  UartMode |= UART_MR_CHMODE(Mode);

  uart->UART_MR = UartMode;

  if((uart->UART_MR) == UartMode){
    return E_OK;
  } else {
    return E_NOT_OK;
  }

}

Std_ReturnType Uart_SetParity(uint8_t Channel, uint8_t Parity){
  Uart * uart;
  uart = UartRegAddr[Channel];
  uint32_t UartParity;
  Std_ReturnType status = E_NOT_OK;

  UartParity = uart->UART_MR;
  UartParity &=~ UART_MR_PAR_Msk;
  UartParity |= UART_MR_PAR(Parity);

  uart->UART_MR = UartParity;

  if((uart->UART_MR) == UartParity){
    status = E_OK;
  } else {
    status = E_NOT_OK;
  }

  return status;
}

Std_ReturnType Uart_SetBaudrate(uint8_t Channel, uint32_t Baudrate){
  Uart * uart;
  uart = UartRegAddr[Channel];
  uint32_t UartBaudrate;
  Std_ReturnType status;

  UartBaudrate = uart->UART_BRGR;
  UartBaudrate = Baudrate/16;

  uart->UART_BRGR = UartBaudrate;

  if((uart->UART_BRGR) == UartBaudrate){
    status = E_OK;
  } else {
    status = E_NOT_OK;
  }

  return status;
}

void Uart_SetTxEnable(uint8_t Channel, uint32_t Enable){
  Uart * uart;
  uart = UartRegAddr[Channel];
  uint32_t UartTransmit;
  Std_ReturnType status;

  UartTransmit = uart->UART_CR;
  UartTransmit &=~ UART_CR_TXDIS | UART_CR_TXEN | UART_CR_RSTTX;
  UartTransmit |= UART_CR_TXDIS | UART_CR_RSTTX;
  uart->UART_CR = UartTransmit;
  
  if(Enable == 1){
    UartTransmit &=~ UART_CR_TXDIS | UART_CR_TXEN | UART_CR_RSTTX;
    UartTransmit |= UART_CR_TXEN;
    uart->UART_CR = UartTransmit;
  }
}

void Uart_SetRxEnable(uint8_t Channel, uint32_t Enable){
  Uart * uart;
  uart = UartRegAddr[Channel];
  uint32_t UartReceive;
  Std_ReturnType status;

  UartReceive = uart->UART_CR;
  UartReceive &=~ UART_CR_RXDIS | UART_CR_RXEN | UART_CR_RSTRX;
  UartReceive |= UART_CR_RXDIS | UART_CR_RSTRX;
  uart->UART_CR = UartReceive;
  
  if(Enable == 1){
    UartReceive &=~ UART_CR_RXDIS | UART_CR_RXEN | UART_CR_RSTRX;
    UartReceive |= UART_CR_RXEN;
    uart->UART_CR = UartReceive;
  }
}

Std_ReturnType Uart_SendByte(uint8_t Channel, uint8_t Byte){
  Uart * uart;
  uart = UartRegAddr[Channel];
  uint8_t RecvChar;
  Std_ReturnType status = E_NOT_OK;

  /* Wait for the transmitter to be ready*/
	while (!(uart->UART_SR & UART_SR_TXRDY) && !(uart->UART_SR & UART_SR_TXEMPTY));

	/* Send character*/
	uart->UART_THR = Byte;

	/* Wait for the transfer to complete*/
	while (!(uart->UART_SR & UART_SR_TXEMPTY));

  if(uart->UART_SR & UART_SR_TXEMPTY){
    status = E_OK;
  } else {
    status = E_NOT_OK;
  }

  return status;
}

Std_ReturnType Uart_SendBuffer(uint8_t Channel, uint8_t *Buffer, uint16_t Length){
  uint16_t i = 0;
  uint32_t offset = 0;
  Std_ReturnType status = E_NOT_OK;

  for(i = 0; i < Length; i++){
      offset = sizeof(Buffer)*i;
      status = Uart_SendByte(Channel, (*(Buffer+offset)));
      if(status == E_NOT_OK){
        break;
      }
  }

  return status;
}

uint8_t Uart_GetByte(uint8_t Channel){
  const Uart * uart;
  uart = UartRegAddr[Channel];
  uint8_t RecvChar;

  while(!(uart->UART_SR & UART_SR_RXRDY)) {}
  RecvChar = uart->UART_RHR;

  return RecvChar;
}

uint32_t Uart_GetStatus(uint8_t Channel){
  const Uart * uart;
  uart = UartRegAddr[Channel];
  uint32_t UartStatus;

  UartStatus = uart->UART_SR;

  return UartStatus;
}

void Uart_EnableInt(uint8_t Channel, uint32_t IntMode, uint8_t Enable){

}

// void Uart_Send(uint8_t Channel)
// {
//   const Uart * LocUartReg = UartRegAddr[UartStatus[Channel].ChannelId];
//   /* Example Code */
//   UartStatus[Channel].Counter++;
// }

/*****************************************************************************************************
* Code of module wide Interrupt Handler FUNCTIONS
*****************************************************************************************************/

void Uart_Isr( uint8_t Channel )
{
  // const Uart * LocUartReg = UartRegAddr[Channel];
  // uint8_t LocUartLogicChannel = Uart_GetLogChannel(Channel);
  
  // /* Example Code */
  // /* UART_CFG_CHANNELS represents the number of configured channels from configuration structure */
  // if (LocUartLogicChannel<UART_CFG_CHANNELS)
  //   UartStatus[LocUartLogicChannel].TriggerCounter++;
	    
}


void UART0_Handler(void)
{
	Uart_Isr(UART_CH0);
}


void UART1_Handler(void)
{
	Uart_Isr(UART_CH1);
}


void UART2_Handler(void)
{
	Uart_Isr(UART_CH2);
}


void UART3_Handler(void)
{
	Uart_Isr(UART_CH3);
}


void UART4_Handler(void)
{
	Uart_Isr(UART_CH4);
}