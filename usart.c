#include "usart.h"
#include <stdio.h>

void MX_USART2_UART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART2_IRQn);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);
	
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
//	LL_USART_EnableIT_TC(USART1);
//	LL_USART_EnableIT_ERROR(USART1);
	LL_USART_EnableIT_RXNE(USART2);

	while (!LL_USART_IsActiveFlag_TC(USART2))
		;
	LL_USART_ClearFlag_TC(USART2);

}

void setUart(serial_t* serial, USART_TypeDef* UARTx)
{
	serial->UARTx = UARTx;
}

void uartSetBuffer(serial_t* serial, uint8_t* rxBuffer, uint32_t size)
{
	serial->rxBuff = rxBuffer;
	serial->sizeOfBuff = size;
}

uint32_t uartBytesAvailable(serial_t* serial)
{
	if(serial->byteCounter > serial->sizeOfBuff)
		serial->byteCounter = serial->sizeOfBuff;
	serial->rxBuff[serial->byteCounter++] = LL_USART_ReceiveData8(serial->UARTx);
	return serial->byteCounter;
}

void uartClear(serial_t* serial)
{
	serial->byteCounter = 0;
	for(uint32_t i = 0; i < serial->sizeOfBuff; i++)
	serial->rxBuff[i] = 0;
}
