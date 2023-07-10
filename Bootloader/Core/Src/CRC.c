/*
 * CRC.c
 *
 *  Created on: Feb 7, 2023
 *      Author: moham
 */

#include "CRC.h"

uint32_t CRC_CheckSum ;
uint8_t CRC_Complete = 0;
uint32_t CRC_Calc(uint32_t *data, uint16_t size){
	uint16_t counter = 0;
	CRC->CR |= 0x01; //reset CR register
	while(counter < size)
		CRC->DR = data[counter++];
	CRC_CheckSum = CRC->DR;
	CRC->CR |= 0x01;
	return CRC_CheckSum;
}
//Using any available DMA channel
uint32_t* CRC_Calc_DMA(DMA_Channel_TypeDef *DMA_Channel, uint32_t *data, uint16_t size){
	DMA_Channel_Config ChannelConfig ={
			DMA_CHANNEL_MEMORY_TO_MEMORY,
			DMA_CHANNEL_VERYHIGH,
			DMA_CHANNEL_32BITS,
			DMA_CHANNEL_32BITS,
			DMA_CHANNEL_MEMORY_INC_DIABLE,
			DMA_CHANNEL_PERIPHERAL_INC_ENABLE,
			DMA_CHANNEL_CIRCULAR_OFF
	};
	DMA_ChannelConfig(DMA_Channel, ChannelConfig);
	/*
	 *  4. Enable the DMA transfer complete interrupt./
		5. Configure the DMA NVIC IRQ./
		6. Enable the DMA channel/stream./
		7. Wait for the DMA transfer complete to occur./
		8. Disable the DMA channel. In the case of DMA with stream architecture, the controller
			disables automatically the channel when the transfer ends, while in the other case, the
			NVIC DMA_IRQ routine must disable the channel for further use.
		9. Clear DMA IT pending bit.
	 */
	DMA_Enable(DMA_Channel, (uint32_t)data, (uint32_t)&(CRC->DR), size);
	//TODO:configure interrupt(must inside it first inactive Systick interrupt ) and set call back function
	//in callback func allocate the result
	DMA_Interrput_SelectCallBack(DMA_Channel, setFlag);
	return &CRC_CheckSum;
}
void setFlag(){
	CRC_Complete = 1;
	CRC_CheckSum = CRC->DR;
	CRC->CR |= 0x01;
	DMA_Disble(DMA_Channel2);
}

