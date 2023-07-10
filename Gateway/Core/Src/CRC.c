/*
 * CRC.c
 *
 *  Created on: Feb 11, 2023
 *      Author: moham
 */
#include "CRC.h"




uint32_t CRC_Clac(uint32_t* data, uint16_t size){
	uint16_t counter = 0;
	uint32_t result;
	CRC->CR |= 0x01;
	while(counter < size){
		CRC->DR = data[counter];
		counter++;
	}
	result = CRC->DR ;
	CRC->CR |= 0x01;
	return result;
}
