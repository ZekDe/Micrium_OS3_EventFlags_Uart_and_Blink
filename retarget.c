
#include <stdint.h>
#include <stdio.h>
#include "main.h"


struct __FILE 
{
	int handle;
};
FILE __stdin = {0};
FILE __stdout ={1};
FILE __stderr ={2};

int fgetc(FILE *f)
{
	while (!LL_USART_IsActiveFlag_RXNE(USART2)
			&& !LL_USART_IsActiveFlag_ORE(USART2))
		;

	if (LL_USART_IsActiveFlag_ORE(USART2))
	{
		LL_USART_ClearFlag_ORE(USART2);
		return -1;
	}

	return LL_USART_ReceiveData8(USART2);
}

int fputc(int c,FILE *f)
{
		LL_USART_TransmitData8(USART2, c);
	while (!LL_USART_IsActiveFlag_TC(USART2))
		;
	LL_USART_ClearFlag_TC(USART2);
	return c;
}

