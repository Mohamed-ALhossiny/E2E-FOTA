/*
 * CAN1.c
 *
 *  Created on: Jan 31, 2023
 *      Author: ashra
 */

/*----------------------------------------include----------------------------------------- */

#include "CAN.h"

/* ------------------------------- Configuration for CAN ---------------------------------*/
/*
 * */

void CAN_Config(CAN_TypeDef *CANx, CAN_setupTypeDef *CANsetupStruct) {
	/* Initialization request */
	CANx->MCR |= CAN_MCR_INRQ;

	/*Request to exit Sleep mode */
	CANx->MCR &= ~CAN_MCR_SLEEP_Msk;

	/* Wait for initialization mode */
	while (!(CAN_MSR_INAK & CANx->MSR))
		;

	/******************* SET MCR register configuration**************************/

	/* Set the time triggered communication mode */
	if (CANsetupStruct->TTCM == ENABLE) {
		CANx->MCR |= CAN_MCR_TTCM;
	} else {
		CANx->MCR &= ~(CAN_MCR_TTCM_Msk);
	}
	/* Set the automatic bus-off management */
	if (CANsetupStruct->ABOM == ENABLE) {
		CANx->MCR |= CAN_MCR_ABOM;
	} else {
		CANx->MCR &= ~(CAN_MCR_ABOM_Msk);
	}

	/* Set the automatic wake-up mode */
	if (CANsetupStruct->AWUM == ENABLE) {
		CANx->MCR |= CAN_MCR_AWUM;
	} else {
		CANx->MCR &= ~(CAN_MCR_AWUM_Msk);
	}

	/* Set the no automatic retransmission */
	if (CANsetupStruct->NART == ENABLE) {
		CANx->MCR |= CAN_MCR_NART;
	} else {
		CANx->MCR &= ~(CAN_MCR_NART_Msk);
	}

	/* Set the transmit FIFO priority */
	if (CANsetupStruct->TXFP == ENABLE) {
		CANx->MCR &= ~(CAN_MCR_TXFP_Msk);
	} else {
		CANx->MCR |= CAN_MCR_TXFP;
	}
	if (CANsetupStruct->RFLM == ENABLE) {
		//FIFO Overload Lock Mode
		CANx->MCR |= CAN_MCR_RFLM;
	} else {
		CANx->MCR &= ~(CAN_MCR_RFLM_Msk);
	}

	/*
	 * set BTR register so that sample point is at about 87.5 % bit time from bit start
	 * BRP[9:0] = 4, TS1[3:0] = 6, TS2[2:0] = 0, SJW[1:0] = 0
	 * tq = (BRP[9:0] + 1) x tPCLK = 5 * 1 / 45 MHz = 0.111 micro sec
	 * tBS1 = tq * (TS1[3:0] + 1)  = 0.111 * (6+1)  = 0.777 micro sec
	 * tBS2 = tq * (TS2[2:0] + 1)  = 0.111 * (0+1)  = 0.111 micro sec
	 * NominalBitTime = 1 * tq + tBS1 + tBS2 = 0.111 + 0.777 + 0.111 = 0.999 micro sec
	 * Boudrate = 1 / NominalBitTime = 1 Mbps
	 *
	 * */
	CANx->BTR = CAN_BTR;
	//set all filters to our can (other can must deactivate)
	//CANx->FMR &= ~(CAN_FMR_CAN2SB_Msk);

}

/*
 * ----------------------------------- CAN set test mode ---------------------------------
 */
void CAN_Testmode(CAN_TypeDef *CANx, uint32_t testmode) {

	CANx->BTR &= ~(CAN_BTR_SILM_Msk | CAN_BTR_LBKM_Msk);     // set test mode

	CANx->BTR |= testmode;

}

/*
 * ---------------------------------- CAN set normal mode ---------------------------------
 */
void CAN_Normal(CAN_TypeDef *CANx) {

	CANx->MCR &= ~(CAN_MCR_INRQ_Msk | CAN_MCR_SLEEP_Msk); //normal operating mode, reset(INRQ,SLEEP)

	while (CANx->MSR & CAN_MSR_INAK)
		;

}

/*
 * note : para MailboxNumber Can be :TX_MAILBOX_0, TX_MAILBOX_1, TX_MAILBOX_2
 *------------------------------------ CAN Transmission ----------------------------------
 */
void CAN_Transmit(CAN_TypeDef *CANx, CAN_msg *msg) {
	/* check if transmission mailbox is empty */
	uint8_t mailBoxIndex = 10;
	while (mailBoxIndex == 10){
		if(CANx->TSR & CAN_TSR_TME0){
			mailBoxIndex = CAN_TSR_TME0 >> 27;
		}else if(CANx->TSR & CAN_TSR_TME1){
			mailBoxIndex = CAN_TSR_TME1 >> 27;
		}else if (CANx->TSR & CAN_TSR_TME2) {
			mailBoxIndex = CAN_TSR_TME2 >> 27;
		}
	}
	 // ------ check if transmission mailbox  is empty ------------------------------------

	CANx->sTxMailBox[mailBoxIndex].TIR = (uint32_t) 0; // Reset TIR register

	// Setup identifier information   ?<==================
	if (msg->format == CAN_STANDARD_FORMAT) {
		CANx->sTxMailBox[mailBoxIndex].TIR |= (uint32_t) (msg->id << 21);
	} else {
		// Extended ID
		CANx->sTxMailBox[mailBoxIndex].TIR |= (uint32_t) (msg->id << 3)
				| CAN_TI0R_IDE;
	}

	// Setup type information
	if (msg->type == CAN_DATA_FRAME) {
		// DATA FRAME
		/*reset transmit data registers*/
		CANx->sTxMailBox[mailBoxIndex].TDLR = 0;
		CANx->sTxMailBox[mailBoxIndex].TDHR = 0;

		CANx->sTxMailBox[mailBoxIndex].TDLR = (((uint32_t) msg->data[3] << 24)
				| ((uint32_t) msg->data[2] << 16)
				| ((uint32_t) msg->data[1] << 8) | ((uint32_t) msg->data[0]));

		CANx->sTxMailBox[mailBoxIndex].TDHR = (((uint32_t) msg->data[7] << 24)
				| ((uint32_t) msg->data[6] << 16)
				| ((uint32_t) msg->data[5] << 8) | ((uint32_t) msg->data[4]));
	} else {
		// REMOTE FRAME
		CANx->sTxMailBox[mailBoxIndex].TIR |= (CAN_TI0R_RTR);
	}

	// Setup Can frame Data length Code
	CANx->sTxMailBox[mailBoxIndex].TDTR &= ~CAN_TDT0R_DLC;
	CANx->sTxMailBox[mailBoxIndex].TDTR |= (msg->length & CAN_TDT0R_DLC);

	/* Transmit Mailbox Request */
	CANx->sTxMailBox[mailBoxIndex].TIR |= CAN_TI0R_TXRQ;
}

uint32_t CAN_GetRxId(CAN_TypeDef *CANx, uint8_t FifoNumber) {
	uint32_t id;
	// Read identifier information
	if ((CANx->sFIFOMailBox[FifoNumber].RIR & CAN_TI0R_IDE) == 0) {
		// Standard ID
		id = (uint32_t) 0x000007FF
				& (CANx->sFIFOMailBox[FifoNumber].RIR >> 21);
	} else {
		id = (uint32_t) 0x0003FFFF
				& (CANx->sFIFOMailBox[FifoNumber].RIR >> 3);
	}
	return id;
}

/* -------------------------------      CAN Receiving     ------------------------------- */
// note para FifoNumber can be CAN_FIFO0, CAN_FIFO1
void CAN_Receive(CAN_TypeDef *CANx, uint8_t FifoNumber, CAN_msg *msg) {

	// Read identifier information
	if ((CANx->sFIFOMailBox[FifoNumber].RIR & CAN_TI0R_IDE) == 0) {
		// Standard ID

		msg->format = CAN_STANDARD_FORMAT;

		msg->id = (uint32_t) 0x000007FF
				& (CANx->sFIFOMailBox[FifoNumber].RIR >> 21);
	} else {
		// Extended ID
		msg->format = CAN_EXTENDED_FORMAT;

		msg->id = (uint32_t) 0x0003FFFF
				& (CANx->sFIFOMailBox[FifoNumber].RIR >> 3);
	}

	// Read type information

	if ((CANx->sFIFOMailBox[FifoNumber].RIR & CAN_TI0R_RTR) == 0) {

		msg->type = CAN_DATA_FRAME;                     // DATA   FRAME

	} else {
		msg->type = CAN_REMOTE_FRAME;                   // REMOTE FRAME
	}

	// Read length (number of received bytes)
	msg->length = (uint8_t) 0x0000000F & CANx->sFIFOMailBox[FifoNumber].RDTR;

	// Read data bytes
	uint8_t ReceivDataCounter;

	for (ReceivDataCounter = 0; ReceivDataCounter < msg->length;
			ReceivDataCounter++)

			{
		if (ReceivDataCounter < 4) /*get Data from low register*/
		{
			msg->data[ReceivDataCounter] = (CANx->sFIFOMailBox[FifoNumber].RDLR
					>> (ReceivDataCounter * 8)) & 0xFF; //get Data from low register byte by byte
		} else /*get Data from high register*/
		{
			msg->data[ReceivDataCounter] = (CANx->sFIFOMailBox[FifoNumber].RDHR
					>> ((ReceivDataCounter - 4) * 8)) & 0xFF; //get Data from high register byte by byte
		}

	}

	if (FifoNumber == CAN_FIFO0) //check for input FIFO to release
	{

		CANx->RF0R |= CAN_RF0R_RFOM0;  //release FIFO 0
	}

	else if (FifoNumber == CAN_FIFO1) {

		CANx->RF1R |= CAN_RF1R_RFOM1;  //release FIFO 1

	}

	else {

		/*wrong FIFO number input*/
	}
	msg->ActiveFlag = ENABLE;
}

/*
 * ------------------------------------ CAN interrupt ------------------------------------
 */

void CAN_Interrupt_Enable(CAN_TypeDef *CANx, uint32_t CanInterruptType,
		FunctionalState InterruptNewState) {

	if (InterruptNewState == ENABLE) //check for input new state of input interrupt
			{

		CANx->IER |= CanInterruptType; // Enable the selected CANx interrupt
	} else if (InterruptNewState == DISABLE) {
		CANx->IER &= ~CanInterruptType; // Disable the selected CANx interrupt

	} else {
		/*WRONG STATE */
	}
}

/*
 * ----------------------------------- Get Last Error Code --------------------------------
 */
uint8_t CAN_GetLastErrorCodeType(CAN_TypeDef *CANx) {
	uint8_t ErrorcodeType = 0; //contain Code error Type

	ErrorcodeType = (((uint8_t) CANx->ESR) & (uint8_t) CAN_ESR_LEC); // Get the last error code

	return ErrorcodeType; //Return the error code
}

void CAN_ConfigFilter(CAN_TypeDef *CANx, CAN_FilterTypeDef *sFilterConfig) {
	uint32_t filternbrbitpos;

	/* Initialization mode for the filter */
	CANx->FMR |= CAN_FMR_FINIT;

	/* Convert filter number into bit position */
	filternbrbitpos = (uint32_t) 1 << (sFilterConfig->FilterBank & 0x1FU);

	/* Filter Deactivation */
	CANx->FA1R &= ~(filternbrbitpos);

	/* Filter Scale */

	/* 32-bit scale for the filter */
	CANx->FS1R |= filternbrbitpos;

	/* Filter Mode */
	if (sFilterConfig->FilterMode == CAN_FILTERMODE_IDMASK) {
		/* Id/Mask mode for the filter*/
		CANx->FM1R &= ~(filternbrbitpos);

	} else /* CAN_FilterInitStruct->CAN_FilterMode == CAN_FilterMode_IdList */
	{
		/* Identifier list mode for the filter*/
		CANx->FM1R |= (filternbrbitpos);
	}
	/* 32-bit identifier  */
	CANx->sFilterRegister[sFilterConfig->FilterBank].FR1 =
				((uint32_t) sFilterConfig->FilterId) << 21;
	CANx->sFilterRegister[sFilterConfig->FilterBank].FR2 =
				((uint32_t) sFilterConfig->FilterMaskId) << 21;

	/* Filter FIFO assignment */
	if (sFilterConfig->FilterFIFOAssignment == CAN_FIFO0) {
		/* FIFO 0 assignation for the filter */
		CANx->FFA1R &= ~(filternbrbitpos);
	} else {
		/* FIFO 1 assignation for the filter */
		CANx->FFA1R |= (filternbrbitpos);
	}

	/* Filter activation */
	if (sFilterConfig->FilterActivation == CAN_FILTER_ENABLE) {
		CANx->FA1R |= filternbrbitpos;
	}

	/* Leave the initialization mode for the filter */
	CANx->FMR &= ~(CAN_FMR_FINIT);

}
