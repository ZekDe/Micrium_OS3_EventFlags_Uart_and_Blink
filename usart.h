#ifndef __USART_H
#define __USART_H

#include "main.h"
 
void MX_USART2_UART_Init(void);

typedef struct
{
	uint32_t byteCounter;
	USART_TypeDef* UARTx;
	uint32_t sizeOfBuff;
	uint8_t* rxBuff;
}serial_t;


void setUart(serial_t* serial, USART_TypeDef* UARTx);
void uartSetBuffer(serial_t* serial, uint8_t* rxBuffer, uint32_t size);
uint32_t uartBytesAvailable(serial_t* serial);
void uartClear(serial_t* serial);
	
#endif
