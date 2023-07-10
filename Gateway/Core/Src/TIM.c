/*
 * TIM.c
 *
 *  Created on: Apr 23, 2023
 *      Author: moham
 */
#include "TIM.h"

void TIM_INIT(TIM_TypeDef* TIMx) {
	/* Initialize TIMx peripheral as follow:
	 + Period = [(TIM2CLK/1000) - 1]. to have a (1/1000) s time base.
	 + Prescaler = (APB1CLK/1000000 - 1) to have a 1MHz counter clock.
	 + ClockDivision = 0
	 + Counter direction = Up
	 */
	uint32_t Period = (1000000U / 1000U) - 1U;
	uint32_t Prescaler = (uint32_t) ((90000000 / 1000000U) - 1U);

	TIMx->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
	TIMx->CR1 &= ~TIM_CR1_CKD;
	TIMx->CR1 &= ~TIM_CR1_ARPE;
	TIMx->ARR = (uint32_t)Period ;
	TIMx->PSC = Prescaler;
	TIMx->EGR = TIM_EGR_UG;
	TIMx->DIER |= TIM_DIER_UIE;

	TIMx->CR1 |= TIM_CR1_CEN;

}

volatile uint32_t TIM2_Ticks = 0;
void TIM_Delay(uint32_t time){
	uint32_t startTick = TIM2_Ticks;
	time++ ;
	while(TIM2_Ticks - startTick < time);
}


void TIM2_IRQHandler(void) {

	TIM2_Ticks++;
	TIM2->SR = ~TIM_DIER_UIE;
}


