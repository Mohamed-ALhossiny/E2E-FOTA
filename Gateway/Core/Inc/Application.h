/*
 * Application.h
 *
 *  Created on: May 7, 2023
 *      Author: moham
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

#include "STD_TYPE.h"
#include "RTOS.h"
#include "CAN.h"
#include "GSM.h"
#include "GPIO.h"
#include "main.h"
#include "SEGGER_SYSVIEW.h"
#include "gcm.h"
#include "base64.h"




/*------------------------CAN Commands--------------------------------*/

//GUI Tasks CAN IDs
#define GUI_LOGIN_ID							1
#define GUI_SIGNUP_RQ_ID						2
#define GUI_CHECKUPDATE_RQ_ID					3
#define GUI_UPDATE_RQ_ID						4
#define GUI_DIAGNOSTIC_RQ_ID					5
#define GUI_DATA_ID								6


//APP ECUs Tasks CAN IDs
#define ECU_FIRMWAREVERSION_ID					7
#define ECU_BOOTLOADER_RQ_ID					8
#define ECU_FLASH_RQ_ID							9
#define ECU_ERASE_RQ_ID							10
#define ECU_BOOTLAODER_RQ_ID					0x123
#define ECU_IMAGE_ID							0x122


/*-------------------------------------------------------------------*/

/*-----------------------------TASK EVENTs---------------------------*/
//Update_TSK
#define TSK_EVENT_UPDATE							0x1
#define TSK_EVENT_CHECKUPDATE						0x2

//Authentication_TSK
#define TSK_ENENT_LOGIN								(1u << 0)
#define TSK_ENENT_SIGNUP							(1u << 1)

//SendDownloadProgress_TSK
#define TSK_ENENT_SENDPROGRESS						(1u << 0)

//Diagnostic_TSK
#define TSK_ENENT_SENDDIAGNOSTICS					(1u << 0)




/*-------------------------------------------------------------------*/

/*---------------------------URL Macros------------------------------*/
#define BASE_URL						"https://6bde-197-54-40-16.ngrok-free.app/"
#define SIGNUP_ENDNODE					"signup/"
#define LOGIN_ENDNODE					"token/"
#define GETUSERINFO_ENDNODE				"users/"
#define GETCARINFO_ENDNODE				"cars/"
#define GETFIRMWARE_ENDNODE				"download/?filename="
#define GETLAST_FIRMWARE_INFO_ENDNODE	"firmware/?"
#define DIAGNOSTIC_ENDNODE				"ecu_diag/?"

/*-------------------------------------------------------------------*/

/*---------------------------Functions Declaration-------------------*/
void CAN_ReceiveStr(uint8_t* buffer, uint32_t msg_ID, uint16_t dataSize);
void CAN_SendData(uint8_t* buffer, uint16_t length, uint32_t CAN_id);
void StrCat(uint8_t* buffer, uint8_t* str1, uint8_t* str2);
void ParseVersion(const uint8_t* version, uint8_t* major, uint8_t* minor, uint8_t* patch);
uint8_t CompareVersions(const uint8_t* currentVersion, const uint8_t* targetVersion);
uint8_t NumToSting(uint32_t num, uint8_t *buffer);
uint16_t StringToNum(uint8_t* String);
void CAN1_RX0_IRQHandler(void);

/*------------------------TSKs-------------------------*/
void Authentication_TSK(void);
void Update_TSK(void);
void SendDownloadProgress_TSK(void);
void Diagnostic_TSK(void);
void Teset_TSK(void);

/********************************Macros*******************************/
#define VERSION_FULLUPDATE						0x1
#define VERSION_NEWFEATURES						0x2
#define VERSION_PATCH							0x3
#define VERSION_NOCHANGE						0x4
#define VERSION_ROLLBACK						0x5

/**************************BootLoader Status**************************/
#define BL_OK									0x00
#define BL_MEMORYLEAKAGE						0x11
#define BL_UNCOMPRESSION_ERROR					0x22
#define BL_PATCH_INVALIED_HEADER				0x33
#define BL_PATCH_UNPATCHING_ERROR				0x44
#define BL_FLASH_ERROR							0x55
#define BL_TIMEOUT								0x66

/*--------------------------- Decryption INFO------------------------*/

#define IV_LENGTH								12
#define TAG_LENGTH								16



/*-------------------------------------------------------------------*/


#endif /* INC_APPLICATION_H_ */
