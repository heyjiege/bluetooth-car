#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define D4 PBout(5)// PB5 
#define D2 PEout(5)// PE5

#define IOG2 PGout(11)
#define IOG3 PGout(14)
#define IOG4 PGout(9)
#define IOG5 PGout(13)

#define IOA0 PAout(0)
#define IOA1 PAout(1)
#define IOA4 PAout(4)
#define IOA5 PAout(5)
#define IOA6 PAout(6)
#define IOA7 PAout(7)
#define IOB0 PBout(0)



void LED_Init(void);//��ʼ��


#define IN_E2 PEin(2)
#define IN_E3 PEin(3)
#define IN_E4 PEin(4)
#define IN_E7 PEin(7)
 void Sencer_Init(void);
				    
#endif
