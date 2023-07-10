/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "stm32f1xx_it.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
	/* USER CODE BEGIN MemoryManagement_IRQn 0 */
		uint8_t cont = 0;
	  /* USER CODE END MemoryManagement_IRQn 0 */
	  while (cont == 0)
	  {
	    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
	    /* USER CODE END W1_MemoryManagement_IRQn 0 */
	  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */
	uint8_t cont = 0;
  /* USER CODE END MemoryManagement_IRQn 0 */
  while (cont == 0)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
extern volatile uint32_t ticks;
void SysTick_Handler(void)
{
	ticks++;

}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */
//EXTI
void EXTI0_IRQHandler(void){

}
void EXTI1_IRQHandler(void){


}
void EXTI2_IRQHandler(void){

}
void EXTI3_IRQHandler(void){

}
void EXTI4_IRQHandler(void){

}
//DMA
extern fptr DMA_fptr[7];
void DMA1_Channel1_IRQHandler(void){

	if(DMA->ISR & DMA_ISR_TCIF1){
		DMA_fptr[0]();
	}
	DMA->IFCR |= (DMA_IFCR_CTCIF1);
	NVIC_ClearPendingIRQ(DMA1_Channel1_IRQn);
}
void DMA1_Channel2_IRQHandler(void){
	if(DMA->ISR & DMA_ISR_TCIF2){
			DMA_fptr[1]();
		}
	DMA->IFCR |= (DMA_IFCR_CTCIF2);
		NVIC_ClearPendingIRQ(DMA1_Channel2_IRQn);
}
void DMA1_Channel3_IRQHandler(void){
	if(DMA->ISR & DMA_ISR_TCIF3){
			DMA_fptr[2]();
		}
	DMA->IFCR |= (DMA_IFCR_CTCIF3);
		NVIC_ClearPendingIRQ(DMA1_Channel3_IRQn);
}
void DMA1_Channel4_IRQHandler(void){
	if(DMA->ISR & DMA_ISR_TCIF4){
			DMA_fptr[3]();
		}
	DMA->IFCR |= (DMA_IFCR_CTCIF4);
		NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
}
void DMA1_Channel5_IRQHandler(void){
	if(DMA->ISR & DMA_ISR_TCIF5){
			DMA_fptr[4]();
		}
	DMA->IFCR |= (DMA_IFCR_CTCIF5);
		NVIC_ClearPendingIRQ(DMA1_Channel5_IRQn);
}
void DMA1_Channel6_IRQHandler(void){
	if(DMA->ISR & DMA_ISR_TCIF6){
			DMA_fptr[5]();
		}
	DMA->IFCR |= (DMA_IFCR_CTCIF6);
		NVIC_ClearPendingIRQ(DMA1_Channel6_IRQn);
}
void DMA1_Channel7_IRQHandler(void){
	if(DMA->ISR & DMA_ISR_TCIF7){
			DMA_fptr[6]();
		}
	DMA->IFCR |= (DMA_IFCR_CTCIF7);
		NVIC_ClearPendingIRQ(DMA1_Channel7_IRQn);
}

//CAN
/* USER CODE END 1 */
