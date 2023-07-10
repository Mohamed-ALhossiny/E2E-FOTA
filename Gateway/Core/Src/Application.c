/*
 * Application.c
 *
 *  Created on: May 7, 2023
 *      Author: Mohamed
 * TODO: implement diagnostics
 */

#include "Application.h"

/*--------------------------------------------------------------------*/
/*---------------------------TASKS------------------------------------*/
//Update_TSK
OS_STACKPTR uint8_t StackUpdate_TSK[5120]; //Task Stack
OS_TASK CB_Update_TSK; //Control Block

//Authentication_TSK
OS_STACKPTR uint8_t StackAuthentication_TSK[1024]; //Task Stack
OS_TASK CB_Authentication_TSK; //Control Block

//SendDownloadProgress_TSK
OS_STACKPTR uint8_t StackSendDownloadProgress_TSK[1024]; //Task Stack
OS_TASK CB_SendDownloadProgress_TSK; //Control Block

//Diagnostic_TSK
OS_STACKPTR uint8_t StackDiagnostic_TSK[1024]; //Task Stack
OS_TASK CB_Diagnostic_TSK; //Control Block

//Test_TSK
OS_STACKPTR uint8_t StackTest_TSK[512]; //Task Stack
OS_TASK CB_Test_TSK; //Control Block

/*---------------------------MUTEX------------------------------------*/
OS_MUTEX CAN_RxMsg_Mutex;
OS_MUTEX AuthToken_Mutex;
/*--------------------------------------------------------------------*/

CAN_msg CAN_RXMsg;

uint8_t *gsm_InternalBuffer;
uint8_t decryptionKey[] = "2b7e151628aed2a6";
uint8_t AuthToken[150];
uint8_t *firmwareBase64File; // encoded file
uint16_t firmwareBase64Size = 0;
uint8_t *firmwareFile; //The binary it self
size_t firmwareFileSize = 0;

void Authentication_TSK(void) {
	OS_TASKEVENT AuthenticationEvents;
	uint8_t url[50] = { 0 };
	uint16_t GSM_State = GSM_NOK;
	uint8_t *gsm_InternalBuffer;
	uint16_t gsm_RxDataSize = 0;
	GSM_TxType gsm_Tx;
	uint8_t *gsm_RxData;

	while (1) {
		AuthenticationEvents = OS_TASKEVENT_GetSingleBlocked(
		TSK_ENENT_LOGIN | TSK_ENENT_SIGNUP);
		SEGGER_SYSVIEW_PrintfHost("Authentication TSK Starts ...");
		CAN_msg TX_msg = { 0 };
		uint32_t userDataSize = 0;
		uint8_t *pUserData;
		uint8_t header[100] = { 0 };

		userDataSize = CAN_RXMsg.data[0] | CAN_RXMsg.data[1] << 8;
		CAN_RXMsg.ActiveFlag = DISABLE;
		//Note the data from GUI in Content-Type: application/x-www-form-urlencoded for login but application/json for signup
		SEGGER_SYSVIEW_PrintfHost("User Data Size = %d", userDataSize);
		//Allocate user data buffer
		OS_ThreadSafe_Lock();
		pUserData = (uint8_t*) OS_HEAP_malloc(
				(userDataSize + 1) * sizeof(uint8_t));
		OS_ThreadSafe_Unlock();
		if (pUserData == NULL) {
			SEGGER_SYSVIEW_PrintfHost("user data Buffer not Allocated");
			continue;
		}
		SEGGER_SYSVIEW_PrintfHost("user data Buffer Allocated!");
		//receive user data from GUI
		CAN_ReceiveStr(pUserData, GUI_DATA_ID, userDataSize);

		SEGGER_SYSVIEW_PrintfHost("User Data Received!");
		gsm_Tx.payload = pUserData;
		gsm_Tx.payloadSize = userDataSize;
		gsm_Tx.header = NULL;
		//Allocate GSM Internal Buffer
		OS_ThreadSafe_Lock();
		gsm_InternalBuffer = (uint8_t*) OS_HEAP_malloc(300 * sizeof(uint8_t));
		OS_ThreadSafe_Unlock();
		if (gsm_InternalBuffer == NULL) {
			SEGGER_SYSVIEW_PrintfHost("gsm Buffer not Allocated");
			continue;
		}
		GSM_SetInternalBuffer(gsm_InternalBuffer, 300);

		/*---------------------------TSK_LOGIN-----------------------------------------*/
		if (AuthenticationEvents & TSK_ENENT_LOGIN) {

			gsm_Tx.contentType = (uint8_t*) "application/x-www-form-urlencoded";
			//request the token
			StrCat(url, (uint8_t*) BASE_URL, (uint8_t*) LOGIN_ENDNODE);

			/*---------------------------TSK_SIGNUP-----------------------------------------*/
		} else if (AuthenticationEvents & TSK_ENENT_SIGNUP) {

			uint8_t dataSizeStr[10] = { 0 };
			NumToSting(userDataSize, dataSizeStr);
			StrCat(header, (uint8_t*) "Connection: keep-alive;"
					"Accept-Encoding: gzip, deflate, br;Accept: */*;"
					"Content-Length: ", dataSizeStr);
			gsm_Tx.header = header;
			gsm_Tx.contentType = (uint8_t*) "application/json";
			StrCat(url, (uint8_t*) BASE_URL, (uint8_t*) SIGNUP_ENDNODE);

		} else {
			//NOP
		}
		GSM_State = GSM_HTTPRequest(GSM_POST, url, &gsm_Tx, &gsm_RxDataSize);
		//Send the State to GUI
		TX_msg.format = CAN_STANDARD_FORMAT;
		TX_msg.type = CAN_DATA_FRAME;
		TX_msg.id = GUI_LOGIN_ID;
		TX_msg.length = 2;
		TX_msg.data[0] = (0xFF) & GSM_State;
		TX_msg.data[1] = ((0xFF00) & GSM_State) >> 8;

		if (GSM_State != 200) {
			SEGGER_SYSVIEW_PrintfHost(
					"Token is not Received or Signup failed!");

		}
		uint8_t date[11] = { 0 };
		if ((AuthenticationEvents & TSK_ENENT_LOGIN) && (GSM_State == 200)) {
			//Allocate received data buffer
			OS_ThreadSafe_Lock();
			gsm_RxData = (uint8_t*) OS_HEAP_malloc(gsm_RxDataSize);
			OS_ThreadSafe_Unlock();
			if (gsm_RxData == NULL) {
				SEGGER_SYSVIEW_PrintfHost("received data Buffer not Allocated");
				continue;
			}
			GSM_DownloadData(gsm_RxData, NULL);
			OS_MUTEX_LockBlocked(&AuthToken_Mutex);
			parse_json(gsm_RxData, (uint8_t*) "access_token", AuthToken);
			OS_MUTEX_Unlock(&AuthToken_Mutex);
			parse_json(gsm_RxData, (uint8_t*) "date", date);
			OS_HEAP_free(gsm_RxData);
			SEGGER_SYSVIEW_PrintfHost("Token is Received!");
			TX_msg.length = 8;
			//dd-mm-yyyy
			//days
			TX_msg.data[2] = date[0];
			TX_msg.data[3] = date[1];
			//months
			TX_msg.data[4] = date[3];
			TX_msg.data[5] = date[4];
			//years
			TX_msg.data[6] = date[8];
			TX_msg.data[7] = date[9];
		}
		CAN_Transmit(CAN1, &TX_msg);
		OS_ThreadSafe_Lock();
		OS_HEAP_free(pUserData);
		OS_HEAP_free(gsm_InternalBuffer);
		OS_ThreadSafe_Unlock();
		GSM_State = GSM_NOK;

	}
}

void Update_TSK(void) {
	OS_TASKEVENT UpdateEvents;
	CAN_msg TX_msg = { 0 };
	//uint16_t ecuId = 0;
	uint8_t updateState = 0;
	uint8_t url[100] = { 0 };
	uint16_t GSM_State = GSM_NOK;
	uint8_t *TxHeader;
	uint8_t *firmwareInfo;
	uint16_t firmwareInfoSize = 0;
	size_t decodedFileSize = 0;


	while (1) {
		GSM_TxType TxData = { NULL, 0, NULL, NULL };
		UpdateEvents = OS_TASKEVENT_GetSingleBlocked(
		TSK_EVENT_UPDATE | TSK_EVENT_CHECKUPDATE);

		/*------authorize----------*/
		OS_ThreadSafe_Lock();
		TxHeader = (uint8_t*) OS_HEAP_malloc(200);
		OS_ThreadSafe_Unlock();
		OS_MUTEX_LockBlocked(&AuthToken_Mutex);
		StrCat(TxHeader, (uint8_t*) "Authorization: Bearer ", AuthToken);
		OS_MUTEX_Unlock(&AuthToken_Mutex);
		TxData.header = TxHeader;
		//Allocate GSM Internal Buffer
		OS_ThreadSafe_Lock();
		gsm_InternalBuffer = (uint8_t*) OS_HEAP_malloc(
				((UpdateEvents & TSK_EVENT_UPDATE) ? 30000 : 1000)
						* sizeof(uint8_t));
		OS_ThreadSafe_Unlock();
		if (gsm_InternalBuffer == NULL) {
			SEGGER_SYSVIEW_PrintfHost("gsm Buffer not Allocated");
		}
		GSM_SetInternalBuffer(gsm_InternalBuffer,
				((UpdateEvents & TSK_EVENT_UPDATE) ? 30000 : 1000));

		/*---------------------------------------TSK_UPDATE-----------------------------------------*/

		if (UpdateEvents & TSK_EVENT_UPDATE) {

			//firmware_id in ASCII
			uint8_t firmware_id[9] = { 0 };
			for (uint8_t counter = 0; counter <= CAN_RXMsg.length - 1;
					counter++) {
				firmware_id[counter] = CAN_RXMsg.data[counter];
			}
			CAN_RXMsg.ActiveFlag = DISABLE;

			/*----------------URL----------------*/
			StrCat(url, (uint8_t*) BASE_URL, (uint8_t*) GETFIRMWARE_ENDNODE);
			StrCat(url, url, (uint8_t*) firmware_id);
			StrCat(url, url, (uint8_t*) ".fota");

			GSM_State = GSM_HTTPRequest(GSM_GET, url, &TxData,
					&firmwareBase64Size);
			//send state to GUI
			TX_msg.id = GUI_UPDATE_RQ_ID;
			TX_msg.format = CAN_STANDARD_FORMAT;
			TX_msg.type = CAN_DATA_FRAME;
			TX_msg.length = 4;
			TX_msg.data[0] = (uint8_t) (0xFF & GSM_State);
			TX_msg.data[1] = (uint8_t) ((0xFF00 & GSM_State) >> 8);
			TX_msg.data[2] = (uint8_t) (0xFF & firmwareBase64Size);
			TX_msg.data[3] = (uint8_t) ((0xFF00 & firmwareBase64Size) >> 8);

			CAN_Transmit(CAN1, &TX_msg);
			if (GSM_State == 200) {
				OS_ThreadSafe_Lock();
				firmwareBase64File = (uint8_t*) OS_HEAP_malloc(
						firmwareBase64Size);
				if (firmwareBase64File == NULL) {
					SEGGER_SYSVIEW_PrintfHost("Firmware is not Received!");
					OS_HEAP_free(TxHeader);
					OS_HEAP_free(gsm_InternalBuffer);
					continue;
				}
				OS_ThreadSafe_Unlock();
				OS_TASKEVENT_Set(&CB_SendDownloadProgress_TSK,
				TSK_ENENT_SENDPROGRESS);
				GSM_DownloadData(firmwareBase64File, NULL);
			} else {
				SEGGER_SYSVIEW_PrintfHost("Firmware is not Received!");
				OS_HEAP_free(TxHeader);
				OS_HEAP_free(gsm_InternalBuffer);
				continue;
			}


			//get firmware Size
			mbedtls_base64_decode(NULL, 0, &decodedFileSize, firmwareBase64File,
					firmwareBase64Size);

			if (decodedFileSize == 0) {
				SEGGER_SYSVIEW_PrintfHost("Base64 decoding Error!");
				OS_HEAP_free(firmwareBase64File);
				continue;
			}
			OS_ThreadSafe_Lock();
			firmwareFile = (uint8_t*) OS_HEAP_malloc(decodedFileSize + 5);
			OS_ThreadSafe_Unlock();

			int32_t decodingStatus = 0;

			decodingStatus = mbedtls_base64_decode(firmwareFile,
					decodedFileSize, NULL, firmwareBase64File,
					firmwareBase64Size);
			if ((decodingStatus == MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL)
					|| (decodingStatus == MBEDTLS_ERR_BASE64_INVALID_CHARACTER)) {
				SEGGER_SYSVIEW_PrintfHost("Base64 decoding Error!");
				OS_HEAP_free(firmwareBase64File);
				OS_HEAP_free(firmwareFile);
				continue;
			}
			OS_HEAP_free(firmwareBase64File);

			firmwareFileSize = decodedFileSize - (IV_LENGTH + TAG_LENGTH);

			mbedtls_gcm_context ctx = { 0 };
			int32_t decryptionStatus = 0;

			mbedtls_gcm_init(&ctx);
			mbedtls_gcm_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, decryptionKey, 128);

			decryptionStatus = mbedtls_gcm_auth_decrypt(&ctx, firmwareFileSize,
					firmwareFile,
					IV_LENGTH,
					NULL, 0, (firmwareFile + decodedFileSize - TAG_LENGTH),
					TAG_LENGTH, (firmwareFile + IV_LENGTH), firmwareFile);
			if ((decryptionStatus == MBEDTLS_ERR_GCM_AUTH_FAILED)
					|| (decryptionStatus == MBEDTLS_ERR_GCM_BAD_INPUT)
					|| (decryptionStatus == MBEDTLS_ERR_GCM_BUFFER_TOO_SMALL)) {
				SEGGER_SYSVIEW_PrintfHost("decryption failed code : %d !",
						decryptionStatus);
				OS_HEAP_free(firmwareFile);
				continue;
			}
			mbedtls_gcm_free(&ctx);

			// So, i have now the firmware in 'firmwareFile' buffer and its size is 'firmwareFileSize'
			// The next step is to request bootloader from the ECU then send and flash it
			/*----------------BL----------------*/
			//BL Request
			TX_msg.id = ECU_BOOTLAODER_RQ_ID;
			TX_msg.format = CAN_STANDARD_FORMAT;
			TX_msg.type = CAN_DATA_FRAME;
			TX_msg.length = 1;
			TX_msg.data[0] = updateState;
			CAN_Transmit(CAN1, &TX_msg);

			//Wait for BL ACK
			while ((CAN_RXMsg.ActiveFlag != ENABLE)
					|| (CAN_RXMsg.id != ECU_BOOTLAODER_RQ_ID))
				;
			CAN_RXMsg.ActiveFlag = DISABLE;
			SEGGER_SYSVIEW_PrintfHost("BootLoader ACK received!");

			//image size
			TX_msg.format = CAN_STANDARD_FORMAT;
			TX_msg.type = CAN_DATA_FRAME;
			TX_msg.length = 2;
			TX_msg.data[0] = (uint8_t) (0xFF & firmwareFileSize);
			TX_msg.data[1] = (uint8_t) ((0xFF00 & firmwareFileSize) >> 8);
			SEGGER_SYSVIEW_PrintfHost("Sending image to ECU");
			CAN_Transmit(CAN1, &TX_msg);
			TX_msg.length = 8;
			//image
			uint16_t DWCounter = 0;
			while (DWCounter <= firmwareFileSize) {
				for (int16_t datacounter = 0; datacounter < 8; datacounter++) {
					TX_msg.data[datacounter] = (firmwareFile[DWCounter
							+ datacounter]);
				}
				if(firmwareFileSize - DWCounter < 8){
					TX_msg.length = firmwareFileSize - DWCounter;
				}
				CAN_Transmit(CAN1, &TX_msg);
				while ((CAN_RXMsg.ActiveFlag != ENABLE)
						|| (CAN_RXMsg.id != ECU_BOOTLAODER_RQ_ID))
					;
				CAN_RXMsg.ActiveFlag = DISABLE;
				DWCounter = CAN_RXMsg.data[0] | CAN_RXMsg.data[1] << 8;
			}
			OS_HEAP_free(firmwareFile);
			SEGGER_SYSVIEW_PrintfHost("Image received! in ECU!");

			//Wait for BL ACK
			OS_TIME Time = OS_TIME_GetTicks();
			while ((CAN_RXMsg.ActiveFlag != ENABLE)
					|| (CAN_RXMsg.id != ECU_BOOTLAODER_RQ_ID)){
				if((OS_TIME_GetTicks()- Time) > 3000){
					TX_msg.id = GUI_UPDATE_RQ_ID;
					TX_msg.format = CAN_STANDARD_FORMAT;
					TX_msg.type = CAN_DATA_FRAME;
					TX_msg.length = 1;
					TX_msg.data[0] = BL_TIMEOUT;
					CAN_Transmit(CAN1, &TX_msg);
					break;
				}
			}
			if(CAN_RXMsg.ActiveFlag != DISABLE){
				TX_msg.id = GUI_UPDATE_RQ_ID;
				TX_msg.format = CAN_STANDARD_FORMAT;
				TX_msg.type = CAN_DATA_FRAME;
				TX_msg.length = 1;
				TX_msg.data[0] = CAN_RXMsg.data[0];
				CAN_Transmit(CAN1, &TX_msg);
			}
			CAN_RXMsg.ActiveFlag = DISABLE;



			/*-----------------------------------TSK_CHECKUPDATE--------------------------------------*/

		} else if (UpdateEvents & TSK_EVENT_CHECKUPDATE) {
			uint8_t metaDataSize = 0;
			uint8_t *pmetaData;
			uint8_t firmware_version[20] = { 0 };


			metaDataSize = CAN_RXMsg.data[0] | CAN_RXMsg.data[1] << 8;
			CAN_RXMsg.ActiveFlag = DISABLE;
			SEGGER_SYSVIEW_PrintfHost("Meta Data Size = %d", metaDataSize);
			OS_ThreadSafe_Lock();
			pmetaData = (uint8_t*) OS_HEAP_malloc(
					metaDataSize * sizeof(uint8_t));
			OS_ThreadSafe_Unlock();
			if (pmetaData == NULL) {
				SEGGER_SYSVIEW_PrintfHost("meta data Buffer not Allocated");
			}
			SEGGER_SYSVIEW_PrintfHost("meta data Buffer Allocated");
			//receive meta data from GUI
			CAN_ReceiveStr(pmetaData, GUI_DATA_ID, metaDataSize);

			SEGGER_SYSVIEW_PrintfHost("meta Data Received!");

			//car_id=22&ecu_id=22
			StrCat(url, (uint8_t*) BASE_URL,
					(uint8_t*) GETLAST_FIRMWARE_INFO_ENDNODE);
			StrCat(url, url, (uint8_t*) pmetaData);

			GSM_State = GSM_HTTPRequest(GSM_GET, url, &TxData,
					&firmwareInfoSize);
			//send state to GUI
			TX_msg.id = GUI_CHECKUPDATE_RQ_ID;
			TX_msg.format = CAN_STANDARD_FORMAT;
			TX_msg.type = CAN_DATA_FRAME;
			TX_msg.length = 2;
			TX_msg.data[0] = (uint8_t) (0xFF & GSM_State);
			TX_msg.data[1] = (uint8_t) ((0xFF00 & GSM_State) >> 8);
			CAN_Transmit(CAN1, &TX_msg);

			if (GSM_State != 200) {
				SEGGER_SYSVIEW_PrintfHost("Request Error State = %d",
						GSM_State);
				OS_HEAP_free(TxHeader);
				OS_HEAP_free(gsm_InternalBuffer);
				continue;
			}

			TX_msg.id = ECU_FIRMWAREVERSION_ID;
			TX_msg.format = CAN_STANDARD_FORMAT;
			TX_msg.type = CAN_DATA_FRAME;
			CAN_Transmit(CAN1, &TX_msg);
			uint8_t currentVersion[20] = { 0 };
			uint8_t currentVersionSize = 0;
			//wait for current version size
			while ((CAN_RXMsg.id != ECU_FIRMWAREVERSION_ID)
					|| (CAN_RXMsg.ActiveFlag == DISABLE))
				;
			currentVersionSize = CAN_RXMsg.data[0] | CAN_RXMsg.data[1] << 8;
			CAN_RXMsg.ActiveFlag = DISABLE;
			//receive current version from ECU
			CAN_ReceiveStr(currentVersion, ECU_FIRMWAREVERSION_ID, currentVersionSize);

			OS_ThreadSafe_Lock();
			firmwareInfo = (uint8_t*) OS_HEAP_malloc(firmwareInfoSize);
			OS_ThreadSafe_Unlock();
			GSM_DownloadData(firmwareInfo, NULL);
			parse_json(firmwareInfo, (uint8_t*) "firmware_version",
					firmware_version);

			updateState = CompareVersions((const uint8_t*) currentVersion,
					firmware_version);
			//send state to GUI
			TX_msg.id = GUI_CHECKUPDATE_RQ_ID;
			TX_msg.format = CAN_STANDARD_FORMAT;
			TX_msg.type = CAN_DATA_FRAME;
			TX_msg.length = 1;
			TX_msg.data[0] = updateState;
			CAN_Transmit(CAN1, &TX_msg);
			if (updateState == VERSION_NOCHANGE) {
				SEGGER_SYSVIEW_PrintfHost("VERSION is up to date");
				OS_HEAP_free(TxHeader);
				OS_HEAP_free(gsm_InternalBuffer);
				OS_HEAP_free(firmwareInfo);
				continue;
			}
			SEGGER_SYSVIEW_PrintfHost("sending update info to GUI!");
			TX_msg.length = 2;
			TX_msg.data[0] = (uint8_t) (0xFF & firmwareInfoSize);
			TX_msg.data[1] = (uint8_t) ((0xFF00 & firmwareInfoSize) >> 8);
			//OS_TASK_Delay(100);
			CAN_Transmit(CAN1, &TX_msg);
			CAN_SendData(firmwareInfo, firmwareInfoSize, GUI_CHECKUPDATE_RQ_ID);
			OS_HEAP_free(firmwareInfo);

		} else {
			//NOP
		}
		OS_HEAP_free(TxHeader);
		OS_HEAP_free(gsm_InternalBuffer);
	}
}



void SendDownloadProgress_TSK(void) {
	CAN_msg TX_msg = { 0 };
	uint16_t progress = 0;
	uint32_t dataStart = 0;

	TX_msg.id = GUI_UPDATE_RQ_ID;
	TX_msg.format = CAN_STANDARD_FORMAT;
	TX_msg.type = CAN_DATA_FRAME;
	TX_msg.length = 2;

	while (1) {
		OS_TASKEVENT_GetSingleBlocked(TSK_ENENT_SENDPROGRESS);
		while (strSearch(gsm_InternalBuffer, (const uint8_t*) "\n", 2,
				&dataStart) != GSM_OK) {
			OS_Delay(100);
		}
		do {

			progress = DMA_NDTR_MAX - (dataStart + (GSM_RX_DMA_STREAM->NDTR));
			if(progress > firmwareBase64Size){
				progress = firmwareBase64Size;
			}
			if (progress != 0) {

				TX_msg.data[0] = (uint8_t) (0xFF & progress);
				TX_msg.data[1] = (uint8_t) ((0xFF00 & progress) >> 8);
				CAN_Transmit(CAN1, &TX_msg);
			}
			OS_Delay(100);
		} while (progress < firmwareBase64Size);


	}
}

void Diagnostic_TSK(void) {
	CAN_msg CAN_Tx = { 0 };
	uint8_t *diagData;
	uint16_t diagDataSize = 0;
	uint8_t *jsonfile;
	uint16_t metaDataSize = 0;
	uint16_t GSM_Status = GSM_NOK;
	while (1) {
		GSM_TxType gsm_Tx = { NULL, 0, NULL, NULL };
		uint8_t metaData[40] = { 0 };
		uint8_t url[100] = { 0 };
		uint8_t TxHeader[300] = { 0 };
		OS_TASKEVENT_GetSingleBlocked(TSK_ENENT_SENDDIAGNOSTICS);
		diagDataSize = CAN_RXMsg.data[0] | CAN_RXMsg.data[1] << 8;
		CAN_RXMsg.ActiveFlag = DISABLE;

		OS_ThreadSafe_Lock();
		diagData = (uint8_t*) OS_HEAP_malloc(diagDataSize + 15);
		OS_ThreadSafe_Unlock();
		if (diagData == NULL) {
			SEGGER_SYSVIEW_PrintfHost(
					"Failed to allocate Diagnostics buffer !");
			continue;
		}

		//receive diag data from GUI
		CAN_ReceiveStr(diagData, GUI_DATA_ID, diagDataSize);
		//car_id=--&ecu_id=--
		while ((CAN_RXMsg.id != GUI_DATA_ID) || (CAN_RXMsg.ActiveFlag == DISABLE));
		CAN_RXMsg.ActiveFlag = DISABLE;
		metaDataSize = CAN_RXMsg.data[0] | CAN_RXMsg.data[1] << 8;
		CAN_ReceiveStr(metaData, GUI_DATA_ID, metaDataSize);

		OS_ThreadSafe_Lock();
		jsonfile = (uint8_t*) OS_HEAP_malloc(diagDataSize + 20);
		OS_ThreadSafe_Unlock();
		if (diagData == NULL) {
			SEGGER_SYSVIEW_PrintfHost(
					"Failed to allocate Diagnostics json buffer !");
			OS_HEAP_free(diagData);
			continue;
		}
		StrCat(jsonfile, (uint8_t*) "{\"file\":\"", diagData);
		StrCat(jsonfile, jsonfile, (uint8_t*) "\"}");
		gsm_Tx.payload = jsonfile;
		gsm_Tx.payloadSize = diagDataSize + 11;
		gsm_Tx.contentType = (uint8_t*) "application/json";

		uint8_t dataSizeStr[10] = { 0 };
		NumToSting(diagDataSize + 11, dataSizeStr);
		StrCat(TxHeader, (uint8_t*) "Connection: keep-alive;"
				"Accept-Encoding: gzip, deflate, br;Accept: */*;"
				"Content-Length: ", dataSizeStr);
		StrCat(TxHeader, TxHeader, (uint8_t*) ";");
		gsm_Tx.header = TxHeader;
		gsm_Tx.contentType = (uint8_t*) "application/json";
		StrCat(url, (uint8_t*) BASE_URL, (uint8_t*) DIAGNOSTIC_ENDNODE);
		StrCat(url, url, metaData);
		//Allocate GSM Internal Buffer
		OS_ThreadSafe_Lock();
		gsm_InternalBuffer = (uint8_t*) OS_HEAP_malloc(100);
		OS_ThreadSafe_Unlock();
		if (gsm_InternalBuffer == NULL) {
			SEGGER_SYSVIEW_PrintfHost("gsm Buffer not Allocated");
			OS_HEAP_free(jsonfile);
			OS_HEAP_free(diagData);
			continue;
		}
		GSM_SetInternalBuffer(gsm_InternalBuffer, 100);
		GSM_Status = GSM_HTTPRequest(GSM_POST, url, &gsm_Tx, NULL);
		CAN_Tx.id = GUI_DIAGNOSTIC_RQ_ID;
		CAN_Tx.format = CAN_STANDARD_FORMAT;
		CAN_Tx.type = CAN_DATA_FRAME;
		CAN_Tx.length = 2;
		CAN_Tx.data[0] = (uint8_t) (0xFF & GSM_Status);
		CAN_Tx.data[1] = (uint8_t) ((0xFF00 & GSM_Status) >> 8);
		CAN_Transmit(CAN1, &CAN_Tx);
		OS_HEAP_free(gsm_InternalBuffer);
		OS_HEAP_free(jsonfile);
		OS_HEAP_free(diagData);

	}
}

void Teset_TSK(void) {
	while (1) {
		GPIO_Write_Pin(GPIOA, GPIO_PIN_5, HIGH);
		OS_Delay(1000);
		GPIO_Write_Pin(GPIOA, GPIO_PIN_5, LOW);
		OS_Delay(1000);

	}
}


void CAN_SendData(uint8_t* buffer, uint16_t length, uint32_t CAN_id){
	CAN_msg msg = {0};
	msg.id = CAN_id;
	msg.format = CAN_STANDARD_FORMAT;
	msg.type = CAN_DATA_FRAME;
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
		CAN_Transmit(CAN1, &msg);
	}
}

void CAN_ReceiveStr(uint8_t *buffer, uint32_t msg_ID, uint16_t dataSize) {
	uint16_t currentDataSize = 0;
	while (currentDataSize < dataSize) {
		//wait for User Data
		while ((CAN_RXMsg.id != msg_ID) || (CAN_RXMsg.ActiveFlag == DISABLE))
			;
		CAN_RXMsg.ActiveFlag = DISABLE;
		for (uint8_t counter = 0; counter < CAN_RXMsg.length; counter++) {
			buffer[currentDataSize++] = CAN_RXMsg.data[counter];
		}
	}
	buffer[currentDataSize] = '\0';
}

/*
 *  Major: Represents significant updates, usually indicating backward-incompatible changes or major feature additions.
 Minor: Indicates smaller updates and new features that are backward-compatible with previous versions.
 Patch: Denotes bug fixes, patches, or small updates that do not introduce new features or break backward compatibility.
 0  1  2  3  4
 "Major.Minor.Patch"
 */

void ParseVersion(const uint8_t *version, uint8_t *major, uint8_t *minor,
		uint8_t *patch) {
	uint8_t versionCounter = 0;
	// Parse the current version components
	while (version[versionCounter] != '\0') {
		if (version[versionCounter] == '.') {
			versionCounter++;
			break;
		}
		*major = *major * 10 + (version[versionCounter] - '0');
		versionCounter++;
	}
	while (version[versionCounter] != '\0') {
		if (version[versionCounter] == '.') {
			versionCounter++;
			break;
		}
		*minor = *minor * 10 + (version[versionCounter] - '0');
		versionCounter++;
	}
	while (version[versionCounter] != '\0') {
		*patch = *patch * 10 + (version[versionCounter] - '0');
		versionCounter++;
	}
}

uint8_t CompareVersions(const uint8_t *currentVersion,
		const uint8_t *targetVersion) {
	uint8_t currentMajor = 0, currentMinor = 0, currentPatch = 0;
	uint8_t targetMajor = 0, targetMinor = 0, targetPatch = 0;

	ParseVersion(currentVersion, &currentMajor, &currentMinor, &currentPatch);
	ParseVersion(targetVersion, &targetMajor, &targetMinor, &targetPatch);

	if (currentMajor < targetMajor) {
		// Current version major component is smaller
		return VERSION_FULLUPDATE; // Update needed
	} else if (currentMajor > targetMajor) {
		// Current version major component is larger
		return VERSION_ROLLBACK; // No update needed
	} else {
		// Major components are the same, compare minor components
		if (currentMinor < targetMinor) {
			// Current version minor component is smaller
			return VERSION_NEWFEATURES; // Update needed
		} else if (currentMinor > targetMinor) {
			// Current version minor component is larger
			return VERSION_ROLLBACK; // No update needed
		} else {
			// Minor components are the same, compare patch components
			if (currentPatch < targetPatch) {
				// Current version patch component is smaller
				return VERSION_PATCH; // Update needed
			} else if (currentPatch > targetPatch) {
				// Current version patch component is larger
				return VERSION_ROLLBACK; // No update needed
			} else {
				// Versions are identical
				return VERSION_NOCHANGE; // No update needed
			}
		}
	}
}

void StrCat(uint8_t *buffer, uint8_t *str1, uint8_t *str2) {
	uint16_t str1Size = 0;
	while (str1[++str1Size] != '\0')
		;
	uint16_t str2Size = 0;
	while (str2[++str2Size] != '\0')
		;
	uint16_t counter;
	for (counter = 0; counter <= str1Size + str2Size; counter++) {
		if (counter < str1Size) {
			buffer[counter] = str1[counter];
		} else {
			buffer[counter] = str2[counter - str1Size];
		}
	}
	buffer[counter] = '\0';

}

uint8_t NumToSting(uint32_t num, uint8_t *buffer) {
	int counter = 0;
	do {
		buffer[counter++] = (num % 10) + '0';
		num /= 10;
	} while (num != 0);
	buffer[counter] = '\0';
	// Reverse the string in-place
	int j = 0, k = counter - 1;
	while (j < k) {
		char temp = buffer[j];
		buffer[j++] = buffer[k];
		buffer[k--] = temp;
	}
	return counter;
}

uint16_t StringToNum(uint8_t *String) {
	uint16_t sum = 0;
	uint8_t counter = 0;
	// Process each character of the string
	while (String[counter] != '\0') {
		// Convert character to digit
		int digit = String[counter] - '0';
		// Accumulate the number
		sum = sum * 10 + digit;
		counter++;
	}
	return sum;
}

/*
 * FIFO0
 * TODO: Write the logic and IM (Set events, ...)
 */


void CAN1_RX0_IRQHandler(void) {
	OS_INT_Enter()
	if (CAN1->RF0R & CAN_RF0R_FMP0) {
		CAN_Receive(CAN1, CAN_FIFO0, &CAN_RXMsg);
		//Authenticatation
		if (CAN_RXMsg.id == GUI_LOGIN_ID) {
			SEGGER_SYSVIEW_PrintfHost(
					"Authentication LOGIN TASKEVENT Setted from CAN1_RX0_IRQHandler ...");
			OS_TASKEVENT_Set(&CB_Authentication_TSK, TSK_ENENT_LOGIN);

		} else if (CAN_RXMsg.id == GUI_SIGNUP_RQ_ID) {
			OS_TASKEVENT_Set(&CB_Authentication_TSK, TSK_ENENT_SIGNUP);
			SEGGER_SYSVIEW_PrintfHost(
					"Authentication SIGNUP TASKEVENT Setted from CAN1_RX0_IRQHandler ...");
		}
		//Update
		else if (CAN_RXMsg.id == GUI_UPDATE_RQ_ID) {
			OS_TASKEVENT_Set(&CB_Update_TSK, TSK_EVENT_UPDATE);
			SEGGER_SYSVIEW_PrintfHost(
					"Update TASKEVENT Setted from CAN1_RX0_IRQHandler ...");
		} else if (CAN_RXMsg.id == GUI_CHECKUPDATE_RQ_ID) {
			SEGGER_SYSVIEW_PrintfHost(
					"Update TASKEVENT Setted from CAN1_RX0_IRQHandler ...");
			OS_TASKEVENT_Set(&CB_Update_TSK, TSK_EVENT_CHECKUPDATE);
		}
		//Diagnostics
		else if (CAN_RXMsg.id == GUI_DIAGNOSTIC_RQ_ID) {
			SEGGER_SYSVIEW_PrintfHost(
					"Diagnostic TASKEVENT Setted from CAN1_RX0_IRQHandler ...");
			OS_TASKEVENT_Set(&CB_Diagnostic_TSK, TSK_ENENT_SENDDIAGNOSTICS);
		}
	}
	OS_INT_Leave();

}

