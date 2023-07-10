/*
 * UART_Config.h
 *
 *  Created on: Feb 3, 2023
 *      Author: moham
 */

#ifndef INC_UART_CONFIG_H_
#define INC_UART_CONFIG_H_

#define BAUDRATE 	9600
//used clock bus
#define FCLK		45000000

//Enable TX ,RX
#define UART_ENABLE_TX				USART_CR1_TE
#define UART_ENABLE_RX				USART_CR1_RE

#define UAST_OVERSAMPLING 			UAST_OVERSAMPLING_8

// determines the word length (9 bits if defined ,8 if is not defined)
//#define UART_WORDLENGHT_9		USART_CR1_M

//Enable Parity Control (if defined)
//#define UART_ENABLEPARITY		USART_CR1_PCE

//Select Parity type
//#define UART_PARITYSELECTION		USART_CR1_PS

//Number of Stop Bits
//0.5 and 1.5 not supported in USART4 and USART5
#define UART_STOPBITS				UART_1STOPBIT

//Enable DMA TX and RX
#define UART_DMA_TX_Enable			USART_CR3_DMAT
#define UART_DMA_RX_Enable			USART_CR3_DMAR

//Enable RX,TX interrupt
//#define UART_RX_INTERRUPT			USART_CR1_RXNEIE
//#define UART_TX_INTERRUPT			USART_CR1_TXEIE

#endif /* INC_UART_CONFIG_H_ */
