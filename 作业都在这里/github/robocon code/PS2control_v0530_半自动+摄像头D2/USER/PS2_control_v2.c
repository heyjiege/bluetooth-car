/************************************************************************************
   @Janson
修改历史：
20180528:
*************************************************************************************/
#include "stm32f10x.h"
#include "sys.h" 
#include "ops9.h"
#include "pingdingsystem.h"
#include "Shoudong_move.h"
//#include "OLED_I2C.h"  //默认为 SCL=PB6  SDA=PB7
sanVTypeDef O2A,A2Yi,A2Er;
_Bool isOps9Rreay = 0;
extern u8 wheretogo;
void xyashow(void);
int main(void)
{
	
/************变量定义区******************/
    u16 temptime;
//    u8 keynum;
O2A.x=0,O2A.y=-2900,O2A.a=0;  //取红球区
A2Yi.x=-557;A2Yi.y=6150;A2Yi.a=90; //交接区 1
A2Er.x=-363;A2Er.y=8010;A2Er.a=90;  //交接区 2
/**********初始化************************/
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  uart_init(38400);
	LED_Init();
	USART2_Init(115200); 
	USART3_Init(9600);
	//KEY_Init(); 
   Sencer_Init();  //IN_E2,3,4
  PS2_Init();			   //驱动端口初始化
	PS2_SetInit();		 //配置初始化,配置“红绿灯模式”，并选择是否可以修改
	
  car_3lun_Init(Velocity_Mode,0);  //0组驱动器初始化为速度模式，每  ms回传一次数据，
	delay_ms(10);
   PS2_Init();			   //驱动端口初始化
	PS2_SetInit();		 //配置初始化,配置“红绿灯模式”，并选择是否可以修改

	printf("初始化PS2完成. \r\n");	

while(isOps9Rreay == 0)//wait for ops9 ready
{
	//  USART_OUT(USART3, (uint8_t *)"初始化OPS9中. \r\n");	
		 if(temptime==1)
		{
//      USART_OUT(DEBUG_USART, (uint8_t *)"初始化OPS9中. \r\n");
		   printf("初始化OPS9中. \r\n");		
		}

		 temptime++;
		 temptime=temptime%1000;
		 delay_ms(1);
}	
/*********控制量初始化***********************/
  D2=1;D4=1;//关闭LED
	printf("\r\n F103 is ready.\r\n\r\n");

	posX=0;posY=0;angleZ=0;  //坐标初始化
	SetZuobiao(0,0,0);	

//	TIM3_Int_Init(999,7199);//定时  第一个系数x0.1ms	

	while(1)
	{
		switch(wheretogo)
		{
			case 0 : PD_EA2=0;break;
			case 1 : Shou_Move_0_1();break;  //前往取球区 取红球
			case 2 : Shou_Move_1_11();break;  //从取球区 前往 1区
			case 3 : Shou_Move_11_1();break;  //前往取球区 取金球
		    case 4 : Shou_Move_1_22();break;  //从取球区 前往 2区
			case 5 : Shou_Move_any_1(); break; 
			case 6 : Shou_Move_22_1(); break;
            case 7 : Shou_Move_any_11();break; //交接区 1)
            case 8 : Shou_Move_any_22();break;				
			default : break;
		}
		printf("togo:%d |#|",wheretogo);
         RobomoveTo();//
		 PS2control();
				  if(GetnowY<(-1000)){IOA5=0;}//未离开时球架时不抬箱
				if(PS2_flag)D2=0;
				else D2=1;
		        if(PD_EA2)D4=1;
				else D4=0;   //灯亮时为手动控制
		ControlAngle();
				if(IN_E2==1||IN_E3==1)if(PwmX<0)PwmX=0;
		if(IN_E7==1)if(PwmX>0)PwmX=0;//ps:效果不怎么好
		SetSpeed();
	//			printf("PwmX:%0.2f  PwmY:%0.2f  ",PwmX,PwmY);  //调试使用
	//		printf(" |#| posX:%d posY:%d angleZ:%d  |#|  ",posX,posY,angleZ);
	 //   xyashow();
		
	}
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		//下面写中断服务内容;

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

