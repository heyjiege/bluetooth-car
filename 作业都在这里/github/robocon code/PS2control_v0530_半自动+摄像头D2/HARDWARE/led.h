#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
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



void LED_Init(void);//初始化


#define IN_E2 PEin(2)
#define IN_E3 PEin(3)
#define IN_E4 PEin(4)
#define IN_E7 PEin(7)
 void Sencer_Init(void);
				    
#endif
