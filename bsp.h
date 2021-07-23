#ifndef __BSP_H
#define __BSP_H

#include "stm32f4xx.h"                  // Device header
#include "os.h"                         // Micrium.Micrium::RTOS:uC/OS Kernel

__STATIC_INLINE void dwt_enable(void)
{
	//24.Bit TRCENA in Debug Exception and Monitor Control Register must be set before enable DWT
	CoreDebug->DEMCR 	|= (CPU_INT32U)CoreDebug_DEMCR_TRCENA_Msk;
	// CYCCNT is a free running counter, counting upwards.32 bits. 2^32 / cpuFreq = max time
	DWT->CYCCNT 			 = (CPU_INT32U)0u; // initialize the counter to 0
	DWT->CTRL 				|= (CPU_INT32U)DWT_CTRL_CYCCNTENA_Msk; // Enable the CYCCNT counter.
}

#endif
