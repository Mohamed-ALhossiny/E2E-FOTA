/*
 * CAN1_cfg.h
 *
 *  Created on: Feb 2, 2023
 *      Author: ashra
 */

#ifndef CAN_CFG_H_
#define CAN_CFG_H_
#include "STD_TYPE.h"
#include "main.h"

#define CAN_BTR							0x001e000f//125k

#define CAN_TESTMODE_OFF					0x0
#define CAN_TESTMODE_ON			(CAN_BTR_SILM | CAN_BTR_LBKM)




/*
Description: CAN setup structure configuration
*/
typedef struct
{
  FunctionalState TTCM; //enable  or disable time trigger communication mode

  FunctionalState ABOM; //Enable or disable the automatic bus-off  management

  FunctionalState AWUM;  //Enable or disable the automatic wake-up mode

  FunctionalState NART; //Enable or disable the no-automatic retransmission mode

  FunctionalState RFLM;  // Enable or disable the Receive FIFO Locked mode

  FunctionalState TXFP;  // Enable or disable the transmit FIFO priority

} CAN_setupTypeDef;



typedef struct
{
  uint32_t FilterId;           /*!< Specifies the filter identification number (LSBs for a 32-bit
                                       configuration, second one for a 16-bit configuration).
                                       This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  uint32_t FilterMaskId;      /*!< Specifies the filter mask number or identification number,
                                       according to the mode (MSBs for a 32-bit configuration,
                                       first one for a 16-bit configuration).
                                       This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  uint32_t FilterFIFOAssignment;  /*!< Specifies the FIFO (0 or 1U) which will be assigned to the filter.
                                       This parameter can be a value of @ref CAN_filter_FIFO */

  uint32_t FilterBank;            /*!< Specifies the filter bank which will be initialized.
                                       For single CAN instance(14 dedicated filter banks),
                                       this parameter must be a number between Min_Data = 0 and Max_Data = 13.
                                       For dual CAN instances(28 filter banks shared),
                                       this parameter must be a number between Min_Data = 0 and Max_Data = 27. */

  uint32_t FilterMode;            /*!< Specifies the filter mode to be initialized.
                                       This parameter can be a value of @ref CAN_filter_mode */


  uint32_t FilterActivation;      /*!< Enable or disable the filter.
                                       This parameter can be a value of @ref CAN_filter_activation */

} CAN_FilterTypeDef;



#endif /* CAN_CFG_H_ */
