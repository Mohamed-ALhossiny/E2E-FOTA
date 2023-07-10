/*
 * BOOTLOADER.h
 *
 *  Created on: Apr 28, 2023
 *      Author: ashra
 */

#ifndef CORE_BOOTLOADER_INC_BOOTLOADER_H_
#define CORE_BOOTLOADER_INC_BOOTLOADER_H_

#include"STD_TYPE.h"
#include"CAN_cfg.h"
#include "CAN.h"
#include "RCC.h"
#include "SysTick.h"
#include "SCB.h"
#include "NVIC.h"
#include"FLASH.h"


#define FLASH_BASE									   0x08000000
#define APP1_START_ADDRESS                             0x08005000			//Origin + Bootloader size (16kB + 4 reserved)
#define APP2_START_ADDRESS                             0x08012800			//Origin + Bootloader size (16kB + 4 reserved) + App1 Bank (54kB)
#define APP_BANK_SIZE                                  0xD800	        	//54kB
#define PAGE_SIZE_INDOUBLEWORD							128
#define PAGE_SIZE_BYTES									1024


#define BOOTLOADER_MODE_ADDRESS							0x08004000
#define BOOTLOADER_MODE									*((uint32_t*)BOOTLOADER_MODE_ADDRESS) //FOTA or Normal boot
#define BOOTLOADER_REQUEST								(uint32_t)0x000000001
#define BOOTLOADER_NORMALBOOT							(uint32_t)0x000000002
#define BOOTLOADER_FIRSTTIME							(uint32_t)0xfffffffff


#define BOOTLOADER_CURRENTBANK_ADDRESS							0x08004008
#define BOOTLOADER_CURRENTBANK							*((uint32_t*)BOOTLOADER_CURRENTBANK_ADDRESS) // Active bank

//CAN IDs
#define RQ_UPDATE_FIRMWARE              00000000001
#define GATEWAY_FLASH_ID                 0x123              // Define the CAN ID for the application size message





void BootLoader(void);

void USB_LP_CAN1_RX0_IRQHandler(void);

#endif /* CORE_BOOTLOADER_INC_BOOTLOADER_H_ */
