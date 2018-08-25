/********************************************************
修改历史
20180401：
20180503：使模式切换后Speed_Moto_Control依然可用
****************************************************/
#include "motorthree.h"
#include "math.h"
#define VX_VALUE           (0.5f)
#define VY_VALUE           (sqrt(3)/2.f)      
#define L_value            (20*0.01f)  
//#define RADIUS_value       (1.0/12.5*0.01f)

static u8 ModeofNow;
void Speed_Moto_Control(float vx,float vy,float vz)   ////其中vx表示x方向的速度，vy表示y方向的速度，vz表示以电机轴线为中心的角速度！！！
{
	    short motor_one,motor_two,motor_three;
	    vy=(-vy);
        motor_one   = (-VX_VALUE*vx + VY_VALUE*vy + L_value*vz);
        motor_two   = (-VX_VALUE*vx - VY_VALUE*vy + L_value*vz);
        motor_three = (vx + L_value*vz);
	
		switch(ModeofNow)
		{
			case 0x03 :
					//速度模式
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,motor_one);
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,motor_two);
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,motor_three);
				break;
			case 0x04 :
				   //位置
				CAN_RoboModule_DRV_Position_Mode(0,1,2000,motor_one);	
				CAN_RoboModule_DRV_Position_Mode(0,2,2000,motor_two);
				CAN_RoboModule_DRV_Position_Mode(0,3,2000,motor_three);
				break;
			case 0x06 :
				   //电流速度
				CAN_RoboModule_DRV_Current_Velocity_Mode(0,1,3500,(short)motor_one);	
				CAN_RoboModule_DRV_Current_Velocity_Mode(0,2,3500,(short)motor_two);	
				CAN_RoboModule_DRV_Current_Velocity_Mode(0,3,3500,(short)motor_three);	
				break;
			default : break;
		}
	//printf("x: %d y: %d  z:%d \t\n",motor_one,motor_two,motor_three);
}
void car_3lun_Init(u8 mode,u8 t)
{
	ModeofNow=mode;
    CAN1_Configuration();        //CAN1初始化	
    delay_ms(300);                                      //刚开始要有足够的延时，确保驱动器已经初始化完成 
    CAN_RoboModule_DRV_Reset(0,0);                      //对0组所有驱动器进行复位 
    
    delay_ms(400);                                      //发送复位指令后的延时必须要有，等待驱动器再次初始化完成
    
    CAN_RoboModule_DRV_Config(0,1,t,0);               //1号驱动器配置为tms传回一次数据
    delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
    CAN_RoboModule_DRV_Config(0,2,t,0);               //2号驱动器配置为tms传回一次数据
    delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
    CAN_RoboModule_DRV_Config(0,3,t,0);               //3号驱动器配置为tms传回一次数据

    CAN_RoboModule_DRV_Mode_Choice(0,0,mode);  //0组的所有驱动器 都进入mode模式
    delay_ms(500);          	//发送模式选择指令后，要等待驱动器进入模式就绪。所以延时也不可以去掉。

}

u8 turn90(int k)
{
	int tempa=k*61700;
		car_3lun_Init(Position_Mode,100);

		CAN_RoboModule_DRV_Position_Mode(0,1,2000,tempa);	
		CAN_RoboModule_DRV_Position_Mode(0,2,2000,tempa);
		CAN_RoboModule_DRV_Position_Mode(0,3,2000,tempa);	
	while(!(Real_Position_Value[1]<(tempa+10)&&Real_Position_Value[1]>(tempa-10)));
	    car_3lun_Init(Velocity_Mode,0);
	return 0;
}


///**
//  * @brief  标准单位速度转化为脉冲速度
//  * @param  vel:速度 mm/s
//  * @retval velpulse:速度 脉冲/s
//  */
//static float Vel2Pulse(float vel)
//{
//	float pulse = 0.0f;
//	pulse = vel / (2.0f * PI * WHEEL_RADIUS) * PULSE_PER_CIRCLE * REDUCTION;
//	return pulse;
//}
///**
//  * @brief  脉冲速度转化为标准单位速度
//  * @param  pulse:速度 脉冲/s
//  * @retval vel:速度 mm/s
//  */
//static float Pulse2Vel(float pulse)
//{
//	float vel = 0.0f;
//	vel = pulse * (2.0f * PI * WHEEL_RADIUS) / REDUCTION / PULSE_PER_CIRCLE;
//	return vel;
//}
