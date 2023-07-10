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
//#include "BOOTLOADER.h"
#include "TIM.h"

#define APP1_START_ADDRESS                              0x08005000			//Origin + Bootloader size (16kB + 4 reserved)
#define APP2_START_ADDRESS                              0x08012800			//Origin + Bootloader size (16kB + 4 reserved) + App1 Bank (54kB)
#define BOOTLOADER_CURRENTBANK_ADDRESS					0x08004004
#define BOOTLOADER_CURRENTBANK							*((uint32_t*)BOOTLOADER_CURRENTBANK_ADDRESS) // Active bank

#define BOOTLOADER_REQUEST								(uint32_t)0x000000001
#define BOOTLOADER_MODE_ADDRESS							0x08004000
#define BOOTLOADER_MODE									*((uint32_t*)BOOTLOADER_MODE_ADDRESS) //FOTA or Normal boot
#define FLASH_BASE									    0x08000000
#define CAN_BOOTLOADER_RQ_ID							0x123
#define CAN_CURRENTVERSION_ID							7
#define ECU_DIAGNOSTIC_LOG_ID							22
#define CAN_DATA_ID										6


void jumpToBoot(void);
uint16_t ULTRASONIC_GetDistance(void);
void CAN_SendData(uint8_t* buffer, uint16_t length, uint32_t CAN_id);

uint16_t ULTRASONIC_GetDistance(void){
	uint32_t Value1 = 0;
	uint32_t Value2 = 0;
	uint32_t SysTicks = 0;
	GPIO_Write_Pin(GPIOA, GPIO_PIN_9, HIGH);
	TIM_SetCounter(TIM1, 0);
	while (TIM_GetCounter(TIM1) < 10);
	GPIO_Write_Pin(GPIOA, GPIO_PIN_9, LOW);
	SysTicks = SysTick_GetTicks();

	while(!(GPIO_Read_Pin(GPIOA, GPIO_PIN_8)) && (SysTicks + 10 >  SysTick_GetTicks()));
	Value1 = TIM_GetCounter(TIM1);

	SysTicks = SysTick_GetTicks();
	while(GPIO_Read_Pin(GPIOA, GPIO_PIN_8) && (SysTicks + 50 > SysTick_GetTicks()));
	Value2 = TIM_GetCounter(TIM1);

	return((Value2-Value1)* 0.03448/2);
}


uint8_t diagFile[] = {
		  0x4E, 0x75, 0x6D, 0x62, 0x65, 0x72, 0x09, 0x09, 0x7C, 0x09, 0x50, 0x49, 0x4E, 0x41, 0x39, 0x09, 0x09, 0x7C, 0x09, 0x50, 0x49, 0x4E, 0x41, 0x38, 0x09, 0x09, 0x7C, 0x09, 0x4D, 0x65, 0x61, 0x73, 0x75, 0x72, 0x65, 0x64, 0x20, 0x44, 0x69, 0x73,
		  0x74, 0x61, 0x6E, 0x63, 0x65, 0x0D, 0x0A, 0x31, 0x09, 0x09, 0x09, 0x30, 0x09, 0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x32, 0x30, 0x0D, 0x0A, 0x32, 0x09, 0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x30, 0x09, 0x09, 0x09, 0x31, 0x32, 0x0D, 0x0A, 0x33,
		  0x09, 0x09, 0x09, 0x30, 0x09, 0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x32, 0x33, 0x0D, 0x0A, 0x34, 0x09, 0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x30, 0x09, 0x09, 0x09, 0x32, 0x34, 0x0D, 0x0A, 0x35, 0x09, 0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x31,
		  0x09, 0x09, 0x09, 0x34, 0x33, 0x0D, 0x0A, 0x36, 0x09, 0x09, 0x09, 0x30, 0x09, 0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x31, 0x31, 0x0D, 0x0A, 0x37, 0x09, 0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x30, 0x09, 0x09, 0x09, 0x34, 0x33, 0x0D, 0x0A, 0x38,
		  0x09, 0x09, 0x09, 0x30, 0x09, 0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x31, 0x32, 0x0D, 0x0A, 0x39, 0x09, 0x09, 0x09, 0x30, 0x09, 0x09, 0x09, 0x31, 0x09, 0x09, 0x09, 0x33, 0x32, 0x0D, 0x0A, 0x31, 0x30, 0x09, 0x09, 0x09, 0x30, 0x09, 0x09, 0x09,
		  0x30, 0x09, 0x09, 0x09, 0x34, 0x33
		};




uint16_t Distance  = 0;  // cm

CAN_msg RX_msg;
CAN_msg TX_msg;

uint8_t isdiagRequest = 0;
uint8_t isBootloaderRQ = 0;
const uint8_t currentVersion[] = "1.0.0";
const uint8_t ecu_id = 1;
int main(void) {
	FLASH_Unlock();
	FLASH_Program(FLASH_TYPEPROGRAM_WORD, BOOTLOADER_CURRENTBANK_ADDRESS, APP1_START_ADDRESS);
	FLASH_Lock();
	RCC_Config();
	SCB->VTOR = 0x08005000 - 0x08000000;
	Enable_irq();
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	SysTick_INIT();

	RCC_EnablePeripheral(RCC_AFIOCLK);
	RCC_EnablePeripheral(RCC_GPIOACLK);
	RCC_EnablePeripheral(RCC_GPIOCCLK);
	RCC_EnablePeripheral(RCC_CANCLK);
	//for ultrasonic
	RCC_EnablePeripheral(RCC_TIM1CLK);

	AFIO_INIT();
	GPIO_Config_Pin(GPIOC, GPIO_PIN_13, GPIO_PINMODE_OUTPUT50MHZ);
	GPIO_Config_Pin(GPIOA, GPIO_PIN_11, GPIO_PINMODE_INPUT); //rx
	GPIO_Config_Pin(GPIOA, GPIO_PIN_12, GPIO_PINMODE_AFIO); //tx

	GPIO_Config_Pin(GPIOA, GPIO_PIN_9, GPIO_PINMODE_OUTPUT50MHZ);//trigger
	GPIO_Config_Pin(GPIOA, GPIO_PIN_8, GPIO_PINMODE_INPUT);//echo
	GPIO_Config_Pin(GPIOA, GPIO_PIN_10, GPIO_PINMODE_OUTPUT50MHZ);//Buzzer

	CAN_setupTypeDef CANsetupStruct = { DISABLE, DISABLE, DISABLE, DISABLE,
			DISABLE, DISABLE, };
	CAN_Setup(CAN, &CANsetupStruct);
	CAN_Normal(CAN);
	CAN_FilterTypeDef filterConfig;
	filterConfig.FilterActivation = CAN_FILTER_ENABLE;
	filterConfig.FilterBank = 1;
	filterConfig.FilterFIFOAssignment = CAN_FIFO0;
	filterConfig.FilterId = CAN_CURRENTVERSION_ID;
	filterConfig.FilterMaskId = CAN_CURRENTVERSION_ID;
	filterConfig.FilterMode = CAN_FILTERMODE_LIST;
	CAN_ConfigFilter(CAN, &filterConfig);
	filterConfig.FilterActivation = CAN_FILTER_ENABLE;
	filterConfig.FilterBank = 2;
	filterConfig.FilterFIFOAssignment = CAN_FIFO0;
	filterConfig.FilterId = CAN_BOOTLOADER_RQ_ID;
	filterConfig.FilterMaskId = CAN_BOOTLOADER_RQ_ID;
	filterConfig.FilterMode = CAN_FILTERMODE_LIST;
	CAN_ConfigFilter(CAN, &filterConfig);
	CAN_Interrupt_Enable(CAN, CAN_IER_FMPIE0, ENABLE);
	filterConfig.FilterActivation = CAN_FILTER_ENABLE;
	filterConfig.FilterBank = 3;
	filterConfig.FilterFIFOAssignment = CAN_FIFO0;
	filterConfig.FilterId = ECU_DIAGNOSTIC_LOG_ID;
	filterConfig.FilterMaskId = ECU_DIAGNOSTIC_LOG_ID;
	filterConfig.FilterMode = CAN_FILTERMODE_LIST;
	CAN_ConfigFilter(CAN, &filterConfig);
	CAN_Interrupt_Enable(CAN, CAN_IER_FMPIE0, ENABLE);

	//Timer Configuration
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_Base_InitTypeDef Config ={0};
	Config.Prescaler = 72 - 1;
	Config.CounterMode = TIM_COUNTERMODE_UP;
	Config.Period = 0xffff;
	Config.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Config.RepetitionCounter = 0;
	Config.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	TIM_Base_SetConfig(TIM1, &Config);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	TIM_ConfigClockSource(TIM1, &sClockSourceConfig);
	TIM_MasterConfigSynchronization(TIM1, TIM_TRGO_RESET, TIM_MASTERSLAVEMODE_DISABLE);

	TIM_Base_Start(TIM1);
	GPIO_Write_Pin(GPIOA, GPIO_PIN_9, LOW);


	while(1){
		Distance = ULTRASONIC_GetDistance();

		//APP1
		if((Distance < 40) & (Distance > 2)){
			GPIO_Write_Pin(GPIOA, GPIO_PIN_10, HIGH);
			SysTick_Delay(500);
			GPIO_Write_Pin(GPIOA, GPIO_PIN_10, LOW);
			SysTick_Delay(500);
	        }
/*
		//APP2
		if((Distance < 40) & (Distance > 2)){
			GPIO_Write_Pin(GPIOA, GPIO_PIN_10, HIGH);
			SysTick_Delay(Distance * 10);
			GPIO_Write_Pin(GPIOA, GPIO_PIN_10, LOW);
			SysTick_Delay(Distance * 10);
		}
*/
		if(isdiagRequest == 1){
			isdiagRequest = 0;
			CAN_msg msg = {0};
			msg.id = CAN_DATA_ID;
			msg.format = STANDARD_FORMAT;
			msg.type = DATA_FRAME;
			msg.length = 2;
			msg.data[0] = (uint8_t)(0xFF & sizeof(diagFile));
			msg.data[1] = (uint8_t)(0xFF & (sizeof(diagFile) >> 8));
			CAN_Transmit(CAN, &msg);
			CAN_SendData(diagFile, sizeof(diagFile), CAN_DATA_ID);

		}
		if(isBootloaderRQ){
			isBootloaderRQ = 0;
			jumpToBoot();
		}

	}


}


void CAN_SendData(uint8_t* buffer, uint16_t length, uint32_t CAN_id){
	CAN_msg msg = {0};
	msg.id = CAN_id;
	msg.format = STANDARD_FORMAT;
	msg.type = DATA_FRAME;
	msg.length = length < 8 ? length : 8;
	for (uint8_t datacounter = 0; datacounter < length;
			datacounter += 8) {
		for (uint8_t msgcounter = 0; msgcounter < 8; msgcounter++) {
			msg.data[msgcounter] = buffer[datacounter
					+ msgcounter];
		}
		if((length - datacounter) >= 8){
			msg.length = 8;
		}else{
			msg.length = length - datacounter;
		}
		CAN_Transmit(CAN, &msg);
	}
}






void System_DeInit(void);
void System_DeInit(void){
	RCC->CFGR &= ~(RCC_CFGR_SW);
	while((RCC->CFGR & RCC_CFGR_SWS) != 0);
	RCC->CR &= ~(RCC_CR_PLLON);
	RCC->CR &= ~(RCC_CR_HSEON);
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	RCC_DisablePeripheral(RCC_AFIOCLK);
	RCC_DisablePeripheral(RCC_GPIOCCLK);
	RCC_DisablePeripheral(RCC_GPIOACLK);
	RCC_DisablePeripheral(RCC_CANCLK);
	RCC_DisablePeripheral(RCC_TIM1CLK);
	Disable_irq();
}

void JumpToApplication( uint32_t appAddress);
void JumpToApplication( uint32_t appAddress){
	void (*app_reset_handler)(void) = (void*)(*((volatile uint32_t*)(appAddress + 4U)));
	SCB->VTOR = appAddress - FLASH_BASE;

	__asm volatile ("MSR msp, %0" : : "r" (*(volatile uint32_t*)appAddress) : );

	app_reset_handler();
	//SCB_SystemReset();
}


void jumpToBoot(void){
	//GPIO_Write_Pin(GPIOC, GPIO_PIN_13, HIGH);
	if(RX_msg.data[0] == 0x4){
		return;
	}
	System_DeInit();
	FLASH_Unlock();
	FLASH_Program(FLASH_TYPEPROGRAM_WORD, BOOTLOADER_MODE_ADDRESS, BOOTLOADER_REQUEST | (RX_msg.data[0] << 8));
	FLASH_Lock();
	JumpToApplication(FLASH_BASE);
}

/*GPIO_Write_Pin(GPIOC, GPIO_PIN_13, HIGH);
		SysTick_Delay(1000);
		GPIO_Write_Pin(GPIOC, GPIO_PIN_13, LOW);
		SysTick_Delay(1000);*/

//fifo0
void USB_LP_CAN1_RX0_IRQHandler(void) {

	if (CAN->RF0R & CAN_RF0R_FMP0) {
		CAN_Receive(CAN, CAN_FIFO0, &RX_msg);
	}
	if((RX_msg.id == CAN_CURRENTVERSION_ID) && (RX_msg.ActiveFlag == ENABLE)){
		GPIO_Write_Pin(GPIOA, GPIO_PIN_10, LOW);
		TX_msg.id = CAN_CURRENTVERSION_ID;
		TX_msg.format = STANDARD_FORMAT;
		TX_msg.type = DATA_FRAME;
		TX_msg.length = 2;
		TX_msg.data[0] = 5;
		TX_msg.data[1] = 0;
		CAN_Transmit(CAN, &TX_msg);
		TX_msg.length = 5;
		TX_msg.data[0] = currentVersion[0];
		TX_msg.data[1] = currentVersion[1];
		TX_msg.data[2] = currentVersion[2];
		TX_msg.data[3] = currentVersion[3];
		TX_msg.data[4] = currentVersion[4];
		CAN_Transmit(CAN, &TX_msg);

	}else if ((RX_msg.id == CAN_BOOTLOADER_RQ_ID) && (RX_msg.ActiveFlag == ENABLE)) {
		isBootloaderRQ = 1;

	}else if ((RX_msg.id == ECU_DIAGNOSTIC_LOG_ID) && (ecu_id == (RX_msg.data[0] | RX_msg.data[1] << 8)) && (RX_msg.ActiveFlag == ENABLE)){
		isdiagRequest = 1;
		RX_msg.ActiveFlag = DISABLE;
	}

}


