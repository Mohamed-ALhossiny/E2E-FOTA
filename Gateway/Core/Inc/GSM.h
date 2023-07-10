/*
 * GSM.h
 *
 *  Created on: Feb 25, 2023
 *      Author: moham
 */

#ifndef INC_GSM_H_
#define INC_GSM_H_
#include "STD_TYPE.h"
#include "GSM_Config.h"
#include "DMA.h"
#include "UART.h"
#include "TIM.h"
#include "main.h"
#include "stdlib.h"
#include "RTOS.h"

typedef enum
{
  GSM_NOK= 0U,
  GSM_OK = !GSM_NOK
} GSMRXStatus;

typedef enum
{
  STATE_OK= 200U,
  STATE_MOVED = 301,
  STATE_FOUND = 302,
  STATE_TEMP_REDIRECT = 307,
  STATE_BAD_REQUEST = 400,
  STATE_UNAUTHORIZED = 401,
  STATE_FORBIDDEN = 403,
  STATE_NOT_FOUND = 404,
  STATE_REQUEST_OUT = 408,
  STATE_BAD_GATEWAY = 502,
  STATE_GATEWAY_TIMEOUT = 504,
  STATE_NETWORK_ERROR = 601,
  STATE_NO_MEMORY = 602,
  STATE_DNS_ERROR = 603,
  STATE_STACK_BUSY = 604
} SERVERRESPONSE;

typedef struct
{
	uint8_t* 	payload;
	uint32_t 	payloadSize;
	uint8_t*	header;
	uint8_t*	contentType;
} GSM_TxType;


#define GSM_GET				0x0
#define GSM_POST			0x1

void GSM_SetInternalBuffer(uint8_t* internalBuffer, uint16_t internalBufferSize);
uint8_t GSM_INIT(void);
void GSM_DMA_SendCommand(uint8_t* buffer ,const uint8_t *command, uint8_t crlfToWait);
uint16_t GSM_HTTPRequest(uint8_t RequestType, const uint8_t *url,
		GSM_TxType* TxData, uint16_t *RxPayloadSize);
void GSM_DownloadData( uint8_t *RxPayload, uint8_t *RxHeader);
uint32_t GSM_GetReceivedDataSize(void);

uint8_t GSM_IsRegistered(void);
uint8_t parse_json(uint8_t* json, uint8_t* pKey, uint8_t* pValue);
uint8_t strSearch(uint8_t *string, const uint8_t *subString,
		uint8_t numberOfSubstring, uint32_t *lastSubstringIndex);



#endif /* INC_GSM_H_ */
