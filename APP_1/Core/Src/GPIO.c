/*
 * GPIO.c
 *
 *  Created on: Feb 3, 2023
 *      Author: moham
 */
#include "GPIO.h"

void GPIO_Config_Port(GPIO_TypeDef *GPIOx, uint32_t GPIO_Mode){
	GPIOx->CRL &= 0x0;
	GPIOx->CRH &= 0x0;
	GPIOx->CRL |= GPIO_Mode;
	GPIOx->CRH |= GPIO_Mode;
}
uint16_t GPIO_Read_Port(GPIO_TypeDef *GPIOx){
	return (GPIOx->IDR);
}
void GPIO_Write_Port(GPIO_TypeDef *GPIOx,uint16_t Port_Value){
	GPIOx->ODR = Port_Value;
}

void GPIO_Config_Pin(GPIO_TypeDef *GPIOx, uint8_t Pin_Number, uint8_t Pin_Mode){
	if(Pin_Number <= GPIO_PIN_7){
		GPIOx->CRL &= ~(0xC << (Pin_Number*4)); //Reset CNF bits
		GPIOx->CRL |= (Pin_Mode << (Pin_Number*4));
	}else if (Pin_Number <= GPIO_PIN_15) {
		Pin_Number -= 8; //remove bits in CRL
		GPIOx->CRH &= ~(0xC << (Pin_Number*4));
		GPIOx->CRH |= (Pin_Mode << (Pin_Number*4));
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








