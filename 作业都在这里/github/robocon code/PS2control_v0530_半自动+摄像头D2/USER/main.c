/************************************************************************************
  ============= STM32F103ZET6 综合型工程模板============
  外设硬件： 		   
  内部启用：
  软件功能：
		   
  作    者：Janson	正点原子	             
*************************************************************************************/
#include "stm32f10x.h"
#include "sys.h"       //系统中断分组设置化	置NVIC中断分组2:2位抢占优先级，2位响应优先级 

#include "ST7735.h"

#include "exti.h"

int main(void)
{	
    u8 keynum;//软件 按键扫描用
	u16 tempwm=0;
//	u8 dir=1;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	delay_init();
	LED_Init();

    uart_init(115200);	 //串口初始化为9600

	KEY_Init();         	//初始化与按键连接的硬件接口 	
//	KEY_EXTIX_Init();		 	//外部中断按键初始化	
//	TIM3_PWM_Init(899,0);	 //不分频。PWM频率=72000/900=8Khz

	printf("\r\nMCU is ready.\r\n\r\n");
	while(1)
	{
 	     tempwm++;
		 tempwm=tempwm%1000;
		 delay_ms(1);	
     		
//		if(dir)led0pwmval=led0pwmval+2;
//		else led0pwmval=led0pwmval-2;
// 		if(led0pwmval>300)dir=0;
//		if(led0pwmval==0)dir=1;							 
//		TIM_SetCompare2(TIM3,led0pwmval);		
        if(tempwm==10) D2=~D2;
		if(tempwm==1||tempwm==500) D4=~D4;
/*******软件 按键扫描 *******/
      keynum=KEY_Scan(0);
		switch (keynum)
		{
		  case 1 : printf("KEY1 \r\n");break;
		  case 2 : printf("KEY2 \r\n");break;
		  case 3 : printf("WK_UP \r\n");break;
		}
		
	}
		 
}






