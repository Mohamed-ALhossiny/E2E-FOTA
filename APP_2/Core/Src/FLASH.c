/*
 * flash.c
 *
 *  Created on: Apr 23, 2023
 *      Author: ashra
 */

#include "FLASH.h"

/* Private function prototypes -----------------------------------------------*/
static void FLASH_MassErase(void);
static void FLASH_PageErase(uint32_t PageAddress);
static void FLASH_Program_HalfWord(uint32_t Address, uint16_t Data);


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


void FLASH_Erase(FLASH_EraseTypeDef *pEraseInit)
{
	 uint32_t address = 0U;
	  if (pEraseInit->TypeErase == FLASH_TYPEERASE_MASSERASE)
	  {
	        /*Mass erase to be done*/
	        FLASH_MassErase();
	        /* If the erase operation is completed, disable the MER Bit */
	        FLASH->CR &= ~(FLASH_CR_MER);
	  }

	  else
	  {
	        /* Page Erase is requested */

		    /* Erase page by page to be done*/
		     for(address = pEraseInit->PageAddress; address <= ((pEraseInit->NbPages * FLASH_PAGE_SIZE) + pEraseInit->PageAddress); address += FLASH_PAGE_SIZE)
		     {
		        FLASH_PageErase(address);

			  /* If the erase operation is completed, disable the PER Bit */
			   FLASH->CR &= ~(FLASH_CR_PER);

	         }
	  }

}
/*
 *  ----------------------------  Mass Erase Of Flash  ----------------------------------
 */
static void FLASH_MassErase(void)
{

	   /* Wait for busy flag */
	   while (RESET != (FLASH->SR & FLASH_SR_BSY))
	   {
		  /* WAITING */
	   }
  /*  proceed to erase all sectors */
  FLASH->CR |= FLASH_CR_MER;
  FLASH->CR |= FLASH_CR_STRT;
       /* Wait for busy flag */
       while (RESET != (FLASH->SR & FLASH_SR_BSY))
       {
	       /* WAITING */
       }
}

/**
  *         Erase the specified FLASH memory page
  *         PageAddress FLASH page to erase
  *         The value of this parameter depend on device used within the same series
  */
static void FLASH_PageErase(uint32_t PageAddress)
{
	/* Wait for busy flag */
	  while (RESET != (FLASH->SR & FLASH_SR_BSY))
	  {
		  /* WAITING */
	  }
    /* Proceed to erase the page */
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = PageAddress;
    FLASH->CR |= FLASH_CR_STRT;

	/* Wait for busy flag */
	  while (RESET != (FLASH->SR & FLASH_SR_BSY))
	  {
		  /* WAITING */
	  }

}

void FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
{
	  uint8_t index = 0;
	  uint8_t nbiterations = 0;

		/* Wait for busy flag */
		  while (RESET != (FLASH->SR & FLASH_SR_BSY))
		  {
			  /* WAITING */
		  }

		  if(TypeProgram == FLASH_TYPEPROGRAM_HALFWORD)
		      {
		        /* Program halfword (16-bit) at a specified address. */
		        nbiterations = 1U;
		      }
		      else if(TypeProgram == FLASH_TYPEPROGRAM_WORD)
		      {
		        /* Program word (32-bit = 2*16-bit) at a specified address. */
		        nbiterations = 2U;
		      }
		      else
		      {
		        /* Program double word (64-bit = 4*16-bit) at a specified address. */
		        nbiterations = 4U;
		      }

		    for (index = 0U; index < nbiterations; index++)
		    {
		      FLASH_Program_HalfWord((Address + (2U*index)), (uint16_t)(Data >> (16U*index)));

				/* Wait for busy flag */
				  while (RESET != (FLASH->SR & FLASH_SR_BSY))
				  {
					  /* WAITING */
				  }

		        /* If the program operation is completed, disable the PG Bit */
				  FLASH->CR &= (~FLASH_CR_PG);
		     }
}

/**
  *        Program a half-word (16-bit) at a specified address.
  *        Address specify the address to be programmed.
  *        Data    specify the data to be programmed.
  *
  */
static void FLASH_Program_HalfWord(uint32_t Address, uint16_t Data)
{

	/* Proceed to program the new data */
    /* Set Programming Bit */
	FLASH->CR |= FLASH_CR_PG;
	  /* Write data in the address */
	  *(volatile uint16_t*)Address = Data;
}



