/************************************************************************************
   @Janson
�޸���ʷ��
20180528:
*************************************************************************************/
#include "stm32f10x.h"
#include "sys.h" 
#include "ops9.h"
#include "pingdingsystem.h"
#include "Shoudong_move.h"
//#include "OLED_I2C.h"  //Ĭ��Ϊ SCL=PB6  SDA=PB7
sanVTypeDef O2A,A2Yi,A2Er;
_Bool isOps9Rreay = 0;
extern u8 wheretogo;
void xyashow(void);
int main(void)
{
	
/************����������******************/
    u16 temptime;
//    u8 keynum;
O2A.x=0,O2A.y=-2900,O2A.a=0;  //ȡ������
A2Yi.x=-557;A2Yi.y=6150;A2Yi.a=90; //������ 1
A2Er.x=-363;A2Er.y=8010;A2Er.a=90;  //������ 2
/**********��ʼ��************************/
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  uart_init(38400);
	LED_Init();
	USART2_Init(115200); 
	USART3_Init(9600);
	//KEY_Init(); 
   Sencer_Init();  //IN_E2,3,4
  PS2_Init();			   //�����˿ڳ�ʼ��
	PS2_SetInit();		 //���ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
	
  car_3lun_Init(Velocity_Mode,0);  //0����������ʼ��Ϊ�ٶ�ģʽ��ÿ  ms�ش�һ�����ݣ�
	delay_ms(10);
   PS2_Init();			   //�����˿ڳ�ʼ��
	PS2_SetInit();		 //���ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�

	printf("��ʼ��PS2���. \r\n");	

while(isOps9Rreay == 0)//wait for ops9 ready
{
	//  USART_OUT(USART3, (uint8_t *)"��ʼ��OPS9��. \r\n");	
		 if(temptime==1)
		{
//      USART_OUT(DEBUG_USART, (uint8_t *)"��ʼ��OPS9��. \r\n");
		   printf("��ʼ��OPS9��. \r\n");		
		}

		 temptime++;
		 temptime=temptime%1000;
		 delay_ms(1);
}	
/*********��������ʼ��***********************/
  D2=1;D4=1;//�ر�LED
	printf("\r\n F103 is ready.\r\n\r\n");

	posX=0;posY=0;angleZ=0;  //�����ʼ��
	SetZuobiao(0,0,0);	

//	TIM3_Int_Init(999,7199);//��ʱ  ��һ��ϵ��x0.1ms	

	while(1)
	{
		switch(wheretogo)
		{
			case 0 : PD_EA2=0;break;
			case 1 : Shou_Move_0_1();break;  //ǰ��ȡ���� ȡ����
			case 2 : Shou_Move_1_11();break;  //��ȡ���� ǰ�� 1��
			case 3 : Shou_Move_11_1();break;  //ǰ��ȡ���� ȡ����
		    case 4 : Shou_Move_1_22();break;  //��ȡ���� ǰ�� 2��
			case 5 : Shou_Move_any_1(); break; 
			case 6 : Shou_Move_22_1(); break;
            case 7 : Shou_Move_any_11();break; //������ 1)
            case 8 : Shou_Move_any_22();break;				
			default : break;
		}
		printf("togo:%d |#|",wheretogo);
         RobomoveTo();//
		 PS2control();
				  if(GetnowY<(-1000)){IOA5=0;}//δ�뿪ʱ���ʱ��̧��
				if(PS2_flag)D2=0;
				else D2=1;
		        if(PD_EA2)D4=1;
				else D4=0;   //����ʱΪ�ֶ�����
		ControlAngle();
				if(IN_E2==1||IN_E3==1)if(PwmX<0)PwmX=0;
		if(IN_E7==1)if(PwmX>0)PwmX=0;//ps:Ч������ô��
		SetSpeed();
	//			printf("PwmX:%0.2f  PwmY:%0.2f  ",PwmX,PwmY);  //����ʹ��
	//		printf(" |#| posX:%d posY:%d angleZ:%d  |#|  ",posX,posY,angleZ);
	 //   xyashow();
		
	}
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		//����д�жϷ�������;

		}
}

void xyashow()
{
	short nowX = 0;
	short nowY = 0;
	short nowZ = 0;	
		nowX=GetnowX;
		nowY=GetnowY;
//        nowZ=GetZuobiao(3);	
//		nowX=GetPositionX();
//		nowY=GetPositionY();
      nowZ=GetAngle();	

				printf("nowX:%d nowY:%d  nowZ:%d   \r\n",nowX,nowY,nowZ);				
}

