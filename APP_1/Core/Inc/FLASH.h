/*
 * flash.h
 *
 *  Created on: Apr 23, 2023
 *      Author: ashra
 */
#include"STD_TYPE.h"
#include "main.h"

#ifndef BOOTLOADER_INC_FLASH_H_
#define BOOTLOADER_INC_FLASH_H_

/******************  Bit definition for FLASH_KEYR register  ******************/
#define FLASH_KEY1_Pos                      (0U)
#define FLASH_KEY1_Msk                      (0x45670123UL << FLASH_KEY1_Pos)    /*!< 0x45670123 */
#define FLASH_KEY1                          FLASH_KEY1_Msk                     /*!< FPEC Key1 */
#define FLASH_KEY2_Pos                      (0U)
#define FLASH_KEY2_Msk                      (0xCDEF89ABUL << FLASH_KEY2_Pos)    /*!< 0xCDEF89AB */
#define FLASH_KEY2                          FLASH_KEY2_Msk                     /*!< FPEC Key2 */

/******************  Bit definition for FLASH_SR register  ********************/
#define FLASH_SR_BSY_Pos                    (0U)
#define FLASH_SR_BSY_Msk                    (0x1UL << FLASH_SR_BSY_Pos)         /*!< 0x00000001 */
#define FLASH_SR_BSY                        FLASH_SR_BSY_Msk                   /*!< Busy */
#define FLASH_SR_PGERR_Pos                  (2U)
#define FLASH_SR_PGERR_Msk                  (0x1UL << FLASH_SR_PGERR_Pos)       /*!< 0x00000004 */
#define FLASH_SR_PGERR                      FLASH_SR_PGERR_Msk                 /*!< Programming Error */
#define FLASH_SR_WRPRTERR_Pos               (4U)
#define FLASH_SR_WRPRTERR_Msk               (0x1UL << FLASH_SR_WRPRTERR_Pos)    /*!< 0x00000010 */
#define FLASH_SR_WRPRTERR                   FLASH_SR_WRPRTERR_Msk              /*!< Write Protection Error */
#define FLASH_SR_EOP_Pos                    (5U)
#define FLASH_SR_EOP_Msk                    (0x1UL << FLASH_SR_EOP_Pos)         /*!< 0x00000020 */
#define FLASH_SR_EOP                        FLASH_SR_EOP_Msk                   /*!< End of operation */

/*******************  Bit definition for FLASH_CR register  *******************/
#define FLASH_CR_PG_Pos                     (0U)
#define FLASH_CR_PG_Msk                     (0x1UL << FLASH_CR_PG_Pos)          /*!< 0x00000001 */
#define FLASH_CR_PG                         FLASH_CR_PG_Msk                    /*!< Programming */
#define FLASH_CR_PER_Pos                    (1U)
#define FLASH_CR_PER_Msk                    (0x1UL << FLASH_CR_PER_Pos)         /*!< 0x00000002 */
#define FLASH_CR_PER                        FLASH_CR_PER_Msk                   /*!< Page Erase */
#define FLASH_CR_MER_Pos                    (2U)
#define FLASH_CR_MER_Msk                    (0x1UL << FLASH_CR_MER_Pos)         /*!< 0x00000004 */
#define FLASH_CR_MER                        FLASH_CR_MER_Msk                   /*!< Mass Erase */
#define FLASH_CR_OPTPG_Pos                  (4U)
#define FLASH_CR_OPTPG_Msk                  (0x1UL << FLASH_CR_OPTPG_Pos)       /*!< 0x00000010 */
#define FLASH_CR_OPTPG                      FLASH_CR_OPTPG_Msk                 /*!< Option Byte Programming */
#define FLASH_CR_OPTER_Pos                  (5U)
#define FLASH_CR_OPTER_Msk                  (0x1UL << FLASH_CR_OPTER_Pos)       /*!< 0x00000020 */
#define FLASH_CR_OPTER                      FLASH_CR_OPTER_Msk                 /*!< Option Byte Erase */
#define FLASH_CR_STRT_Pos                   (6U)
#define FLASH_CR_STRT_Msk                   (0x1UL << FLASH_CR_STRT_Pos)        /*!< 0x00000040 */
#define FLASH_CR_STRT                       FLASH_CR_STRT_Msk                  /*!< Start */
#define FLASH_CR_LOCK_Pos                   (7U)
#define FLASH_CR_LOCK_Msk                   (0x1UL << FLASH_CR_LOCK_Pos)        /*!< 0x00000080 */
#define FLASH_CR_LOCK                       FLASH_CR_LOCK_Msk                  /*!< Lock */
#define FLASH_CR_OPTWRE_Pos                 (9U)
#define FLASH_CR_OPTWRE_Msk                 (0x1UL << FLASH_CR_OPTWRE_Pos)      /*!< 0x00000200 */
#define FLASH_CR_OPTWRE                     FLASH_CR_OPTWRE_Msk                /*!< Option Bytes Write Enable */
#define FLASH_CR_ERRIE_Pos                  (10U)
#define FLASH_CR_ERRIE_Msk                  (0x1UL << FLASH_CR_ERRIE_Pos)       /*!< 0x00000400 */
#define FLASH_CR_ERRIE                      FLASH_CR_ERRIE_Msk                 /*!< Error Interrupt Enable */
#define FLASH_CR_EOPIE_Pos                  (12U)
#define FLASH_CR_EOPIE_Msk                  (0x1UL << FLASH_CR_EOPIE_Pos)       /*!< 0x00001000 */
#define FLASH_CR_EOPIE                      FLASH_CR_EOPIE_Msk                 /*!< End of operation interrupt enable */

/**  FLASHEx_Page_Size Page Size
  *
  */
#define FLASH_PAGE_SIZE          0x400U          //1kB


#define FLASH_SIZE		(uint32_t)0x00020000	// 256kB

/**  FLASHEx_Type_Erase Type Erase
  *
  */

#define FLASH_TYPEERASE_PAGES     0x00U  /*!<Pages erase only*/
#define FLASH_TYPEERASE_MASSERASE 0x02U  /*!<Flash mass erase activation*/

/** FLASH_Type_Program FLASH Type Program
  *
  */
#define FLASH_TYPEPROGRAM_HALFWORD             0x01U  /*!<Program a half-word (16-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_WORD                 0x02U  /*!<Program a word (32-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_DOUBLEWORD           0x03U  /*!<Program a double word (64-bit) at a specified address*/
#define FLASH_R_BASE          (0x40000000UL + 0x00020000UL + 0x00002000UL) /*!< Flash registers base address */
#define FLASH               ((FLASH_TypeDef *)FLASH_R_BASE)
/**
  * @ FLASH Registers
  */

typedef struct
{
  volatile uint32_t ACR;
  volatile uint32_t KEYR;
  volatile uint32_t OPTKEYR;
  volatile uint32_t SR;
  volatile uint32_t CR;
  volatile uint32_t AR;
  volatile uint32_t RESERVED;
  volatile uint32_t OBR;
  volatile uint32_t WRPR;
} FLASH_TypeDef;

/**
  * @brief  FLASH Erase structure definition
  */
typedef struct
{
  uint32_t TypeErase;   /*!< TypeErase: Mass erase or page erase.*/



  uint32_t PageAddress; /*!< PageAdress: Initial FLASH page address to erase when mass erase is disabled
                             This parameter must be a number between Min_Data = 0x08000000 and Max_Data = FLASH_BANKx_END
                             (x = 1 or 2 depending on devices)*/

  uint32_t NbPages;     /*!< NbPages: Number of pagess to be erased.
                             This parameter must be a value between Min_Data = 1 and Max_Data = (max number of pages - value of initial page)*/

} FLASH_EraseTypeDef;

/**
  *  FLASH Lock structures definition
  */
typedef enum
{
  FLASH_UNLOCKED = 0x00U,
  FLASH_LOCKED   = 0x01U
} FLASH_LockTypeDef;

void FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
void FLASH_Lock(void);
void FLASH_Unlock(void);
void FLASH_Erase(FLASH_EraseTypeDef *pEraseInit);

#endif /* BOOTLOADER_INC_FLASH_H_ */
