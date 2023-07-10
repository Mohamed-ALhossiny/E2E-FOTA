/*
 * RCC.c
 *
 *  Created on: Jan 29, 2023
 *      Author: moham
 */
#include "RCC.h"

void RCC_Config(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	//Enable Coprocessor CP10, CP11
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
	//Clear CR, CFGR reg
	RCC->CR = 0x01;
	RCC->CFGR = 0x0;
	//reset HSEON, CSSON, PLLON
	RCC->CFGR &= 0xFEF6FFFF;
	//RESET PLLCFGR
	RCC->PLLCFGR = 0x24003010;
	//reset HSEBYP
	RCC->CR &= 0xFFFBFFFF;
	//reset all interrupts
	RCC->CIR = 0x0;
#ifdef RCC_HSE_NO
	//enable HSE
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

#endif
	//HCLK /1
	RCC->CFGR |= RCC_AHB_PRESCALE;
	//PCLK2 /2
	RCC->CFGR |= RCC_APB2_PRESCALE;
	//PCLK1 / 4
	RCC->CFGR |= RCC_APB1_PRESCALE;
#if RCC_SYSCLK == RCC_SYSClk_PLL
	//config main PLL
		RCC->PLLCFGR = ((RCC_PLLCFGR_PLLSRC_HSE) | M | (N << 6) | (((P >> 1) - 1) << 16) | (Q << 24));
	//PLL ON
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY))
		;
#endif
	//config prefetch, inst cache, data cache, wait state
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN
			| FLASH_ACR_LATENCY_5WS;
#ifdef RCC_SYSCLK
	//config sysclck
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= RCC_SYSCLK;
	while (!(RCC->CFGR & RCC_SYSCLK_WAIT))
		;
#endif
}



 void RCC_EnablePeripheral(uint32_t peripheral) {
	if((peripheral >> 8) == 0x00){
		RCC->AHB1ENR |= (uint32_t)(1 << (peripheral & 0xFF));
	}else if ((peripheral >> 8) == 0x01) {
		RCC->APB1ENR |= (uint32_t)(1 << (peripheral & 0xFF));
	}else if ((peripheral >> 8) == 0x02) {
		RCC->APB2ENR |= (uint32_t)(1 << (peripheral & 0xFF));
	}else {
		//NOP
	}
}

 void RCC_DisablePeripheral(uint32_t peripheral){
	 if((peripheral >> 8) == 0x00){
	 		RCC->AHB1RSTR |= (uint32_t)(1 << (peripheral & 0xFF));
	 	}else if ((peripheral >> 8) == 0x01) {
	 		RCC->APB1RSTR |= (uint32_t)(1 << (peripheral & 0xFF));
	 	}else if ((peripheral >> 8) == 0x02) {
	 		RCC->APB2RSTR |= (uint32_t)(1 << (peripheral & 0xFF));
	 	}else {
	 		//NOP
	 	}
 }

uint8_t GetResetReason(void) {
	if (RCC->CSR & RCC_CSR_LPWRRSTF) {
		return RCC_LOWPOWERRESET;
	} else if (RCC->CSR & RCC_CSR_WWDGRSTF) {
		return RCC_WWDGRESET;
	} else if (RCC->CSR & RCC_CSR_IWDGRSTF) {
		return RCC_IWDGRESET;
	} else if (RCC->CSR & RCC_CSR_SFTRSTF) {
		return RCC_SOFTRESET;
	} else if (RCC->CSR & RCC_CSR_PORRSTF) {
		return RCC_PORRESET;
	}
	return RCC_PINRESET;
}
void ClearResetFlags(void) {
	RCC->CSR |= RCC_CSR_RMVF;
}

