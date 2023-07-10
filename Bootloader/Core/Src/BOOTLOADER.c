/*

 * BOOTLOADER.c
 *
 *  Created on: Apr 24, 2023
 *      Author: ashra
 */

#include"BOOTLOADER.h"
#include"CAN.h"
#include "stdlib.h"
#include "lz.h"
#include "bspatch.h"


static uint8_t ReceiveAndFlashUpdate(void)

//static void BUFFER_CLEAR(uint64_t *block, uint16_t size);
static void Set_MSP(uint32_t topOfMainStack);
static void JumpToApplication( uint32_t appAddress);
static void System_DeInit(void);


CAN_msg CANTX_msg;
CAN_msg CANRX_msg;

uint8_t isFlashed = 0;

/*----------------------   Initialize Boot loader ( BL START )   ---------------------------*/

void BootLoader(void) {

	if(((BOOTLOADER_MODE & 0x00FF) == BOOTLOADER_REQUEST)){
		uint8_t ret = ReceiveAndFlashUpdate();
		CANTX_msg.id = GATEWAY_BOOTLOADER_RQ_ID;
		CANTX_msg.format = STANDARD_FORMAT;
		CANTX_msg.type = DATA_FRAME;
		CANTX_msg.length = 1;
		CANTX_msg.data[0] = ret;
		//Bootloader response
		CAN_Transmit(CAN, &CANTX_msg);
		System_DeInit();
		JumpToApplication(BOOTLOADER_CURRENTBANK);

	}else{
		System_DeInit();
		JumpToApplication(BOOTLOADER_CURRENTBANK);

	}

}

static uint8_t ReceiveAndFlashUpdate(void) {
	uint64_t* Dwords;
	uint8_t* patch;
	uint8_t updateStatus = (uint8_t)((BOOTLOADER_MODE & 0xFF00) >> 8);
	int16_t imageSize = 0;
	if(updateStatus == VERSION_PATCH){
		patch = (uint8_t*)malloc(500);
		if(patch == NULL){

			return BL_MEMORYLEAKAGE;
		}
	}else{
		Dwords = (uint64_t*)malloc(1024 * 9);
		if(Dwords == NULL){
			return BL_MEMORYLEAKAGE;
		}

	}
	CANTX_msg.id = GATEWAY_BOOTLOADER_RQ_ID;
	CANTX_msg.format = STANDARD_FORMAT;
	CANTX_msg.type = DATA_FRAME;
	CANTX_msg.length = 2;
	//Bootloader ACK
	CAN_Transmit(CAN, &CANTX_msg);


	CANTX_msg.length = 2;
	//receiving app size
	while((CANRX_msg.ActiveFlag != ENABLE) || (CANRX_msg.id != GATEWAY_BOOTLOADER_RQ_ID));
	CANRX_msg.ActiveFlag = DISABLE;
	uint16_t size = CANRX_msg.data[0] | CANRX_msg.data[1] << 8;
	//receiving the image
	uint16_t wordCounter = 0;
	while (wordCounter <= size) {

		//Wait double word
		while((CANRX_msg.ActiveFlag != ENABLE) || (CANRX_msg.id != GATEWAY_BOOTLOADER_RQ_ID));
		CANRX_msg.ActiveFlag = DISABLE;

		for (uint8_t msgCounter = 0; msgCounter < CANRX_msg.length; msgCounter++) {
			if(updateStatus == VERSION_PATCH){
				patch[wordCounter + msgCounter] = CANRX_msg.data[msgCounter];
			}else {
				Dwords[wordCounter / 8] = (Dwords[wordCounter / 8] << 8) | CANRX_msg.data[7 - msgCounter];
			}
		}
		wordCounter+=8;
		CANTX_msg.data[0] = (uint8_t)(0xFF & wordCounter);
		CANTX_msg.data[1] = (uint8_t)((0xFF00 & wordCounter) >> 8);
		//request next double word
		CAN_Transmit(CAN, &CANTX_msg);
	}

	if(updateStatus == VERSION_PATCH){
		uint8_t* unCompressedPatch = (uint8_t*)malloc(8600);//patch file
		if(unCompressedPatch == NULL){
			return BL_MEMORYLEAKAGE;
		}
		uint32_t UncompatchSize = 0;

		uint8_t* image;
		uint8_t *oldimage = (uint8_t*)BOOTLOADER_CURRENTBANK;
		uint32_t oldImageSize = BOOTLOADER_CURRENTBANK  == APP1_START_ADDRESS ? APP1_IMAGESIZE : APP2_IMAGESIZE;

		UncompatchSize = LZ_Uncompress( patch, unCompressedPatch, size);
		free(patch);
		if(UncompatchSize == -1){
			return BL_UNCOMPRESSION_ERROR;
		}
		imageSize = bspatch_newsize(unCompressedPatch, UncompatchSize);
		if(imageSize == -1){
			return BL_PATCH_INVALIED_HEADER;
		}
		image = (uint8_t*)malloc(imageSize);
		if(image == NULL){
			return BL_MEMORYLEAKAGE;
		}
		int8_t status = bspatch(oldimage,  oldImageSize, unCompressedPatch, UncompatchSize, image,  imageSize);
		if(status != 0){
			return BL_PATCH_UNPATCHING_ERROR;
		}
		free(unCompressedPatch);
		//flash Dword
		Program_Software((uint64_t*)image, imageSize / 8);
		free(image);
	}else {
		//flash Dword
		Program_Software(Dwords, size / 8);
	}
	if(isFlashed == 0){
		return BL_FLASH_ERROR;
	}
	FLASH_Unlock();
	//Update Flags
	uint32_t currentBank = BOOTLOADER_CURRENTBANK;
	FLASH_EraseTypeDef pEraseInit;
	pEraseInit.PageAddress = CONFURATION_PAGE;
	pEraseInit.NbPages = 1;
	FLASH_Erase(&pEraseInit);
	FLASH_Program(FLASH_TYPEPROGRAM_WORD, BOOTLOADER_MODE_ADDRESS, BOOTLOADER_NORMALBOOT);
	if(currentBank  == APP1_START_ADDRESS){

		FLASH_Program(FLASH_TYPEPROGRAM_WORD, BOOTLOADER_CURRENTBANK_ADDRESS, APP2_START_ADDRESS);
		FLASH_Program(FLASH_TYPEPROGRAM_WORD, APP2_IMAGESIZE_ADDRESS, updateStatus == VERSION_PATCH ? imageSize : size);

	}else {
		FLASH_Program(FLASH_TYPEPROGRAM_WORD, BOOTLOADER_CURRENTBANK_ADDRESS, APP1_START_ADDRESS);
		FLASH_Program(FLASH_TYPEPROGRAM_WORD, APP1_IMAGESIZE_ADDRESS, updateStatus == VERSION_PATCH ? imageSize : size);

	}
	FLASH_Lock();
	return BL_OK;

}


void Program_Software(uint64_t *Data, uint16_t Data_length) {
/*--------------------  Erase APPLICATION AREA ----------------------*/

	FLASH_Unlock();

	FLASH_EraseTypeDef pEraseInit;
	pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;

	/* Get the application start address to erase application */
	if(BOOTLOADER_CURRENTBANK == APP1_START_ADDRESS){
		pEraseInit.PageAddress = APP2_START_ADDRESS;
	}else if (BOOTLOADER_CURRENTBANK == APP2_START_ADDRESS){
		pEraseInit.PageAddress = APP1_START_ADDRESS;
	}else{
		pEraseInit.PageAddress = APP1_START_ADDRESS;
	}

	/* Get the number of pages to erase */
	pEraseInit.NbPages = APP_BANK_SIZE / PAGE_SIZE_BYTES;
	//FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORDWORD, (APP1_START_ADDRESS + APPLICATION_WRITE_INDEX), HALFWORD_DATA);
	FLASH_Erase(&pEraseInit);

	uint32_t currentBankBaseAddress = pEraseInit.PageAddress;
	for(uint32_t DWcounter = 0;DWcounter <= Data_length; DWcounter++ ){
		FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (currentBankBaseAddress + DWcounter * 0x8), Data[DWcounter]); //0x16 double word size
	}

	FLASH_Lock();
	isFlashed = 1;

}


/*
static void BUFFER_CLEAR(uint64_t *block, uint16_t size) {
	for (uint16_t i = 0; i < size; i++) {
		block[i] = 0;
	}
}
*/

static void Set_MSP(uint32_t topOfMainStack){
  __asm volatile ("MSR msp, %0" : : "r" (topOfMainStack) : );
}


static void JumpToApplication( uint32_t appAddress){

	void (*app_reset_handler)(void) = (void*)(*((volatile uint32_t*)(appAddress + 4U)));
	SCB->VTOR = appAddress - FLASH_BASE;

	Set_MSP(*(volatile uint32_t*) appAddress);

	app_reset_handler();    //call the app reset handler
}



static void System_DeInit(void){

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
	Disable_irq();

}




void USB_LP_CAN1_RX0_IRQHandler(void){
	if(CAN->RF0R & CAN_RF0R_FMP0){
		CAN_Receive(CAN, CAN_FIFO0, &CANRX_msg);//make sure that the function det active flage to enable
	}
}















