/*
 * BIT_MATH.h
 *
 *  Created on: Jan 30, 2023
 *      Author: moham
 */

#ifndef SRC_BIT_MATH_H_
#define SRC_BIT_MATH_H_

#define GET_BIT(Reg,Bit) ( Reg >> Bit) & (0x01)
#define SET_BIT(Byte, Pos) 		Byte |= (1 << Pos)
#define CLEAR_BIT(Byte, Pos) 	Byte &= ~(1 << Pos)
#define TOGGLE_BIT(Byte, Pos) 	Byte ^= (1 << Pos)

#define SET_BYTE(Byte) 			Byte |= (0xFF)
#define CLEAR_BYTE(Byte) 		Byte &=(0x00)

#endif /* SRC_BIT_MATH_H_ */
