/*
 * CRC.h
 *
 *  Created on: Feb 11, 2023
 *      Author: moham
 */

#ifndef INC_CRC_H_
#define INC_CRC_H_
#include "STD_TYPE.h"


/**
  * @brief CRC calculation unit
  */

typedef struct
{
  volatile uint32_t DR;         /*!< CRC Data register,             Address offset: 0x00 */
  volatile uint8_t  IDR;        /*!< CRC Independent data register, Address offset: 0x04 */
  uint8_t       RESERVED0;  /*!< Reserved, 0x05                                      */
  uint16_t      RESERVED1;  /*!< Reserved, 0x06                                      */
  volatile uint32_t CR;         /*!< CRC Control register,          Address offset: 0x08 */
} CRC_TypeDef;

#define CRC_BASE              (0x40000000UL + 0x00020000UL + 0x3000UL)
#define CRC                 ((CRC_TypeDef *) CRC_BASE)






uint32_t CRC_Clac(uint32_t* data, uint16_t size);

















#endif /* INC_CRC_H_ */
