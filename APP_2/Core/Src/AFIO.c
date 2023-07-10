/*
 * AFIO.c
 *
 *  Created on: Jan 30, 2023
 *      Author: moham
 */
#include "RCC.h"
#include "AFIO.h"


void AFIO_INIT(void){
	//no remap of CAN
	AFIO->MAPR &= ~(AFIO_MAPR_CAN_REMAP_Msk);
	//No remap of USART3
	AFIO->MAPR &= ~(AFIO_MAPR_USART3_REMAP_Msk);
}
