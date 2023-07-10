/*
 * DMA_Config.h
 *
 *  Created on: Feb 5, 2023
 *      Author: moham
 */

#ifndef INC_DMA_CONFIG_H_
#define INC_DMA_CONFIG_H_
/*
 * Note: FIFO mode not implemented
 * */

typedef struct
{
	//TODO:adding interrupt options
	uint32_t TransferType; //(mem to periph) or (periph to mem) or (mem to mem)
	uint32_t ChannelPriority; //Channel priority
	uint32_t MDataSize; //Memory data width
	uint32_t PDataSize; //Peripheral data width
	uint32_t MemIncrement; //Memory increment mode
	uint32_t PerIncrement; //Peripheral increment mode
	uint32_t CirularMode; //enable or disable Circular mode (must disable in mem to mem)
	uint32_t ChannelNumber;

}DMA_Stream_Config;

/**************************Stream Configurations**************************/
#define DMA_Stream0        					   	 		0
#define DMA_Stream1       						 		1
#define DMA_Stream2       						 		2
#define DMA_Stream3       						 		3
#define DMA_Stream4       						 		4
#define DMA_Stream5        						 		5
#define DMA_Stream6        						 		6
#define DMA_Stream7        						 		7

#define DMA_Channel0        					   	 0 << 25
#define DMA_Channel1       							 1 << 25
#define DMA_Channel2       							 2 << 25
#define DMA_Channel3       							 3 << 25
#define DMA_Channel4       						 	 4 << 25
#define DMA_Channel5        						 5 << 25
#define DMA_Channel6        						 6 << 25
#define DMA_Channel7        						 7 << 25

//Transfer Direction
#define DMA_CHANNEL_MEMORY_TO_MEMORY							DMA_SxCR_DIR_1
#define DMA_CHANNEL_PERIPHERAL_TO_MEMORY							0x0
#define DMA_CHANNEL_MEMORY_TO_PERIPHERAL						DMA_SxCR_DIR_0
//Channel Priority level
#define DMA_CHANNEL_LOW												0x0
#define DMA_CHANNEL_MEDIUM										DMA_SxCR_PL_0
#define DMA_CHANNEL_HIGH										DMA_SxCR_PL_1
#define DMA_CHANNEL_VERYHIGH									DMA_SxCR_PL
//Data Width
#define DMA_CHANNEL_8BITS											0x0
#define DMA_CHANNEL_16BITS										DMA_SxCR_MSIZE_0
#define DMA_CHANNEL_32BITS										DMA_SxCR_MSIZE_1
//Increment for Peripheral or Memory
#define DMA_CHANNEL_MEMORY_INC_ENABLE							DMA_SxCR_MINC
#define DMA_CHANNEL_MEMORY_INC_DIABLE								0x0
#define DMA_CHANNEL_PERIPHERAL_INC_ENABLE						DMA_SxCR_PINC
#define DMA_CHANNEL_PERIPHERAL_INC_DIABLE							0x0
//Circular mode
#define DMA_CHANNEL_CIRCULAR_ON									DMA_SxCR_CIRC
#define DMA_CHANNEL_CIRCULAR_OFF									0x0
//peripheral flow controller
#define DMA_CHANNEL_CONTROL_BY_DMA									0x0
#define DMA_CHANNEL_CONTROL_BY_PERIPHERAL						DMA_SxCR_PFCTRL















#endif /* INC_DMA_CONFIG_H_ */
