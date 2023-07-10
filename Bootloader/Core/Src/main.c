#include "UART.h"
#include "RCC.h"
#include "AFIO.h"
#include "GPIO.h"
#include "DMA.h"
#include "SCB.h"
#include "NVIC.h"
#include "CAN.h"
#include "CRC.h"
#include "SysTick.h"
#include "BOOTLOADER.h"

int main(void) {
	RCC_Config();
	//SCB->VTOR = 0x08005000 - 0x08000000;
	Enable_irq();
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	//SysTick_INIT();

	RCC_EnablePeripheral(RCC_AFIOCLK);
	RCC_EnablePeripheral(RCC_GPIOACLK);
	//RCC_EnablePeripheral(RCC_GPIOCCLK);
	RCC_EnablePeripheral(RCC_CANCLK);

	AFIO_INIT();
	//GPIO_Config_Pin(GPIOC, GPIO_PIN_13, GPIO_PINMODE_OUTPUT50MHZ);
	GPIO_Config_Pin(GPIOA, GPIO_PIN_11, GPIO_PINMODE_INPUT); //rx
	GPIO_Config_Pin(GPIOA, GPIO_PIN_12, GPIO_PINMODE_AFIO); //tx

	CAN_setupTypeDef CANsetupStruct = { DISABLE, DISABLE, DISABLE, DISABLE,
			DISABLE, DISABLE, };
	CAN_Setup(CAN, &CANsetupStruct);
	CAN_Normal(CAN);
	CAN_FilterTypeDef filter0;
	filter0.FilterActivation = CAN_FILTER_ENABLE;
	filter0.FilterBank = 1;
	filter0.FilterFIFOAssignment = CAN_FIFO0;
	filter0.FilterId = GATEWAY_BOOTLOADER_RQ_ID;
	filter0.FilterMaskId = GATEWAY_BOOTLOADER_RQ_ID;
	filter0.FilterMode = CAN_FILTERMODE_LIST;
	CAN_ConfigFilter(CAN, &filter0);
	CAN_Interrupt_Enable(CAN, CAN_IER_FMPIE0, ENABLE);
	BootLoader();













	while(1){
		/*GPIO_Write_Pin(GPIOC, GPIO_PIN_13, HIGH);
		SysTick_Delay(1000);
		GPIO_Write_Pin(GPIOC, GPIO_PIN_13, LOW);
		SysTick_Delay(1000);*/

	}


	}





