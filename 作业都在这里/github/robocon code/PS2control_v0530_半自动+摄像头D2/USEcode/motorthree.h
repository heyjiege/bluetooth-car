#ifndef _motorthree_H_
#define _motorthree_H_

#include "sys.h"
extern u8 ModeofNow;  //保存当前驱动器所设置的模式
void Speed_Moto_Control(float vx,float vy,float vz);   ////其中vx表示x方向的速度，vy表示y方向的速度，vz表示以电机轴线为中心的角速度！
void car_3lun_Init(u8 mode,u8 t);
//void hengyiVP(int x);

u8 turn90(int k);

/**/
//角度制转换为弧度制系数
#define CHANGE_TO_RADIAN    (0.017453f)   
//弧度制转换为角度制系数
#define CHANGE_TO_ANGLE     (57.2958f)				
//圆周率
#define PI                  (3.1415926f)

#define NULL 0

//减速比
#define REDUCTION (1.0f)
//车轮半径 单位:mm
#define WHEEL_RADIUS (50.0f)
//每圈脉冲数�
#define PULSE_PER_CIRCLE (4096.0f)

static float Pulse2Vel(float pulse);//脉冲速度转化为标准单位速度
static float Vel2Pulse(float vel);//标准单位速度转化为脉冲速度
#endif


