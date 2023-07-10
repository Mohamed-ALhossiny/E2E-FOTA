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

#define RCC_HSE_NO				RCC_CR_HSEON

//defines bus prescalers
#define RCC_AHB_PRESCALE		RCC_AHB_PRESCALEBY1
#define RCC_APB1_PRESCALE		RCC_APB1_RESCALEBY2
#define RCC_APB2_PRESCALE		RCC_APB2_RESCALEBY1

//PLL Configurations
//PLL Input (HSI by default)
#define RCC_PLL_INPUT			RCC_PLL_INPUT_HSE
#define RCC_PLL_MUL				RCC_PLL_MULNY9

#endif /* INC_RCC_CONFIG_H_ */
