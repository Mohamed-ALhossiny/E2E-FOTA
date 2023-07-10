#include "main.h"
#include "SCB.h"
#include "TIM.h"
#include "Application.h"
/*
//#include "CAN.h"
//#include "RCC.h"
//#include "GPIO.h"
//#include "NVIC.h"
//#include "DMA.h"
//#include "GSM.h"
//#include "RTOS.h"
*/



/*--------------------------------------------------------------------*/
/*---------------------------TASKS------------------------------------*/
//Update_TSK
extern OS_STACKPTR uint8_t StackUpdate_TSK[5120]; //Task Stack
extern  OS_TASK CB_Update_TSK; //Control Block

//Authentication_TSK
extern  OS_STACKPTR uint8_t StackAuthentication_TSK[1024]; //Task Stack
extern  OS_TASK CB_Authentication_TSK; //Control Block

//SendDownloadProgress_TSK
extern OS_STACKPTR uint8_t StackSendDownloadProgress_TSK[1024]; //Task Stack
extern OS_TASK CB_SendDownloadProgress_TSK; //Control Block

//Diagnostic_TSK
extern OS_STACKPTR uint8_t StackDiagnostic_TSK[1024]; //Task Stack
extern OS_TASK CB_Diagnostic_TSK; //Control Block


//Test_TSK
extern  OS_STACKPTR uint8_t StackTest_TSK[512]; //Task Stack
extern  OS_TASK CB_Test_TSK; //Control Block


/*---------------------------MUTEX------------------------------------*/
extern OS_MUTEX CAN_RxMsg_Mutex;
extern OS_MUTEX AuthToken_Mutex;



/*************************** End of file ****************************/



int main() {
	RCC_Setup();
	Interrupt_Setup();
	TIM_INIT(TIM2);
	GPIO_Setup();
	UART_Setup();
	GSM_INIT();
	CAN_Setup();


	OS_Init();
	OS_InitHW();
	OS_TASK_CREATE(&CB_Update_TSK, "Update Task", 50, Update_TSK,
			StackUpdate_TSK);
	OS_TASK_CREATE(&CB_SendDownloadProgress_TSK, "SendDownloadProgress Task", 60, SendDownloadProgress_TSK, StackSendDownloadProgress_TSK);

	OS_TASK_CREATE(&CB_Authentication_TSK, "Authentication Task", 100,
			Authentication_TSK, StackAuthentication_TSK);
	OS_TASK_CREATE(&CB_Diagnostic_TSK, "Diagnostic Task", 60, Diagnostic_TSK, StackDiagnostic_TSK);
	OS_TASK_CREATE(&CB_Test_TSK, "Test Task", 150, Teset_TSK, StackTest_TSK);

	OS_MUTEX_Create(&CAN_RxMsg_Mutex);
	OS_MUTEX_Create(&AuthToken_Mutex);

	OS_Start();


	while (1) {
		//NOP

	}

}

void RCC_Setup(void) {
	RCC_Config();
	RCC_EnablePeripheral(RCC_GPIOACLK);
	RCC_EnablePeripheral(RCC_TIM2CLK);
	RCC_EnablePeripheral(RCC_UART4CLK);
	RCC_EnablePeripheral(RCC_DMA1CLK);
	RCC_EnablePeripheral(RCC_CAN1CLK);
	RCC_EnablePeripheral(RCC_GPIOBCLK);
}

void Interrupt_Setup(void) {
	Enable_irq();
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	//NVIC_EnableIRQ(CAN1_RX1_IRQn);
	NVIC_EnableIRQ(DMA1_Stream4_IRQn);
	NVIC_EnableIRQ(DMA1_Stream2_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 4);
	NVIC_SetPriority(CAN1_RX0_IRQn, 13);
	NVIC_SetPriority(DMA1_Stream4_IRQn, 14);
	NVIC_SetPriority(DMA1_Stream2_IRQn, 14);

}

void GPIO_Setup(void) {
	GPIO_Config PA5 = {
	GPIO_PINMODE_OUTPUT,
	GPIO_PINSPEED_HIGH,
	GPIO_PINPULL_UP };
	GPIO_Config_Pin(GPIOA, GPIO_PIN_5, PA5);

	GPIO_Config PA0 = {
	GPIO_PINMODE_AF8,
	GPIO_PINSPEED_HIGH,
	GPIO_PINPULL_UP };
	GPIO_Config_Pin(GPIOA, GPIO_PIN_0, PA0);
	GPIO_Config PA1 = {
	GPIO_PINMODE_AF8,
	GPIO_PINSPEED_HIGH,
	GPIO_PINPULL_UP };
	GPIO_Config_Pin(GPIOA, GPIO_PIN_1, PA1);
	GPIO_Config PB9 = {
	GPIO_PINMODE_AF9,
	GPIO_PINSPEED_HIGH,
	GPIO_PINPULL_UP };
	GPIO_Config_Pin(GPIOB, GPIO_PIN_9, PB9);
	GPIO_Config PB8 = {
	GPIO_PINMODE_AF9,
	GPIO_PINSPEED_HIGH,
	GPIO_PINPULL_UP };
	GPIO_Config_Pin(GPIOB, GPIO_PIN_8, PB8);
}

void UART_Setup(void) {
	UART_INIT(UART4);
	DMA_Stream_Config Stream2 = {
	DMA_CHANNEL_PERIPHERAL_TO_MEMORY,
	DMA_CHANNEL_VERYHIGH,
	DMA_CHANNEL_8BITS,
	DMA_CHANNEL_8BITS,
	DMA_CHANNEL_MEMORY_INC_ENABLE,
	DMA_CHANNEL_PERIPHERAL_INC_DIABLE,
	DMA_CHANNEL_CIRCULAR_OFF,
	DMA_Channel4,

	};
	DMA_Stream_Config Stream4 = {
	DMA_CHANNEL_MEMORY_TO_PERIPHERAL,
	DMA_CHANNEL_VERYHIGH,
	DMA_CHANNEL_8BITS,
	DMA_CHANNEL_8BITS,
	DMA_CHANNEL_MEMORY_INC_ENABLE,
	DMA_CHANNEL_PERIPHERAL_INC_DIABLE,
	DMA_CHANNEL_CIRCULAR_OFF,
	DMA_Channel4,

	};
	DMA_StreamConfig(DMA1_Stream2, Stream2);
	DMA_StreamConfig(DMA1_Stream4, Stream4);
}

void CAN_Setup(void) {
	CAN_setupTypeDef CANConfigs = { DISABLE, DISABLE, DISABLE, DISABLE, DISABLE,
			DISABLE };
	CAN_Config(CAN1, &CANConfigs);
	CAN_Normal(CAN1);
	//for Testing let it pass all message then
	CAN_FilterTypeDef filter0;
	filter0.FilterActivation = CAN_FILTER_ENABLE;
	filter0.FilterBank = 1;
	filter0.FilterFIFOAssignment = CAN_FIFO0;
	filter0.FilterId = 0x0;
	filter0.FilterMaskId = 0x0;
	filter0.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_ConfigFilter(CAN1, &filter0);

	//CAN_Interrupt_Enable(CAN1, CAN_IER_FMPIE1, ENABLE);
	CAN_Interrupt_Enable(CAN1, CAN_IER_FMPIE0, ENABLE);
}
