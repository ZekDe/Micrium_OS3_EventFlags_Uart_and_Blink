#include "stm32f4xx.h"                  // Device header
#include "os.h"
#include "main.h"
#include "gpio.h"
#include "utility.h"
#include "usart.h"
#include "stdio.h"

#define	BTN_PRESSED   			0x01U
#define UART_PACKET_DONE   	0x02U

#define SIZE_RX	5

void SystemClock_Config(void);
void NVICs_init(void);

void task(void *p);

OS_TCB 	uartTaskTCB;
CPU_STK uartTaskSTK[128U];

OS_ERR osErr;

CPU_INT64U diffTimeF = 0;
CPU_INT64U diffTimeE = 0;
CPU_INT64U diffTimeRes = 0;

OS_FLAG_GRP  genFlag;


uint8_t rxBuf[SIZE_RX];
serial_t serial;

int main()
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	
	SystemClock_Config();
	SysTick_Config(SystemCoreClock / 1000);
	
	
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	
	// RX interrupt settings
	setUart(&serial, USART2);
	uartSetBuffer(&serial, rxBuf, SIZE_RX);
	
	printf("OS has started");
	OSInit(&osErr);

							
	OSTaskCreate(&uartTaskTCB,
								"Uart Task",
								task,
								(void*)0,
								1,
								uartTaskSTK,
								0,
								128U,
								0,
								0,
								0,
								OS_OPT_TASK_STK_CHK + OS_OPT_TASK_STK_CLR,
								&osErr
							 );

								
	OSFlagCreate(&genFlag,
							 "status",
							 (OS_FLAGS)0,
							 &osErr); 
	
	OSStart(&osErr);
}

void uart_rxItHandler(void)
{
	OS_ERR    err;
	uint32_t size;
	
	size = uartBytesAvailable(&serial);
	if(size == SIZE_RX)
	{
		(void)OSFlagPost(&genFlag, UART_PACKET_DONE, OS_OPT_POST_FLAG_SET, &err);
	}
}

void btnPressed(void)
{
	OS_ERR    err;
	(void)OSFlagPost(&genFlag, BTN_PRESSED, OS_OPT_POST_FLAG_SET, &err);
}

void task(void *p)
{
	  OS_ERR    err;
    OS_FLAGS  flags;
    CPU_TS    ts;
	
    while (DEF_ON) 
		{
      flags = OSFlagPend(&genFlag,
                         UART_PACKET_DONE | BTN_PRESSED,
                         0,//wait forever
                         OS_OPT_PEND_FLAG_SET_ANY,
                         &ts,
                         &err);
			
			if(flags & UART_PACKET_DONE)
			{	
				printf("%s", serial.rxBuff);
			
			  uartClear(&serial);
				(void)OSFlagPost(&genFlag, UART_PACKET_DONE, OS_OPT_POST_FLAG_CLR, &err);
				
			}
			
			if(flags & BTN_PRESSED)
			{
				LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_14);
				
				(void)OSFlagPost(&genFlag, BTN_PRESSED, OS_OPT_POST_FLAG_CLR, &err);
			}

		diffTimeF = CPU_TS32_to_uSec(CPU_TS_Get32());
		diffTimeE = CPU_TS32_to_uSec(CPU_TS_Get32());
	  diffTimeRes = diffTimeE - diffTimeF;
    }
		
		
}




void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(168000000);
  LL_SetSystemCoreClock(168000000);
}

void NVICs_init(void)
{
   
//   NVIC_ClearPendingIRQ(NonMaskableInt_IRQn);
//   NVIC_EnableIRQ(NonMaskableInt_IRQn);
//   
//   NVIC_ClearPendingIRQ(MemoryManagement_IRQn);
//   NVIC_EnableIRQ(MemoryManagement_IRQn);
//   
//   NVIC_ClearPendingIRQ(BusFault_IRQn);
//   NVIC_EnableIRQ(BusFault_IRQn);
//   
//   NVIC_ClearPendingIRQ(UsageFault_IRQn);
//   NVIC_EnableIRQ(UsageFault_IRQn);
//   
//   NVIC_ClearPendingIRQ(SVCall_IRQn);
//   NVIC_EnableIRQ(SVCall_IRQn);
//   
//   NVIC_ClearPendingIRQ(DebugMonitor_IRQn);
//   NVIC_EnableIRQ(DebugMonitor_IRQn);
//   
//   NVIC_ClearPendingIRQ(PendSV_IRQn);
//   NVIC_EnableIRQ(PendSV_IRQn);
	 
	 NVIC_ClearPendingIRQ(SysTick_IRQn);
   NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));
   NVIC_EnableIRQ(SysTick_IRQn);
}
