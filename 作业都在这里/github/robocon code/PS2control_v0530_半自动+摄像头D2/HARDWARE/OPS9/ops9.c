/**
  ******************************************************************************
  * @file    ops9.c 
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
  
#include "ops9.h"
#include "stdint.h"
#include "math.h"
#include "usart.h"
#include "pingdingsystem.h"

//distance between robot's geometric center and ops9' center
#define CENTER_COMPENSATE -300.0f
static float gAngle = 0.0f;   	//陀螺仪z方向角度 范围-180~180
static float gPostionX = 0.0f;	 //定位系统返回的X坐标
static float gPostionY = 0.0f;	 //定位系统返回的Y坐标

/**
  * @brief  Set the robot's angle from ops9 
  * @param  none 
  * @retval void
  */
void SetAngle(float angle)
{
	gAngle = angle;
}

/**
  * @brief  Get the robot's angle from ops9 
  * @param  none 
  * @retval void
  */
float GetAngle(void)
{
	return gAngle;
}

/**
  * @brief  Set the robot's position x from ops9 
  * @param  none 
  * @retval void
  */
void SetPositionX(float x)
{
	gPostionX = x;
}

/**
  * @brief  Get the robot's position x from ops9 
  * @param  none 
  * @retval void
  */
float GetPositionX(void)
{
	return gPostionX + CENTER_COMPENSATE*sin(GetAngle()*3.1415926f/180.0f);
}

/**
  * @brief  Set the robot's position y from ops9 
  * @param  none 
  * @retval void
  */
void SetPositionY(float y)
{
	gPostionY = y;
}

/**
  * @brief  Get the robot's position Y from ops9 
  * @param  none 
  * @retval void
  */

float GetPositionY(void)
{
	return gPostionY - CENTER_COMPENSATE*cos(GetAngle()*3.1415926f/180.0f);
}

/****串口1 接收OPS9数据中断服务函数*********/
extern _Bool isOps9Rreay;
#ifdef OPS9toUSART1
void USART1_IRQHandler(void)
{
	static uint8_t ch = 0;
	static uint8_t count = 0;
	static uint8_t i = 0;
	
	static union
	{
		uint8_t data[24];
		float ActVal[6];
	}posture;
		
	 if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)   
	 {
		USART_ClearITPendingBit( USART1,USART_IT_RXNE);
		ch=USART_ReceiveData(USART1);
		switch(count)
		{
		case 0:
			if(ch == 0x0d)
				count++;
			else
				count = 0;
			break;
		 
		case 1:
			if(ch == 0x0a)
			{
				i = 0;
				count++;
			}
			else if(ch == 0x0d);
			else
				count = 0;
			break;
		 
		case 2:
			posture.data[i] = ch;
			i++;
			if(i >= 24)
			{
				i = 0;
				count++;
			}
			break;
		
		case 3:
			if(ch == 0x0a)
				count++;
			else
				count=0;
			break;
		 
		case 4:
			if(ch == 0x0d)
			{				 
				SetPositionX(posture.ActVal[3]);
				SetPositionY(posture.ActVal[4]);
				SetAngle(posture.ActVal[0]);
				isOps9Rreay = 1;
			}
			count = 0;
			break;

		default:
			count = 0;
			break;		 
		}	 	 
	 }
}
#else 
void USART2_IRQHandler(void)
{
	static uint8_t ch = 0;
	static uint8_t count = 0;
	static uint8_t i = 0;
	
	static union
	{
		uint8_t data[24];
		float ActVal[6];
	}posture;
		
	 if(USART_GetITStatus(USART2, USART_IT_RXNE)==SET)   
	 {
		USART_ClearITPendingBit( USART2,USART_IT_RXNE);
		ch=USART_ReceiveData(USART2);
		switch(count)
		{
		case 0:
			if(ch == 0x0d)
				count++;
			else
				count = 0;
			break;
		 
		case 1:
			if(ch == 0x0a)
			{
				i = 0;
				count++;
			}
			else if(ch == 0x0d);
			else
				count = 0;
			break;
		 
		case 2:
			posture.data[i] = ch;
			i++;
			if(i >= 24)
			{
				i = 0;
				count++;
			}
			break;
		
		case 3:
			if(ch == 0x0a)
				count++;
			else
				count=0;
			break;
		 
		case 4:
			if(ch == 0x0d)
			{				 
				SetPositionX(posture.ActVal[3]);
				SetPositionY(posture.ActVal[4]);
				SetAngle(posture.ActVal[0]);
				isOps9Rreay = 1;
			}
			count = 0;
			break;

		default:
			count = 0;
			break;		 
		}	 	 
	 }
}
#endif
/**
  * @brief  Send robot's angle/position x/position y out 
  * @param  none 
  * @retval void
  */
void USART_OUT_XYAngle(void)
{
#ifdef OPS9toUSART1
	USART_OUT(DEBUG_USART, (uint8_t *)"%d\t%d\t%d\r\n", (int)GetPositionX(), (int)GetPositionY(), (int)GetAngle());
#else
	//printf("%d\t%d\t%d\r\n", (int)GetPositionX(), (int)GetPositionY(), (int)GetAngle());
	printf("%d\t%d\t%d\r\n", (int)GetZuobiao(1), (int)GetZuobiao(2), (int)GetZuobiao(3));
#endif
}
