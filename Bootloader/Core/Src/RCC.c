/*
 * RCC.c
 *
 *  Created on: Jan 29, 2023
 *      Author: moham
 */
#include "RCC.h"
#include "RCC_Config.h"


void RCC_Config(void){
	//Enable Flash Prefetch
	FLASH->ACR |= FLASH_ACR_PRFTBE;
	//Clock security system Disable
	RCC->CR &= ~(RCC_CR_CSSON_Msk);
	//Clear All bits
	RCC->CR &= 0x0;
	RCC->CFGR &= 0x0;
	//reset all interrupts
	RCC->CIR = 0x0;
	//enable HSI (ensure)
	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY));

#ifdef RCC_HSE_NO
	//enable HSE
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	RCC->CFGR |= RCC_CFGR_PLLSRC;
#endif
//	//Power interface clock enable
//	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//



#ifdef RCC_AHB_PRESCALE
	RCC->CFGR |= RCC_AHB_PRESCALE;
#endif
#ifdef RCC_APB1_PRESCALE
	RCC->CFGR |= RCC_APB1_PRESCALE;
#endif
#ifdef RCC_APB2_PRESCALE
	RCC->CFGR |= RCC_APB2_PRESCALE;
#endif

#ifdef RCC_SYSCLK
	//Configure Flash Letancy (2 Wait states)
	FLASH->ACR |= FLASH_ACR_LATENCY_1;
	RCC->CFGR |= RCC_PLL_MUL;
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));
	//Sysclk Selection
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
#endif
}

void RCC_EnablePeripheral(uint8_t peripheralNumber){
	if(peripheralNumber <= APB2_END){
		RCC->APB2ENR |= (1 << peripheralNumber);
	}else if((peripheralNumber >= APB1_START) && (peripheralNumber <= APB1_END)){
		peripheralNumber -= APB1_START;
		RCC->APB1ENR |= (1 << peripheralNumber);
	}else if((peripheralNumber >= AHB_START) && (peripheralNumber <= AHB_END)){
		peripheralNumber -= AHB_START;
		RCC->AHBENR |= (1 << peripheralNumber);
	}else{
		//Do nothing
	}
}

void RCC_DisablePeripheral(uint8_t peripheralNumber){
	if(peripheralNumber <= APB2_END){
		RCC->APB2ENR &= ~((1 << peripheralNumber));
	}else if((peripheralNumber >= APB1_START) && (peripheralNumber <= APB1_END)){
		peripheralNumber -= APB1_START;
		RCC->APB1ENR &= ~((1 << peripheralNumber));
	}else if((peripheralNumber >= AHB_START) && (peripheralNumber <= AHB_END)){
		peripheralNumber -= AHB_START;
		RCC->AHBENR &= ~((1 << peripheralNumber));
	}else{
		//Do nothing
	}
}

uint8_t GetResetReason(void){
	if(RCC->CSR & RCC_CSR_LPWRRSTF){
		return RCC_LOWPOWERRESET;
	}else if(RCC->CSR & RCC_CSR_WWDGRSTF){
		return RCC_WWDGRESET;
	}else if(RCC->CSR & RCC_CSR_IWDGRSTF){
		return RCC_IWDGRESET;
	}else if(RCC->CSR & RCC_CSR_SFTRSTF){
		return RCC_SOFTRESET;
	}else if(RCC->CSR & RCC_CSR_PORRSTF){
		return RCC_PORRESET;
	}
	return RCC_PINRESET;
}
void ClearResetFlags(void){
	RCC->CSR |= RCC_CSR_RMVF;
}
