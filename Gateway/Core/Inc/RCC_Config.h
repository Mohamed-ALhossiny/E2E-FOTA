/*
 * RCC_Config.h
 *
 *  Created on: Feb 1, 2023
 *      Author: moham
 */

#ifndef INC_RCC_CONFIG_H_
#define INC_RCC_CONFIG_H_


//defines System clock source (if it is not defined ,it will be HSI) ,
//Values RCC_SYSClk_HSE, RCC_SYSClk_PLL
#define RCC_SYSCLK 				RCC_SYSClk_PLL
#define RCC_SYSCLK_WAIT			RCC_CFGR_SWS_PLL

#define RCC_HSE_NO				RCC_CR_HSEON

//defines bus prescalers
#define RCC_AHB_PRESCALE		RCC_AHB_PRESCALEBY1
#define RCC_APB1_PRESCALE		RCC_APB1_RESCALEBY4
#define RCC_APB2_PRESCALE		RCC_APB2_RESCALEBY1

//PLL Configurations
//PLL Input (HSI by default)
#define RCC_PLL_INPUT			RCC_PLL_INPUT_HSE
//PLL Factors (at 180 MHz)
#define M 						4
#define N 						180
#define P 						2
#define Q 						7

#endif /* INC_RCC_CONFIG_H_ */
