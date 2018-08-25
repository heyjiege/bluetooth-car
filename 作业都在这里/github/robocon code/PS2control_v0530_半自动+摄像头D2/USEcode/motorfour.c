/***********************************
文件：motorfour.c   motorfour.h
说明： 四轮全向轮底盘电机速度计算

版本、修改日期  v201804018
*************************************/

#include "motorfour.h"
#include "math.h"
#define VX_VALUE           (0.5f)
#define VY_VALUE           (sqrt(3)/2.f)      
#define L_value            (20*0.01f)  
#define RADIUS_value       (1.0/12.5*0.01f)

//#define j1 (5*3.1415/6f)
//#define j2 (5*3.1415/4f)
//#define j3 (7*3.1415/4f)
//#define j4 (3.1415/6f)
#define cosj1 (0.9989562f)
#define cosj2 (0.9976521f)
#define cosj3 (0.9953998f)
#define cosj4 (0.9999582f)
#define sinj1 (0.0456767f)
#define sinj2 (0.0684485f)
#define sinj3 (0.9580731f)
#define sinj4 (0.0091383f)
//cos(pi/4)=0.99990605f
//sin(pi/4)=0.01370735f
void Speed_Moto_Control(float vx,float vy,float vz)   ////其中vx表示x方向的速度，vy表示y方向的速度，vz表示以电机轴线为中心的角速度！！！
{
	    int motor_one,motor_two,motor_three,motor_four;
	
        motor_one   = (-cosj1*vx + sinj1*vy + L_value*vz);
        motor_two   = (-cosj2*vx + sinj2*vy + L_value*vz);
        motor_three = (-cosj3*vx + sinj4*vy + L_value*vz);
        motor_four  = (-cosj4*vx + sinj5*vy + L_value*vz);
//		CAN_RoboModule_DRV_OpenLoop_Mode(0,1,motor_one);
//		CAN_RoboModule_DRV_OpenLoop_Mode(0,2,motor_two);
//		CAN_RoboModule_DRV_OpenLoop_Mode(0,3,motor_three);
CAN_RoboModule_DRV_Velocity_Mode(0,1,3000,motor_one);
CAN_RoboModule_DRV_Velocity_Mode(0,2,3000,motor_two);
CAN_RoboModule_DRV_Velocity_Mode(0,3,3000,motor_three);	
CAN_RoboModule_DRV_Velocity_Mode(0,4,3000,motor_four);	
//printf("x: %d y: %d  z:%d \t\n",motor_one,motor_two,motor_three);
}
void car_4lun_Init(u8 mode,u8 t)
{
    CAN1_Configuration();        //CAN1初始化	
    delay_ms(500);                                      //刚开始要有足够的延时，确保驱动器已经初始化完成 
    CAN_RoboModule_DRV_Reset(0,0);                      //对0组所有驱动器进行复位 
    
    delay_ms(500);                                      //发送复位指令后的延时必须要有，等待驱动器再次初始化完成
    
    CAN_RoboModule_DRV_Config(0,1,t,0);               //1号驱动器配置为tms传回一次数据
    delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
    CAN_RoboModule_DRV_Config(0,2,t,0);               //2号驱动器配置为tms传回一次数据
    delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
    CAN_RoboModule_DRV_Config(0,3,t,0);               //3号驱动器配置为tms传回一次数据
    delay_us(200);                                      //此处延时为了不让传回数据时候4个不一起传
    CAN_RoboModule_DRV_Config(0,4,t,0);               //4号驱动器配置为tms传回一次数据
	
    CAN_RoboModule_DRV_Mode_Choice(0,0,mode);  //0组的所有驱动器 都进入开环模式
    delay_ms(500);          	//发送模式选择指令后，要等待驱动器进入模式就绪。所以延时也不可以去掉。
}