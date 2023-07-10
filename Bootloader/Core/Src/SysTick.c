/*
 * SysTick.c
 *
 *  Created on: Feb 6, 2023
 *      Author: moham
 */
#include "SysTick.h"

/**
  \brief   System Tick Configuration
  \details Initializes the System Timer and its interrupt, and starts the System Tick Timer.
           Counter is in free running mode to generate periodic interrupts.
  \param [in]  ticks  Number of ticks between two interrupts.
  \return          0  Function succeeded.
  \return          1  Function failed.
  \note    When the variable <b>__Vendor_SysTickConfig</b> is set to 1, then the
           function <b>SysTick_Config</b> is not included. In this case, the file <b><i>device</i>.h</b>
           must contain a vendor-specific implementation of this function.
 */

volatile uint32_t ticks;
void SysTick_INIT(void)
{

  SysTick->LOAD  = (uint32_t)(SYSTEMCLOCK/1000 - 1UL);                         /* set reload register */
  SCB_SetPriority (SysTick_IRQn, (1UL << SCB_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */                                                    /* Function successful */
}

void SysTick_Delay(uint32_t delay_ms){
	uint32_t tickstart = ticks;
	uint32_t wait = delay_ms;

	/* Add a freq to guarantee minimum wait */
	wait += 1;

	while ((ticks - tickstart) < wait) {
	}
}

void SysTick_SuspendTick(void)
{
  /* Disable SysTick Interrupt */
  SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);
}
