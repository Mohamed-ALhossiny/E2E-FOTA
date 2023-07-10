/*
 * DMA_Config.h
 *
 *  Created on: Feb 5, 2023
 *      Author: moham
 */

#ifndef INC_DMA_CONFIG_H_
#define INC_DMA_CONFIG_H_

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
}DMA_Channel_Config;

/**************************Channel Configurations**************************/
//Transfer Direction
#define DMA_CHANNEL_MEMORY_TO_MEMORY							DMA_CCR_MEM2MEM
#define DMA_CHANNEL_PERIPHERAL_TO_MEMORY							0x0
#define DMA_CHANNEL_MEMORY_TO_PERIPHERAL						DMA_CCR_DIR
//Channel Priority level
#define DMA_CHANNEL_LOW												0x0
#define DMA_CHANNEL_MEDIUM										DMA_CCR_PL_0
#define DMA_CHANNEL_HIGH										DMA_CCR_PL_1
#define DMA_CHANNEL_VERYHIGH									DMA_CCR_PL
//Data Width
#define DMA_CHANNEL_8BITS											0x0
#define DMA_CHANNEL_16BITS										DMA_CCR_MSIZE_0
#define DMA_CHANNEL_32BITS										DMA_CCR_MSIZE_1
//Increment for Peripheral or Memory
#define DMA_CHANNEL_MEMORY_INC_ENABLE							DMA_CCR_MINC
#define DMA_CHANNEL_MEMORY_INC_DIABLE								0x0
#define DMA_CHANNEL_PERIPHERAL_INC_ENABLE						DMA_CCR_PINC
#define DMA_CHANNEL_PERIPHERAL_INC_DIABLE							0x0
//Circular mode
#define DMA_CHANNEL_CIRCULAR_ON									DMA_CCR_CIRC
#define DMA_CHANNEL_CIRCULAR_OFF									0x0




#endif /* INC_DMA_CONFIG_H_ */
