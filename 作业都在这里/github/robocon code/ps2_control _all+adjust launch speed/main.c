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
			case 0 : stop(); //������ʱ��
			           ET1=0;
			           key=100;	  break;
			case 13:  stright();//ǰ��
				       key=100;	  break;
			case 16:  left();	 //��ת
					   key=100;	  break;
			case 15:  back();	 //����
					   key=100;	  break;
			case 14:   right(); //��ת
					   key=100;	  break;
			case 1:  stright1();  //����ǰ��
				       key=100;	  break;
			case 2:  stop();     //ֹͣ
			 	       key=100;	  break;
			case 9:  Launcherb(); //�����ȥ
					   key=100;	  break;
			case 11: Launcherc(); //�ջ�
				       key=100;	  break;
			case 12: Launchera(); //������ջ�
				       key=100;	  break;
			case 8: ET1=1;DIR=0; //���������������2
					   key=100;	  break;
			case 6: ET1=1;DIR=1; //���������������1
					   key=100;	  break;
			case 7: count -= 2;  //Ħ���ּ���
					   key=100;	  break;
			case 5: count += 2;  //Ħ���ּ���
					   key=100;   break;
			case 4: back1();  //����1
					   key=100;   break;
			}
			delay(5);

			  
		}	}  }	}   


