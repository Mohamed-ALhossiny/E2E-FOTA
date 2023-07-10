/*
 * GPIO.h
 *
 *  Created on: Feb 3, 2023
 *      Author: moham
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_
#include "STD_TYPE.h"



typedef struct{
	uint32_t Mode;
	uint32_t Speed;
	uint32_t PullupPullDown;
}GPIO_Config;

/**
  * @brief General Purpose I/O
  */

typedef struct
{
  volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  volatile uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  volatile uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  volatile uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  volatile uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  volatile uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
  volatile uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  volatile uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;


#define GPIOA_BASE            (0x40000000UL + 0x00020000UL + 0x0000UL)
#define GPIOB_BASE            (0x40000000UL + 0x00020000UL + 0x0400UL)
#define GPIOC_BASE            (0x40000000UL + 0x00020000UL + 0x0800UL)
#define GPIOD_BASE            (0x40000000UL + 0x00020000UL + 0x0C00UL)
#define GPIOE_BASE            (0x40000000UL + 0x00020000UL + 0x1000UL)
#define GPIOF_BASE            (0x40000000UL + 0x00020000UL + 0x1400UL)
#define GPIOG_BASE            (0x40000000UL + 0x00020000UL + 0x1800UL)
#define GPIOH_BASE            (0x40000000UL + 0x00020000UL + 0x1C00UL)

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)




#define HIGH										0x1
#define LOW											0x0
//Port Configurations (Input by default)
#define GPIO_PORTMODE_OUTPUT				    0x55555555
#define GPIO_PORTMODE_INPUT			              0x00
#define GPIO_PORTSPEED_LOW				  		  0x00
#define GPIO_PORTSPEED_MEDIUM					0x55555555
#define GPIO_PORTSPEED_FAST						0xAAAAAAAA
#define GPIO_PORTSPEED_HIGH						0xFFFFFFFF
#define GPIO_PORTPULL_UP_DOWN					  0x00
#define GPIO_PORTPULL_UP						0x55555555
#define GPIO_PORTPULL_DOWN						0xAAAAAAAA



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

//Pin Configurations
#define GPIO_PINMODE_OUTPUT							0x01
#define GPIO_PINMODE_INPUT							0x00
#define GPIO_PINMODE_AF0							0x02
#define GPIO_PINMODE_AF1							0x12
#define GPIO_PINMODE_AF2							0x22
#define GPIO_PINMODE_AF3							0x32
#define GPIO_PINMODE_AF4							0x42
#define GPIO_PINMODE_AF5							0x52
#define GPIO_PINMODE_AF6							0x62
#define GPIO_PINMODE_AF7							0x72
#define GPIO_PINMODE_AF8							0x82
#define GPIO_PINMODE_AF9							0x92
#define GPIO_PINMODE_AF10							0xA2
#define GPIO_PINMODE_AF11							0xB2
#define GPIO_PINMODE_AF12							0xC2
#define GPIO_PINMODE_AF13							0xD2
#define GPIO_PINMODE_AF14							0xE2
#define GPIO_PINMODE_AF15							0xF2

#define GPIO_PINSPEED_LOW							0x00
#define GPIO_PINSPEED_MEDIUM						0x01
#define GPIO_PINSPEED_FAST							0x02
#define GPIO_PINSPEED_HIGH							0x03
#define GPIO_PINPULL_UP_DOWN						0x00
#define GPIO_PINPULL_UP								0x01
#define GPIO_PINPULL_DOWN							0x02













void GPIO_Config_Port(GPIO_TypeDef *GPIOx, GPIO_Config Config);
uint16_t GPIO_Read_Port(GPIO_TypeDef *GPIOx);
void GPIO_Write_Port(GPIO_TypeDef *GPIOx,uint16_t Port_Value);
void GPIO_Config_Pin(GPIO_TypeDef *GPIOx, uint8_t pinNumber, GPIO_Config pinConfig);
uint8_t GPIO_Read_Pin(GPIO_TypeDef *GPIOx, uint8_t Pin_Number);
void GPIO_Write_Pin(GPIO_TypeDef *GPIOx, uint8_t Pin_Number, uint8_t Value);

#endif /* INC_GPIO_H_ */
