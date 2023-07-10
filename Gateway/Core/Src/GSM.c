/*
 * GSM.c
 *
 *  Created on: Feb 25, 2023
 *      Author: moham
 */
#include "GSM.h"
//#warning "Debug the buffer changes"
static void GSM_GSMWait(uint8_t *buffer, uint8_t crlfToWait);
static void GSM_FlushBuffer(uint8_t *buffer);
uint8_t strSearch(uint8_t *string, const uint8_t *subString,
		uint8_t numberOfSubstring, uint32_t *lastSubstringIndex);
static uint8_t GSM_ExtractData(uint8_t *inputString,
		const uint8_t *startIdentifier, const uint8_t *endIdentifier,
		uint8_t *extractedData);
static uint8_t NumToSting(uint32_t num, uint8_t *buffer);
static void GSM_DMATx_InterruptFun(void);


uint8_t* GSM_InternalBuffer = NULL;
volatile uint8_t GSM_TX_Completeflag = 0;



void GSM_SetInternalBuffer(uint8_t* internalBuffer, uint16_t internalBufferSize){
	uint32_t counter = 0;
	GSM_InternalBuffer = internalBuffer;
	while(counter < internalBufferSize){
		GSM_InternalBuffer[counter++] = '\0';
	}
}

uint8_t GSM_INIT(void) {
	uint8_t response = GSM_NOK;
	uint8_t GSM_Buffer[300];
	uint16_t counter = 0;
	while(counter < 300){
		GSM_Buffer[counter++] = '\0';
	}
	DMA_Interrput_SelectCallBack(DMA_Stream4, GSM_DMATx_InterruptFun);
	//COMMAND ECHO MODE OFF
	while (response != GSM_OK) {
		GSM_DMA_SendCommand(GSM_Buffer, (const uint8_t*) "AT\n", 2);
		response = strSearch(GSM_Buffer, (const uint8_t*) "OK", 1, NULL);
		TIM_Delay(200);
	}
	response = GSM_NOK;
	GSM_DMA_SendCommand(GSM_Buffer,
			(const uint8_t*) "AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n", 2);
	GSM_DMA_SendCommand(GSM_Buffer,(const uint8_t*) "AT+CSTT=\"internet.vodafone.net\",\"internet\",\"internet\"\n",2); //vodafone
	//GSM_DMA_SendCommand(GSM_Buffer, (const uint8_t*) "AT+CSTT=\"internet.te.eg\"\n", 2);//WE
	//GSM_DMA_SendCommand(GSM_Buffer, (const uint8_t*) "AT+CSTT=\"mobinilweb\"\n", 2);//Orange
	GSM_DMA_SendCommand(GSM_Buffer, (const uint8_t*) "AT+SAPBR=1,1\n", 2);
	response = strSearch(GSM_Buffer, (const uint8_t*) "OK", 1, NULL);
	while (response != GSM_OK) {
		GSM_DMA_SendCommand(GSM_Buffer, (const uint8_t*) "AT+SAPBR=0,1\n", 2);
		TIM_Delay(100);
		GSM_DMA_SendCommand(GSM_Buffer, (const uint8_t*) "AT+SAPBR=1,1\n", 2);
		response = strSearch(GSM_Buffer, (const uint8_t*) "OK", 1, NULL);
	}
	response = GSM_NOK;
	//setup HTTP Protocol
	GSM_DMA_SendCommand(GSM_Buffer, (const uint8_t*) "AT+HTTPINIT\n", 2);
	GSM_DMA_SendCommand(GSM_Buffer, (const uint8_t*) "AT+HTTPSSL=1\n", 2);
	GSM_DMA_SendCommand(GSM_Buffer, (const uint8_t*) "AT+HTTPPARA=\"CID\",1\n",
			2);
	GSM_DMA_SendCommand(GSM_Buffer,
			(const uint8_t*) "AT+HTTPPARA=\"REDIR\",1\n", 2);
	GSM_FlushBuffer(GSM_Buffer);
	return response;
}

/*
 * para : Crfl : min(2)
 * CR = Carriage Return (\r, 0x0D in hexadecimal, 13 in decimal) — moves the cursor to the beginning of the line without advancing to the next line.
 LF = Line Feed (\n, 0x0A in hexadecimal, 10 in decimal) — moves the cursor down to the next line without returning to the beginning of the line.
 * as the Crfl depends on the command
 */
void GSM_DMA_SendCommand(uint8_t *buffer, const uint8_t *command,
		uint8_t crlfToWait) {
	uint16_t datasize = 0;
	GSM_FlushBuffer(buffer);
	DMA_Enable(GSM_RX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
			(uint32_t) buffer, DMA_NDTR_MAX);
	while (command[++datasize] != '\0')
		;
	DMA_Enable(GSM_TX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
			(uint32_t) command, datasize);
	TIM_Delay(10); //just for debugging (-_-) , GSM_TX_Completeflag is not updating while debugging
	while (GSM_TX_Completeflag != 1)
			;
	GSM_TX_Completeflag = 0;
	GSM_GSMWait(buffer, crlfToWait);
}

uint8_t GSM_IsRegistered(void) {

	GSM_DMA_SendCommand(GSM_InternalBuffer, (const uint8_t*) "AT+CGREG?\n", 2);
	uint8_t state = strSearch(GSM_InternalBuffer, (const uint8_t*) ",1", 1,
			NULL);
	GSM_FlushBuffer(GSM_InternalBuffer);
	return state;
}

/*
 * @State: Done
 */

uint16_t GSM_HTTPRequest(uint8_t RequestType, const uint8_t *url,
		GSM_TxType* TxData, uint16_t *RxPayloadSize) {
	uint16_t urlSize = 0;
	uint16_t contentTypeSize = 0;
	uint16_t headerSize = 0;
	uint8_t TxPayloadSize[16];
	uint8_t TxPayloadSizeStringSize = 0;
	GSM_FlushBuffer(GSM_InternalBuffer);
	if (TxData->contentType != NULL) {
		DMA_Enable(GSM_RX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) GSM_InternalBuffer, DMA_NDTR_MAX);
		UART_Send_String(UART4, (uint8_t*)"AT+HTTPPARA=\"CONTENT\",\"");

		while (TxData->contentType[++contentTypeSize] != '\0')
			;
		DMA_Enable(GSM_TX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) TxData->contentType, contentTypeSize);
		TIM_Delay(10);
		while (GSM_TX_Completeflag != 1)
			;
		GSM_TX_Completeflag = 0;
		UART_Send_String(UART4, (uint8_t*)"\"\n");
		GSM_GSMWait(GSM_InternalBuffer, 2);
		GSM_FlushBuffer(GSM_InternalBuffer);
	}

	if (TxData->header != NULL) {
		DMA_Enable(GSM_RX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) GSM_InternalBuffer, DMA_NDTR_MAX);
		UART_Send_String(UART4, (uint8_t*)"AT+HTTPPARA=\"USERDATA\",\"");

		while (TxData->header[++headerSize] != '\0')
			;
		DMA_Enable(GSM_TX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) TxData->header, headerSize);
		TIM_Delay(10);
		while (GSM_TX_Completeflag != 1)
			;
		GSM_TX_Completeflag = 0;
		UART_Send_String(UART4, (uint8_t*)"\"\n");
		GSM_GSMWait(GSM_InternalBuffer, 2);
		GSM_FlushBuffer(GSM_InternalBuffer);
	}

	//adding Tx payload -----------
	if (TxData->payload != NULL) {
		DMA_Enable(GSM_RX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) GSM_InternalBuffer, DMA_NDTR_MAX);
		UART_Send_String(UART4, (uint8_t*)"AT+HTTPDATA=");

		TxPayloadSizeStringSize = NumToSting(TxData->payloadSize, TxPayloadSize);
		DMA_Enable(GSM_TX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) TxPayloadSize, TxPayloadSizeStringSize);
		TIM_Delay(10);
		while (GSM_TX_Completeflag != 1)
			;
		GSM_TX_Completeflag = 0;
		UART_Send_String(UART4, (uint8_t*)",50000\n");

		TIM_Delay(10);
		GSM_GSMWait(GSM_InternalBuffer, 2);
		GSM_FlushBuffer(GSM_InternalBuffer);
		DMA_Enable(GSM_RX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) GSM_InternalBuffer, DMA_NDTR_MAX);
		DMA_Enable(GSM_TX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) TxData->payload, TxData->payloadSize);
		TIM_Delay(10);
		while (GSM_TX_Completeflag != 1)
					;
		GSM_TX_Completeflag = 0;
		GSM_GSMWait(GSM_InternalBuffer, 2);
		GSM_FlushBuffer(GSM_InternalBuffer);
	}

	//------

	DMA_Enable(GSM_RX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
			(uint32_t) GSM_InternalBuffer, DMA_NDTR_MAX);
	UART_Send_String(UART4, (uint8_t*)"AT+HTTPPARA=\"URL\",\"");

	while (url[++urlSize] != '\0')
		;
	DMA_Enable(GSM_TX_DMA_STREAM, (uint32_t) &(GSM_UART->DR), (uint32_t) url,
			urlSize);
	TIM_Delay(10);
	while (GSM_TX_Completeflag != 1)
		;
	GSM_TX_Completeflag = 0;
	UART_Send_String(UART4, (uint8_t*)"\"\n");
	GSM_GSMWait(GSM_InternalBuffer, 2);
	if (RequestType == GSM_GET) {
		GSM_DMA_SendCommand(GSM_InternalBuffer,
				(const uint8_t*) "AT+HTTPACTION=0\n", 4);
	} else if (RequestType == GSM_POST) {
		GSM_DMA_SendCommand(GSM_InternalBuffer,
				(const uint8_t*) "AT+HTTPACTION=1\n", 4);
	} else {
		//NOP
	}
	if(RxPayloadSize != NULL){
		*RxPayloadSize = GSM_GetReceivedDataSize();
	}

	uint32_t stateCodeStart = 0;
	uint16_t stateCode;
	strSearch(GSM_InternalBuffer, (const uint8_t*) ",", 1, &stateCodeStart);
	stateCode = (GSM_InternalBuffer[stateCodeStart + 1] - 48) * 100;
	stateCode += (GSM_InternalBuffer[stateCodeStart + 2] - 48) * 10;
	stateCode += GSM_InternalBuffer[stateCodeStart + 3] - 48;
	GSM_FlushBuffer(GSM_InternalBuffer);
	return stateCode;
}

void GSM_DownloadData( uint8_t *RxPayload, uint8_t *RxHeader){

	GSM_DMA_SendCommand(GSM_InternalBuffer, (const uint8_t*)"AT+HTTPREAD\n", 4);

	GSM_ExtractData(GSM_InternalBuffer, (const uint8_t*) "\r\n",
			(const uint8_t*) "\r\nOK\r\n", RxPayload);
	if (RxHeader != NULL) {
		GSM_FlushBuffer(GSM_InternalBuffer);
		DMA_Enable(GSM_RX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) GSM_InternalBuffer, DMA_NDTR_MAX);
		DMA_Enable(GSM_TX_DMA_STREAM, (uint32_t) &(GSM_UART->DR),
				(uint32_t) "AT+HTTPHEAD\n", 12);
		TIM_Delay(10);
		while (GSM_TX_Completeflag != 1);
		GSM_TX_Completeflag = 0;
		while (strSearch(GSM_InternalBuffer, (const uint8_t*) "\r\nOK\r\n", 1,
				NULL) != GSM_OK);
		DMA_Disable(GSM_RX_DMA_STREAM);
		GSM_ExtractData(GSM_InternalBuffer, (const uint8_t*) "\r\n",
				(const uint8_t*) "\r\nOK\r\n", RxHeader);
	}
}

/*
 * wait till  number of specified CRLF is received
 */

static void GSM_GSMWait(uint8_t *buffer, uint8_t crlfToWait) {

	while (strSearch(buffer, (const uint8_t*) "\r\n", crlfToWait, NULL)
			!= GSM_OK)
		;
	DMA_Disable(GSM_RX_DMA_STREAM);
}

/*
 * used after HTTPACTION Command to get ReceivedDataSize
 */
uint32_t GSM_GetReceivedDataSize(void) {
	uint32_t sum = 0;
	for (uint8_t counter = 0; GSM_InternalBuffer[counter] != '\0'; counter++) {
		if (GSM_InternalBuffer[counter] >= '0'
				&& GSM_InternalBuffer[counter] <= '9') {
			sum = sum * 10 + GSM_InternalBuffer[counter] - '0';
		}
		if (GSM_InternalBuffer[counter] == ',') {
			sum = 0;
		}
	}
	return sum;
}

/*
 * @work : extract data from GSM buffer
 * @para : inputString     ==> input received data from GSM,
 * 		  startIdentifier ==> start of our data,
 * 		  endIdentifier   ==> end of our data,
 * 		  extractedData   ==> output data buffer
 */
static uint8_t GSM_ExtractData(uint8_t *inputString,
		const uint8_t *startIdentifier, const uint8_t *endIdentifier,
		uint8_t *extractedData) {
	uint32_t startOfData = 0;
	uint32_t endOfData = 0;

	if (strSearch(inputString, startIdentifier, 2, &startOfData) != GSM_OK) {
		return GSM_NOK;
	}
	if ((strSearch(inputString, endIdentifier, 1, &endOfData) != GSM_OK)) {
		return GSM_NOK;
	}

	uint16_t startIdentifierSize = 0;
	while (startIdentifier[++startIdentifierSize] != '\0')
		;
	uint16_t endIdentifierSize = 0;
	while (endIdentifier[++endIdentifierSize] != '\0')
		;

	uint32_t strCounter = startOfData + startIdentifierSize;
	uint32_t dataSize = endOfData - startOfData - startIdentifierSize;
	for (uint32_t dataCounter = 0; dataCounter < dataSize; dataCounter++) {
		extractedData[dataCounter] = inputString[strCounter++];
	}
	return GSM_OK;
}




uint8_t parse_json(uint8_t* json, uint8_t* pKey, uint8_t* pValue) {
    uint8_t keySize = 0;
    uint8_t keycounter = 0;
    uint16_t qottNum = 1;
    uint16_t nextQottPosition = 0;
    uint8_t matchingFlag = 0;
    if(strSearch(json, pKey, 1, NULL) != 1){
        return 0;
    }
    while(pKey[++keySize] != '\0');
    if(json[0] == '{') {
        while(1) {
            strSearch(json, (const uint8_t *)"\"", qottNum+=2, (uint32_t*)&nextQottPosition);
            for(keycounter = 0;keycounter <keySize;keycounter++){
                if(json[nextQottPosition + 1 + keycounter] == pKey[keycounter]){
                    matchingFlag = 1;
                }else{
                    matchingFlag = 0;
                    break;
                }
            }
            if(matchingFlag == 1){
                uint16_t counter = nextQottPosition + keySize + 2;
                uint16_t valueCounter = 0;
                while(json[counter++] != '\"');
                while(json[counter] != '\"'){
                    pValue[valueCounter++] = json[counter++];
                }
                pValue[valueCounter++] = '\0';
                return 1;


            }

        }
    } else {
        return 0;
    }
}

 uint8_t strSearch(uint8_t *string, const uint8_t *subString,
		uint8_t numberOfSubstring, uint32_t *lastSubstringIndex) {
	uint8_t matchingFlag = 0;
	uint8_t foundSubStringCounter = 0;
	uint16_t strStart = 0;
	for (uint16_t strCounter = 0; string[strCounter] != '\0'; strCounter++) {
		strStart = strCounter;
		for (uint16_t subCounter = 0; subString[subCounter] != '\0';
				subCounter++) {
			if (string[strStart] == subString[subCounter]
					&& string[strCounter] != '\0') {
				matchingFlag = 1;
				strStart++;
			} else if (string[strStart] != subString[subCounter]) {
				matchingFlag = 0;
				break;
			}
		}
		foundSubStringCounter += matchingFlag;
		if (foundSubStringCounter == numberOfSubstring) {
			if (lastSubstringIndex != NULL) {
				*lastSubstringIndex = strCounter;
			}
			return GSM_OK;
		}
	}
	return GSM_NOK;
}

static uint8_t NumToSting(uint32_t num, uint8_t *buffer) {
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

static void GSM_FlushBuffer(uint8_t *buffer) {
	uint16_t counter = 0;
	while (buffer[counter] != '\0') {
		buffer[counter++] = '\0';
	}
}

static void GSM_DMATx_InterruptFun(void) {
	GSM_TX_Completeflag = 1;
}

