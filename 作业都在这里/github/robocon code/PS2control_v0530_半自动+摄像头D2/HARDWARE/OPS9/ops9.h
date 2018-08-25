/**
  ******************************************************************************
  * @file    ops9.h 
  * @author  Billy, Action Team
  * @version V1.0.0
  * @date    30-October-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ACTION ROBOT SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2017 Action Robot</center></h2>
  ******************************************************************************  
  */
#ifndef __OPS9_H
#define __OPS9_H
#include "stdint.h"


void SetAngle(float angle);
float GetAngle(void);

void  SetPositionX(float x);
float GetPositionX(void);

void  SetPositionY(float y);

float GetPositionY(void);

//#define OPS9toUSART1   //使用串口1来接收，注释后则用串口2
/*移植自debug.c*/
#define DEBUG_USART  USART2
void USART_OUT_XYAngle(void);

#endif

