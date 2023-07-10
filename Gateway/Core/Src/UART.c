/*
 * UART.c
 *
 *  Created on: Jan 29, 2023
 *      Author: moham
 */


#include "UART.h"



void UART_INIT(USART_TypeDef *UARTx){
	//Clear All
	UARTx->CR1 = 0x00;
	UARTx->CR2 = 0x00;
	//UARTx->CR3 = 0x00;
	// Enable the USART by writing the UE bit in USART_CR1 register to 1.
	UARTx->CR1 |= USART_CR1_UE;
	//One sample method
	UARTx->CR3 |= USART_CR3_ONEBIT;
	//enable TX ,RX
#ifdef UART_ENABLE_TX
	UARTx->CR1 |= UART_ENABLE_TX;
#endif

#ifdef UART_ENABLE_RX
	UARTx->CR1 |= UART_ENABLE_RX;
#endif
#if UAST_OVERSAMPLING == UAST_OVERSAMPLING_8
	UARTx->CR1 |= UAST_OVERSAMPLING << 15;
#endif

	//Program the M bit in USART_CR1 to define the word length(8).
#ifdef UART_WORDLENGHT_9
	UARTx->CR1 |= UART_WORDLENGHT_9;
#endif
	//enable parity control
#ifdef UART_ENABLEPARITY
	UARTx->CR1 |= UART_ENABLEPARITY;
#endif
	////odd parity
#ifdef UART_PARITYSELECTION
	UARTx->CR1 |= UART_PARITYSELECTION;
#endif
	//Program stop bits in USART_CR2.
#if (UART_STOPBITS != UART_1STOPBIT)
	UARTx->CR2 |= UART_STOPBITS;
#endif
	/*Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
	place.Configure the DMA register as explained in multibuffer communication. */
#ifdef UART_DMA_TX_Enable
	UARTx->CR3 |= UART_DMA_TX_Enable;
#endif
#ifdef UART_DMA_RX_Enable
	UARTx->CR3 |= UART_DMA_RX_Enable;
#endif

	//Select the desired baud rate using the USART_BRR register.fclk for PCLK1 or 2 (as config in RCC)
	uint16_t Mantissa = FCLK / (8 * (2 - UAST_OVERSAMPLING) * BAUDRATE);
	uint16_t Farction = (8.00 * (2.00 - UAST_OVERSAMPLING)) * (FCLK / (8.00 * (2.00 - UAST_OVERSAMPLING) * BAUDRATE) - Mantissa);
	if(Farction >= 0xF){
		Mantissa += Farction;
		Farction = 0;
	}
	UARTx->BRR = Mantissa << 4 | Farction;

	//Enable Reception and Transmission complete interrupt
#ifdef UART_RX_INTERRUPT
	UARTx->CR1 |= USART_CR1_RXNEIE;
#endif
#ifdef UART_TX_INTERRUPT
	UARTx->CR1 |= UART_TX_INTERRUPT;
#endif

}
void UART_Send(USART_TypeDef *UARTx, uint8_t data){
	//Avoid overwriting the previous data
	while(!(UARTx->SR & USART_SR_TXE));
	UARTx->DR = data;
	//Wait for transfer complete
	while(!(UARTx->SR & USART_SR_TC));
}
void UART_Send_String(USART_TypeDef *UARTx, uint8_t *data){
	uint8_t counter = 0;
	while(data[counter]!= '\0')
		UART_Send(UARTx, data[counter++]);
}
uint8_t UART_Receive(USART_TypeDef *UARTx){
	uint8_t data;
	while(!(UARTx->SR & USART_SR_RXNE));
	data = UARTx->DR;
	UARTx->SR &= ~(USART_SR_RXNE_Msk);
	return data;
}
void UART_Receive_String(USART_TypeDef *UARTx, uint8_t *data){
	static uint8_t counter = 0;
	data[counter++] = UART_Receive(UARTx);
}


