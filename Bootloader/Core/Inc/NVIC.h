/*
 * NVIC.h
 *
 *  Created on: Feb 5, 2023
 *      Author: moham
 */

#ifndef INC_NVIC_H_
#define INC_NVIC_H_
#include "STD_TYPE.h"

typedef struct
{
  volatile uint32_t ISER[8U];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
        uint32_t RESERVED0[24U];
  volatile uint32_t ICER[8U];               /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
        uint32_t RSERVED1[24U];
  volatile uint32_t ISPR[8U];               /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
        uint32_t RESERVED2[24U];
  volatile uint32_t ICPR[8U];               /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
        uint32_t RESERVED3[24U];
  volatile uint32_t IABR[8U];               /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
        uint32_t RESERVED4[56U];
  volatile uint8_t  IP[240U];               /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
        uint32_t RESERVED5[644U];
  volatile  uint32_t STIR;                   /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
}  NVIC_Type;





#define NVIC_BASE           		(0xE000E000UL +  0x0100UL)
#define NVIC               	 ((NVIC_Type      *)     NVIC_BASE     )   /*!< NVIC configuration struct */

#define NVIC_PRIO_BITS          				 4U       /*!< STM32 uses 4 Bits for the Priority Levels    */




/*!< Interrupt Number Definition */
typedef enum {
	/******  STM32 specific Interrupt Numbers *********************************************************/
	WWDG_IRQn = 0, /*!< Window WatchDog Interrupt                            */
	PVD_IRQn = 1, /*!< PVD through EXTI Line detection Interrupt            */
	TAMPER_IRQn = 2, /*!< Tamper Interrupt                                     */
	RTC_IRQn = 3, /*!< RTC global Interrupt                                 */
	FLASH_IRQn = 4, /*!< FLASH global Interrupt                               */
	RCC_IRQn = 5, /*!< RCC global Interrupt                                 */
	EXTI0_IRQn = 6, /*!< EXTI Line0 Interrupt                                 */
	EXTI1_IRQn = 7, /*!< EXTI Line1 Interrupt                                 */
	EXTI2_IRQn = 8, /*!< EXTI Line2 Interrupt                                 */
	EXTI3_IRQn = 9, /*!< EXTI Line3 Interrupt                                 */
	EXTI4_IRQn = 10, /*!< EXTI Line4 Interrupt                                 */
	DMA1_Channel1_IRQn = 11, /*!< DMA1 Channel 1 global Interrupt                      */
	DMA1_Channel2_IRQn = 12, /*!< DMA1 Channel 2 global Interrupt                      */
	DMA1_Channel3_IRQn = 13, /*!< DMA1 Channel 3 global Interrupt                      */
	DMA1_Channel4_IRQn = 14, /*!< DMA1 Channel 4 global Interrupt                      */
	DMA1_Channel5_IRQn = 15, /*!< DMA1 Channel 5 global Interrupt                      */
	DMA1_Channel6_IRQn = 16, /*!< DMA1 Channel 6 global Interrupt                      */
	DMA1_Channel7_IRQn = 17, /*!< DMA1 Channel 7 global Interrupt                      */
	ADC1_2_IRQn = 18, /*!< ADC1 and ADC2 global Interrupt                       */
	USB_HP_CAN1_TX_IRQn = 19, /*!< USB Device High Priority or CAN1 TX Interrupts       */
	USB_LP_CAN1_RX0_IRQn = 20, /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
	CAN1_RX1_IRQn = 21, /*!< CAN1 RX1 Interrupt                                   */
	CAN1_SCE_IRQn = 22, /*!< CAN1 SCE Interrupt                                   */
	EXTI9_5_IRQn = 23, /*!< External Line[9:5] Interrupts                        */
	TIM1_BRK_IRQn = 24, /*!< TIM1 Break Interrupt                                 */
	TIM1_UP_IRQn = 25, /*!< TIM1 Update Interrupt                                */
	TIM1_TRG_COM_IRQn = 26, /*!< TIM1 Trigger and Commutation Interrupt               */
	TIM1_CC_IRQn = 27, /*!< TIM1 Capture Compare Interrupt                       */
	TIM2_IRQn = 28, /*!< TIM2 global Interrupt                                */
	TIM3_IRQn = 29, /*!< TIM3 global Interrupt                                */
	TIM4_IRQn = 30, /*!< TIM4 global Interrupt                                */
	I2C1_EV_IRQn = 31, /*!< I2C1 Event Interrupt                                 */
	I2C1_ER_IRQn = 32, /*!< I2C1 Error Interrupt                                 */
	I2C2_EV_IRQn = 33, /*!< I2C2 Event Interrupt                                 */
	I2C2_ER_IRQn = 34, /*!< I2C2 Error Interrupt                                 */
	SPI1_IRQn = 35, /*!< SPI1 global Interrupt                                */
	SPI2_IRQn = 36, /*!< SPI2 global Interrupt                                */
	USART1_IRQn = 37, /*!< USART1 global Interrupt                              */
	USART2_IRQn = 38, /*!< USART2 global Interrupt                              */
	USART3_IRQn = 39, /*!< USART3 global Interrupt                              */
	EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts                      */
	RTC_Alarm_IRQn = 41, /*!< RTC Alarm through EXTI Line Interrupt                */
	USBWakeUp_IRQn = 42, /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
} NVIC_IRQn_Type;



void Enable_irq(void);
void Disable_irq(void);
void NVIC_EnableIRQ(NVIC_IRQn_Type IRQn);
uint32_t NVIC_GetEnableIRQ(NVIC_IRQn_Type IRQn);
void NVIC_DisableIRQ(NVIC_IRQn_Type IRQn);
uint32_t NVIC_GetPendingIRQ(NVIC_IRQn_Type IRQn);
void NVIC_SetPendingIRQ(NVIC_IRQn_Type IRQn);
void NVIC_ClearPendingIRQ(NVIC_IRQn_Type IRQn);
uint32_t NVIC_GetActive(NVIC_IRQn_Type IRQn);
void NVIC_SetPriority(NVIC_IRQn_Type IRQn, uint32_t priority);
uint32_t NVIC_GetPriority(NVIC_IRQn_Type IRQn);
uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority);
void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* const pPreemptPriority, uint32_t* const pSubPriority);









#endif /* INC_NVIC_H_ */
