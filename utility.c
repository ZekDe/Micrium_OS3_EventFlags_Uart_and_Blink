#include "utility.h"

extern volatile uint32_t systick;

void delay_ms(uint32_t ms)
{
	uint32_t start = systick;
	while ((systick - start) < ms)
		;
}
