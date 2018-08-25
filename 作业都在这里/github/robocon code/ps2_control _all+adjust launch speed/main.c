#include <reg51.h>
#include <intrins.h>
#include "motor.h"
#include "ps2.h"
#include "Launcher.h"


void main()
{ 
	uchar key; 
	 	
	EA = 1;
	Timer1_Init();
	init0();
		
	while(1)
	{
				  																																																																																						
		key=PS2_DataKey();

		if(key == 2)               
		{
			delay(10);
			while(1)
			{
			
			key=PS2_DataKey();
				
			switch(key)
			{
			case 0 : stop(); //不按的时候
			           ET1=0;
			           key=100;	  break;
			case 13:  stright();//前进
				       key=100;	  break;
			case 16:  left();	 //左转
					   key=100;	  break;
			case 15:  back();	 //后退
					   key=100;	  break;
			case 14:   right(); //右转
					   key=100;	  break;
			case 1:  stright1();  //两轮前进
				       key=100;	  break;
			case 2:  stop();     //停止
			 	       key=100;	  break;
			case 9:  Launcherb(); //发射出去
					   key=100;	  break;
			case 11: Launcherc(); //收回
				       key=100;	  break;
			case 12: Launchera(); //发射后收回
				       key=100;	  break;
			case 8: ET1=1;DIR=0; //驱动步进电机方向2
					   key=100;	  break;
			case 6: ET1=1;DIR=1; //驱动步进电机方向1
					   key=100;	  break;
			case 7: count -= 2;  //摩擦轮减速
					   key=100;	  break;
			case 5: count += 2;  //摩擦轮加速
					   key=100;   break;
			case 4: back1();  //后退1
					   key=100;   break;
			}
			delay(5);

			  
		}	}  }	}   


