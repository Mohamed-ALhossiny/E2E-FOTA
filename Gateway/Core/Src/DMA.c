/*
 * DMA.c
 *
 *  Created on: Feb 4, 2023
 *      Author: moham
 *  Note : We implement Direct Mode only
 */
#include "DMA.h"



fptr DMA_fptr[8] = {NULL};

void DMA_StreamConfig(DMA_Stream_TypeDef *DMA_Stream, DMA_Stream_Config StreamConfig){
	DMA_Stream->CR = 0x0;
	DMA_Stream->CR |= StreamConfig.TransferType;
	DMA_Stream->CR |= StreamConfig.ChannelPriority;
	DMA_Stream->CR |= StreamConfig.MDataSize;
	DMA_Stream->CR |= StreamConfig.PDataSize;
	DMA_Stream->CR |= StreamConfig.MemIncrement;
	DMA_Stream->CR |= StreamConfig.PerIncrement;
	DMA_Stream->CR |= StreamConfig.CirularMode;
	DMA_Stream->CR |= StreamConfig.ChannelNumber;
	//enable transfer complete interrupt
	DMA_Stream->CR |= DMA_SxCR_TCIE;
}

void DMA_Enable(DMA_Stream_TypeDef *DMA_Stream, uint32_t PerpheralAddress, uint32_t MemoryAddress, uint16_t amountOfData){
	DMA_Stream->NDTR = amountOfData;
	DMA_Stream->PAR  = PerpheralAddress;
	DMA_Stream->M0AR = MemoryAddress;
	DMA_Stream->M1AR = PerpheralAddress;
	DMA_Stream->CR  |= DMA_SxCR_EN;
}
void DMA_Disable(DMA_Stream_TypeDef *DMA_Stream){
	DMA_Stream->CR &= ~(DMA_SxCR_EN_Msk);
}
void DMA_Interrput_SelectCallBack(uint8_t DMA_StreamNumber, fptr ptr){
	 if(DMA_StreamNumber == DMA_Stream0){
		DMA_fptr[0] = ptr;
	}else if (DMA_StreamNumber == DMA_Stream1) {
		DMA_fptr[1] = ptr;
	}else if (DMA_StreamNumber == DMA_Stream2) {
		DMA_fptr[2] = ptr;
	}else if (DMA_StreamNumber == DMA_Stream3) {
		DMA_fptr[3] = ptr;
	}else if (DMA_StreamNumber == DMA_Stream4) {
		DMA_fptr[4] = ptr;
	}else if (DMA_StreamNumber == DMA_Stream5) {
		DMA_fptr[5] = ptr;
	}else if (DMA_StreamNumber == DMA_Stream6) {
		DMA_fptr[6] = ptr;
	}else if (DMA_StreamNumber == DMA_Stream7) {
		DMA_fptr[7] = ptr;
	}
}


void DMA1_Stream0_IRQHandler(void) {

	if (DMA1->LISR & DMA_LISR_TCIF0) {
		DMA_fptr[0]();
		DMA1->LIFCR |= DMA_LIFCR_CTCIF0;
	}

}
void DMA1_Stream1_IRQHandler(void) {

	if (DMA1->LISR & DMA_LISR_TCIF1) {
		DMA_fptr[1]();
		DMA1->LIFCR |= DMA_LIFCR_CTCIF1;
	}

}
void DMA1_Stream2_IRQHandler(void) {
	if (DMA1->LISR & DMA_LISR_TCIF2) {
		//DMA_fptr[2]();
		DMA1->LIFCR |= DMA_LIFCR_CTCIF2;
	}

}
void DMA1_Stream3_IRQHandler(void) {

	if (DMA1->LISR & DMA_LISR_TCIF3) {
		DMA_fptr[3]();
		DMA1->LIFCR |= DMA_LIFCR_CTCIF3;
	}

}
void DMA1_Stream4_IRQHandler(void) {
	if (DMA1->HISR & DMA_HISR_TCIF4) {
		DMA_fptr[4]();
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4;

	}

}
void DMA1_Stream5_IRQHandler(void) {

	if (DMA1->HISR & DMA_HISR_TCIF5) {
		DMA_fptr[5]();
		DMA1->HIFCR |= DMA_HIFCR_CTCIF5;
	}

}
void DMA1_Stream6_IRQHandler(void) {

	if (DMA1->HISR & DMA_HISR_TCIF6) {
		DMA_fptr[6]();
		DMA1->HIFCR |= DMA_HIFCR_CTCIF6;
	}

}
void DMA1_Stream7_IRQHandler(void) {

	if (DMA1->HISR & DMA_HISR_TCIF7) {
		DMA_fptr[7]();
		DMA1->HIFCR |= DMA_HIFCR_CTCIF7;
	}

}




