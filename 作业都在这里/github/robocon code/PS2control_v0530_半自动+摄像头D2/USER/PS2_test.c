#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h"	
#include "pstwo.h"
/*********************************************************
Description: ps2应用程序输出按键值、模拟值

**********************************************************/	 	 	  
int main(void)
{								  
	u8 key;  
//	Stm32_Clock_Init(9); //系统时钟设置
	delay_init();	     //延时初始化
	uart_init(115200);  //串口1初始化 
	LED_Init();
	PS2_Init();			   //驱动端口初始化
	PS2_SetInit();		 //配置初始化,配置“红绿灯模式”，并选择是否可以修改
	                    
	while(1)
	{
		D2 =! D2;
		key=PS2_DataKey();
		if(key!=0)                   //有按键按下   
    {
			printf("  \r\n   %d   is  light  \r\n",Data[1]);// ID   0x41(65)绿灯模式(灯灭)；0x73(115)红灯模式。
			switch(key)
			{	case 1: printf("   SELECT is  pressed  \r\n");	break;
				case 2: printf("   L3   is  pressed  \r\n");	break;
			  case 3: printf("   R3   is  pressed  \r\n");	break;
			  case 4: printf("   START is  pressed  \r\n");	break;
			  case 5: printf("   UP   is  pressed  \r\n");	break;	
			  case 6: printf("   RIGHT is  pressed  \r\n");	break;	
			  case 7: printf("   DOWN  is  pressed  \r\n");	break;
			  case 8: printf("   LEFT  is  pressed  \r\n");	break;
			  case 9: printf("   L2   is  pressed  \r\n");	break;
			  case 10: printf("   R2   is  pressed  \r\n");	break;
			  case 11: printf("   L1   is  pressed  \r\n");	break;
			  case 12: printf("   R1   is  pressed  \r\n");	break;	
			  case 13: printf("   GREEN is  pressed  \r\n");	break;
			  case 14: printf("   RED   is  pressed  \r\n");	break;
			  case 15: printf("   BLUE  is  pressed  \r\n");	break;
			  case 16: printf("   PINK  is  pressed  \r\n");	break;				 
			}

    }
		//打印出摇杆模拟值   范围0~256  中间值为127，128
		printf(" %5d %5d %5d %5d\r\n",PS2_AnologData(PSS_LX),PS2_AnologData(PSS_LY),
		                              PS2_AnologData(PSS_RX),PS2_AnologData(PSS_RY));
		delay_ms(50);
		
		
	}
	
}


