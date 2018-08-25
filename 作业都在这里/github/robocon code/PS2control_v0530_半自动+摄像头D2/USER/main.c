/************************************************************************************
  ============= STM32F103ZET6 �ۺ��͹���ģ��============
  ����Ӳ���� 		   
  �ڲ����ã�
  ������ܣ�
		   
  ��    �ߣ�Janson	����ԭ��	             
*************************************************************************************/
#include "stm32f10x.h"
#include "sys.h"       //ϵͳ�жϷ������û�	��NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 

#include "ST7735.h"

#include "exti.h"

int main(void)
{	
    u8 keynum;//��� ����ɨ����
	u16 tempwm=0;
//	u8 dir=1;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	delay_init();
	LED_Init();

    uart_init(115200);	 //���ڳ�ʼ��Ϊ9600

	KEY_Init();         	//��ʼ���밴�����ӵ�Ӳ���ӿ� 	
//	KEY_EXTIX_Init();		 	//�ⲿ�жϰ�����ʼ��	
//	TIM3_PWM_Init(899,0);	 //����Ƶ��PWMƵ��=72000/900=8Khz

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
/*******��� ����ɨ�� *******/
      keynum=KEY_Scan(0);
		switch (keynum)
		{
		  case 1 : printf("KEY1 \r\n");break;
		  case 2 : printf("KEY2 \r\n");break;
		  case 3 : printf("WK_UP \r\n");break;
		}
		
	}
		 
}






