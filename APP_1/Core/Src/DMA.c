/*
 * DMA.c
 *
 *  Created on: Feb 4, 2023
 *      Author: moham
 */
#include "DMA.h"



fptr DMA_fptr[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

void DMA_ChannelConfig(DMA_Channel_TypeDef *DMA_Channel, DMA_Channel_Config ChannelConfig){
	DMA_Channel2->CCR = 0x0;
	DMA_Channel->CCR |= ChannelConfig.TransferType;
	DMA_Channel->CCR |= ChannelConfig.ChannelPriority;
	DMA_Channel->CCR |= ChannelConfig.MDataSize;
	DMA_Channel->CCR |= ChannelConfig.PDataSize;
	DMA_Channel->CCR |= ChannelConfig.MemIncrement;
	DMA_Channel->CCR |= ChannelConfig.PerIncrement;
	DMA_Channel->CCR |= ChannelConfig.CirularMode;
	DMA_Channel->CCR |= DMA_CCR_TCIE;
}

void DMA_Enable(DMA_Channel_TypeDef *DMA_Channel, uint32_t PerpheralAddress, uint32_t MemoryAddress, uint16_t amountOfData){
	DMA_Channel->CNDTR = amountOfData;
	DMA_Channel->CPAR = PerpheralAddress;
	DMA_Channel->CMAR = MemoryAddress;
	DMA_Channel->CCR |= DMA_CCR_EN;
}
void DMA_Disble(DMA_Channel_TypeDef *DMA_Channel){
	DMA_Channel->CCR &= ~(DMA_CCR_EN_Msk);
}
void DMA_Interrput_SelectCallBack(DMA_Channel_TypeDef *DMA_Channel, fptr ptr){
	 if(DMA_Channel == DMA_Channel1){
		DMA_fptr[0] = ptr;
	}else if (DMA_Channel == DMA_Channel2) {
		DMA_fptr[1] = ptr;
	}else if (DMA_Channel == DMA_Channel3) {
		DMA_fptr[2] = ptr;
	}else if (DMA_Channel == DMA_Channel4) {
		DMA_fptr[3] = ptr;
	}else if (DMA_Channel == DMA_Channel5) {
		DMA_fptr[4] = ptr;
	}else if (DMA_Channel == DMA_Channel6) {
		DMA_fptr[5] = ptr;
	}else if (DMA_Channel == DMA_Channel7) {
		DMA_fptr[6] = ptr;
	}
}







