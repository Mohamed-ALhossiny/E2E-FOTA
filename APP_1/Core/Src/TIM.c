/*
 * TIM.c
 *
 *  Created on: Jul 3, 2023
 *      Author: moham
 */
#include "TIM.h"

/*
 * Available Timers -> TIM1, TIM2, TIM3, TIM4
 */
void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Config) {
	TIMx->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
	TIMx->CR1 |= Config->CounterMode;
	TIMx->CR1 &= ~TIM_CR1_CKD;
	TIMx->CR1 |= (uint32_t) Config->ClockDivision;
	TIMx->CR1 &= TIM_CR1_ARPE;
	TIMx->CR1 |= Config->AutoReloadPreload;
	TIMx->ARR = (uint32_t) Config->Period;
	TIMx->PSC = Config->Prescaler;
	/* Set the Repetition Counter value */
	TIMx->RCR = Config->RepetitionCounter;

	TIMx->EGR = TIM_EGR_UG;
}

void TIM_ConfigClockSource(TIM_TypeDef *TIMx,
		TIM_ClockConfigTypeDef *sClockSourceConfig) {
	TIMx->SMCR &= ~(TIM_SMCR_SMS | TIM_SMCR_TS);
	TIMx->SMCR &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);

	switch (sClockSourceConfig->ClockSource) {
	case TIM_CLOCKSOURCE_INTERNAL: {
		break;
	}

	case TIM_CLOCKSOURCE_ETRMODE1: {

		/* Configure the ETR Clock source */

		/* Reset the ETR Bits */
		TIMx->SMCR &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE
				| TIM_SMCR_ETP);

		/* Set the Prescaler, the Filter value and the Polarity */
		TIMx->SMCR |= (uint32_t) (sClockSourceConfig->ClockPrescaler
				| (sClockSourceConfig->ClockPolarity
						| (sClockSourceConfig->ClockFilter << 8U)));
		TIMx->SMCR |= (TIM_SLAVEMODE_EXTERNAL1 | TIM_CLOCKSOURCE_ETRMODE1);
		break;
	}

	case TIM_CLOCKSOURCE_ETRMODE2: {
		/* Check whether or not the timer instance supports external trigger input mode 2 (ETRF)*/

		/* Configure the ETR Clock source */
		/* Configure the ETR Clock source */

		/* Reset the ETR Bits */
		TIMx->SMCR &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE
				| TIM_SMCR_ETP);

		/* Set the Prescaler, the Filter value and the Polarity */
		TIMx->SMCR |= (uint32_t) (sClockSourceConfig->ClockPrescaler
				| (sClockSourceConfig->ClockPolarity
						| (sClockSourceConfig->ClockFilter << 8U)));
		/* Enable the External clock mode2 */
		TIMx->SMCR |= TIM_SMCR_ECE;
		break;
	}

	case TIM_CLOCKSOURCE_TI1: {
		TIMx->CCER &= ~TIM_CCER_CC2E;
		TIMx->CCMR1 &= ~TIM_CCMR1_IC2F;
		TIMx->CCMR1 |= (sClockSourceConfig->ClockFilter << 12U);
		TIMx->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
		TIMx->CCER |= (sClockSourceConfig->ClockPolarity << 4U);

		TIMx->SMCR &= ~TIM_SMCR_TS;
		TIMx->SMCR |= (TIM_CLOCKSOURCE_TI1 | TIM_SLAVEMODE_EXTERNAL1);
		break;
	}

	case TIM_CLOCKSOURCE_TI2: {

		TIMx->CCER &= ~TIM_CCER_CC2E;
		TIMx->CCMR1 &= ~TIM_CCMR1_IC2F;
		TIMx->CCMR1 |= (sClockSourceConfig->ClockFilter << 12U);
		TIMx->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
		TIMx->CCER |= (sClockSourceConfig->ClockPolarity << 4U);

		TIMx->SMCR &= ~TIM_SMCR_TS;
		TIMx->SMCR |= (TIM_CLOCKSOURCE_TI2 | TIM_SLAVEMODE_EXTERNAL1);
		break;
	}

	case TIM_CLOCKSOURCE_TI1ED: {

		TIMx->CCER &= ~TIM_CCER_CC2E;
		TIMx->CCMR1 &= ~TIM_CCMR1_IC2F;
		TIMx->CCMR1 |= (sClockSourceConfig->ClockFilter << 12U);
		TIMx->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
		TIMx->CCER |= (sClockSourceConfig->ClockPolarity << 4U);
		TIMx->SMCR &= ~TIM_SMCR_TS;
		TIMx->SMCR |= (TIM_CLOCKSOURCE_TI1 | TIM_SLAVEMODE_EXTERNAL1);

		break;
	}

	case TIM_CLOCKSOURCE_ITR0:
	case TIM_CLOCKSOURCE_ITR1:
	case TIM_CLOCKSOURCE_ITR2:
	case TIM_CLOCKSOURCE_ITR3: {
		TIMx->SMCR &= ~TIM_SMCR_TS;
		TIMx->SMCR |= (sClockSourceConfig->ClockSource | TIM_SLAVEMODE_EXTERNAL1);
		break;
	}

	default:
		break;
	}
}

void TIM_MasterConfigSynchronization(TIM_TypeDef *TIMx, uint32_t masterOutputTrigger, uint32_t masterSlaveMode){
	TIMx->CR2 &= ~TIM_CR2_MMS;
	TIMx->CR2 |= masterOutputTrigger;
	TIMx->SMCR &= ~TIM_SMCR_MSM;
	TIMx->SMCR |= masterSlaveMode;

}

void TIM_Base_Start(TIM_TypeDef *TIMx){
	TIMx->CR1|=(TIM_CR1_CEN);

}

uint32_t TIM_GetCounter(TIM_TypeDef *TIMx){
	return (TIMx->CNT);
}
void TIM_SetCounter(TIM_TypeDef *TIMx, uint32_t value){
	TIMx->CNT = value;
}












