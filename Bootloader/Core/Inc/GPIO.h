/*
 * GPIO.h
 *
 *  Created on: Feb 3, 2023
 *      Author: moham
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_
#include "STD_TYPE.h"


typedef struct
{
  volatile uint32_t CRL;
  volatile uint32_t CRH;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t BRR;
  volatile uint32_t LCKR;
} GPIO_TypeDef;


#define GPIOA_BASE            (0x40000000UL + 0x00010000UL + 0x00000800UL)
#define GPIOB_BASE            (0x40000000UL + 0x00010000UL + 0x00000C00UL)
#define GPIOC_BASE            (0x40000000UL + 0x00010000UL + 0x00001000UL)
#define GPIOD_BASE            (0x40000000UL + 0x00010000UL + 0x00001400UL)
#define GPIOE_BASE            (0x40000000UL + 0x00010000UL + 0x00001800UL)

#define GPIOA               ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *)GPIOE_BASE)




#define HIGH										0x1
#define LOW											0x0
//Port Modes (Floating (High impedance) by default)
#define GPIO_PORTMODE_OUTPUT2MHZ				0x22222222
#define GPIO_PORTMODE_OUTPUT10MHZ				0x11111111
#define GPIO_PORTMODE_OUTPUT50MHZ				0x33333333
#define GPIO_PORTMODE_INPUT						0x88888888

//Pin Numbers
#define GPIO_PIN_0									 0
#define GPIO_PIN_1									 1
#define GPIO_PIN_2									 2
#define GPIO_PIN_3									 3
#define GPIO_PIN_4									 4
#define GPIO_PIN_5									 5
#define GPIO_PIN_6									 6
#define GPIO_PIN_7									 7
#define GPIO_PIN_8									 8
#define GPIO_PIN_9									 9
#define GPIO_PIN_10									 10
#define GPIO_PIN_11									 11
#define GPIO_PIN_12									 12
#define GPIO_PIN_13									 13
#define GPIO_PIN_14									 14
#define GPIO_PIN_15									 15

//Pin Modes
#define GPIO_PINMODE_OUTPUT2MHZ						0x2
#define GPIO_PINTMODE_OUTPUT10MHZ					0x1
#define GPIO_PINMODE_OUTPUT50MHZ					0x3
#define GPIO_PINMODE_AFIO							0xB
#define GPIO_PINMODE_INPUT							0x8

void GPIO_Config_Port(GPIO_TypeDef *GPIOx, uint32_t GPIO_Mode);
uint16_t GPIO_Read_Port(GPIO_TypeDef *GPIOx);
void GPIO_Write_Port(GPIO_TypeDef *GPIOx,uint16_t Port_Value);
void GPIO_Config_Pin(GPIO_TypeDef *GPIOx, uint8_t Pin_Number, uint8_t Pin_Mode);
uint8_t GPIO_Read_Pin(GPIO_TypeDef *GPIOx, uint8_t Pin_Number);
void GPIO_Write_Pin(GPIO_TypeDef *GPIOx, uint8_t Pin_Number, uint8_t Value);

#endif /* INC_GPIO_H_ */
