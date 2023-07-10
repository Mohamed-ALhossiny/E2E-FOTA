/*
 * FLASH.c
 *
 *  Created on: Feb 10, 2023
 *      Author: moham
 */


/*
 * flash.c
 *
 *  Created on: Feb 8, 2023
 *      Author: ashra
 */
#include "FLASH.h"

FLASH_ProcessTypeDef pFlash;

/* Program operations */
static void   FLASH_Program_Word(uint32_t Address, uint32_t Data);
static void   FLASH_Program_HalfWord(uint32_t Address, uint16_t Data);
static void   FLASH_Program_Byte(uint32_t Address, uint8_t Data);


/*
 * --------------------  Unlock the FLASH control register access  -----------------------
 *
 */
void FLASH_Unlock(void)
{

    /* Authorize the FLASH Registers access */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

/**
  *-------------------  Locks the FLASH control register access --------------------------
  */
void FLASH_Lock(void)
{
  /* Set the LOCK Bit to lock the FLASH Registers access */
  FLASH->CR |= FLASH_CR_LOCK;

}

/*
 * -----------------------  Erase the specified FLASH memory sector  ---------------------
 */
void FLASH_Erase_Sector(uint32_t Sector, uint8_t VoltageRange)
{
	/* Wait for busy flag */
	  while (RESET != (FLASH->SR & FLASH_SR_BSY))
	  {
		  /* WAITING */
	  }
  uint32_t tmp_psize = 0U;

  if (VoltageRange == FLASH_VOLTAGE_RANGE_1)
  {
    tmp_psize = FLASH_PSIZE_BYTE;
  }
  else if (VoltageRange == FLASH_VOLTAGE_RANGE_2)
  {
    tmp_psize = FLASH_PSIZE_HALF_WORD;
  }
  else if (VoltageRange == FLASH_VOLTAGE_RANGE_3)
  {
    tmp_psize = FLASH_PSIZE_WORD;
  }
  else
  {
    tmp_psize = FLASH_PSIZE_DOUBLE_WORD;
  }

  /* proceed to erase the sector */
  FLASH->CR &= ~FLASH_CR_PSIZE;
  FLASH->CR |= tmp_psize;
  FLASH->CR &= ~FLASH_CR_SNB;
  FLASH->CR |= FLASH_CR_SER | (Sector << FLASH_CR_SNB_Pos);
  FLASH->CR |= FLASH_CR_STRT;
  /* Wait for busy flag */
  	  while (RESET != (FLASH->SR & FLASH_SR_BSY))
  	  {
  		  /* WAITING */
  	  }
}

/*
 *  ----------------------------  Mass Erase Of Flash  ----------------------------------
 */
void FLASH_MassErase(uint8_t VoltageRange)
{

  /*  proceed to erase all sectors */
  FLASH->CR &= ~FLASH_CR_PSIZE;
  FLASH->CR |= FLASH_CR_MER;
  FLASH->CR |= FLASH_CR_STRT | ((uint32_t)VoltageRange << 8U);
}

/*
 * ------- Program byte, halfword, word or double word at a specified address ----------
 */
void FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint32_t Data)
{


  /* Wait for busy flag */
  while (RESET != (FLASH->SR & FLASH_SR_BSY))
  {
	  /* WAITING */
  }

    if(TypeProgram == FLASH_TYPEPROGRAM_BYTE)
    {
      /*Program byte (8-bit) at a specified address.*/
      FLASH_Program_Byte(Address, (uint8_t) Data);
    }
    else if(TypeProgram == FLASH_TYPEPROGRAM_HALFWORD)
    {
      /*Program halfword (16-bit) at a specified address.*/
      FLASH_Program_HalfWord(Address, (uint16_t) Data);
    }
    else if(TypeProgram == FLASH_TYPEPROGRAM_WORD)
    {
      /*Program word (32-bit) at a specified address.*/
      FLASH_Program_Word(Address, (uint32_t) Data);
    }

    /* Wait for busy flag */
    while (RESET != (FLASH->SR & FLASH_SR_BSY))
    {
  	  /* WAITING */
    }

    /* If the program operation is completed, disable the PG Bit */
    FLASH->CR &= (~FLASH_CR_PG);

}

/*
 *  ----------------  Program word (32-bit) at a specified address  --------------------
 */
static void FLASH_Program_Word(uint32_t Address, uint32_t Data)
{

  /*  proceed to program the new data */
  FLASH->CR &= ~FLASH_CR_PSIZE;
  FLASH->CR |= FLASH_PSIZE_WORD;
  FLASH->CR |= FLASH_CR_PG;

  *(volatile uint32_t*)Address = Data;
}

/*
 * --------------- Program a half-word (16-bit) at a specified address -----------------
 */
static void FLASH_Program_HalfWord(uint32_t Address, uint16_t Data)
{

  /*  proceed to program the new data */
  FLASH->CR &= ~FLASH_CR_PSIZE;
  FLASH->CR |= FLASH_PSIZE_HALF_WORD;
  FLASH->CR |= FLASH_CR_PG;

  *(volatile uint16_t*)Address = Data;
}

/*
 * ------------------- Program byte (8-bit) at a specified address ----------------------
 */
static void FLASH_Program_Byte(uint32_t Address, uint8_t Data)
{

  /*  proceed to program the new data */
  FLASH->CR &= ~FLASH_CR_PSIZE;
  FLASH->CR |= FLASH_PSIZE_BYTE;
  FLASH->CR |= FLASH_CR_PG;

  *(volatile uint8_t*)Address = Data;
}

/*
 *  ----------------------   Set the specific FLASH error  ----------------------------
 */
void FLASH_Set_ErrorCode(void)
{
	  if((FLASH->SR & (FLASH_SR_PGSERR)) != RESET)   // get flag
	  {
	    pFlash.ErrorCode |= FLASH_ERROR_PGS;

	    /* Clear FLASH Programming sequence error pending bit */
	    (FLASH->SR = (FLASH_SR_PGSERR));
	  }

	  if((FLASH->SR & (FLASH_SR_PGPERR)) != RESET)
	  {
	    pFlash.ErrorCode |= FLASH_ERROR_PGP;

	    /* Clear FLASH Programming parallelism error pending bit */
	    (FLASH->SR = (FLASH_SR_PGPERR));
	  }

	  if((FLASH->SR & (FLASH_SR_PGAERR)) != RESET)
	  {
	    pFlash.ErrorCode |= FLASH_ERROR_PGA;

	    /* Clear FLASH Programming parallelism error pending bit */
	    (FLASH->SR = (FLASH_SR_PGAERR));
	  }

	  if((FLASH->SR & (FLASH_SR_WRPERR)) != RESET)
	  {
	    pFlash.ErrorCode |= FLASH_ERROR_WRP;

	    /* Clear FLASH Programming parallelism error pending bit */
	    (FLASH->SR = (FLASH_SR_WRPERR));
	  }

#if defined(FLASH_SR_RDERR)
  if((FLASH->SR & (FLASH_SR_RDERR)) != RESET)
  {
    pFlash.ErrorCode |= FLASH_ERROR_RD;

    /* Clear FLASH Proprietary readout protection error pending bit */
    (FLASH->SR = (FLASH_SR_RDERR));
  }
#endif /* FLASH_SR_RDERR */
  if((FLASH->SR & (FLASH_SR_SOP)) != RESET)
  {
    pFlash.ErrorCode |= FLASH_ERROR_OPERATION;

    /* Clear FLASH Operation error pending bit */
    (FLASH->SR = (FLASH_SR_SOP));
  }

}

/*
 *  ----------------------   Get the specific FLASH error  -----------------------------
 */
uint32_t FLASH_GetError(void)
{
   return pFlash.ErrorCode;
}
