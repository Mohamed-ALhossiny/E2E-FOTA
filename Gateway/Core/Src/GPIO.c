/*
 * GPIO.c
 *
 *  Created on: Feb 3, 2023
 *      Author: moham
 */
#include "GPIO.h"

void GPIO_Config_Port(GPIO_TypeDef *GPIOx, GPIO_Config Config){
	GPIOx->MODER  = Config.Mode;
	GPIOx->OSPEEDR = Config.Speed;
	GPIOx->PUPDR = Config.PullupPullDown;
}
uint16_t GPIO_Read_Port(GPIO_TypeDef *GPIOx){
	return (GPIOx->IDR);
}
void GPIO_Write_Port(GPIO_TypeDef *GPIOx,uint16_t Port_Value){
	GPIOx->ODR = Port_Value;
}

void GPIO_Config_Pin(GPIO_TypeDef *GPIOx, uint8_t pinNumber, GPIO_Config pinConfig){
	GPIOx->MODER |= (uint32_t)((pinConfig.Mode & 0x0F) << (pinNumber*2));
	GPIOx->OSPEEDR |= (uint32_t)(pinConfig.Speed << (pinNumber*2));
	GPIOx->PUPDR |= (uint32_t)(pinConfig.PullupPullDown << (pinNumber*2));
	if((pinConfig.Mode & 0x0F) == 0x02){
		if(pinNumber <= GPIO_PIN_7){
			GPIOx->AFR[0] |= (uint32_t)((pinConfig.Mode >> 4) << pinNumber*4);
		}else if (pinNumber <= GPIO_PIN_15) {
			GPIOx->AFR[1] |= (uint32_t)((pinConfig.Mode >> 4) << (pinNumber - 8)*4);
		}else {
			//NOP
		}
	}

}

uint8_t GPIO_Read_Pin(GPIO_TypeDef *GPIOx, uint8_t Pin_Number){
	return (((GPIOx->IDR) >> Pin_Number) & 0x1);
}
void GPIO_Write_Pin(GPIO_TypeDef *GPIOx, uint8_t Pin_Number, uint8_t Value){
	if(Value == HIGH){
		GPIOx->BSRR |= (0x1 << Pin_Number);
	}else if(Value == LOW){
		GPIOx->BSRR |= (0x1 << (Pin_Number + 16));
	}
}








