/*
 * main.h
 *
 *  Created on: Feb 7, 2023
 *      Author: moham
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

typedef enum
{
  DISABLE = 0U,
  ENABLE = !DISABLE
} FunctionalState;


typedef enum
{
  SUCCESS = 0U,
  ERROR = !SUCCESS
} ErrorStatus;

typedef enum
{
  RESET = 0U,
  SET = !RESET
} FlagStatus, ITStatus;

#define NULL ((void *)0)
typedef void(*fptr)(void);


#endif /* INC_MAIN_H_ */
