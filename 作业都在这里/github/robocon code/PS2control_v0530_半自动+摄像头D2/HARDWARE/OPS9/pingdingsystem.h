#ifndef _pingdingsystem_H_
#define _pingdingsystem_H_
/*
历史：20180518
*/
//***定位器相关文件
#include "ops9.h"
//***用户/Debug头文件  *** 不需要时可删除
#include "led.h"       //LED   取 0 时亮
#include "key.h"  
#include "usart.h"     //默认为 RX1=PA9 TX1=PA10  
#include "ST7735.h"
//***必要头文件
#include <math.h>
#include "delay.h"       //延时函数   delay_us(int);   delay_ms(int);
#include "motorthree.h"  //电机速度分配

#define Pi (3.1415926f)

//用户配置
#define USE_xiaozhun   //开启坐标校准/重置
#define MAXpwm 6000    //速度限幅   800/1500/2500 /3500           机器人最大速度
#define BiLiPeiSpeed  //按比例配速x,y

extern int posX,posY,angleZ; //存放目标坐标
extern int dposX,dposY;
extern float PwmX,PwmY,PwmZ;

extern _Bool PD_EA,PD_EA2; //XY PD总开关
extern _Bool PD_angle_EA;//角度PD总开关
extern _Bool X_PD_EA; //X轴 PD子开关
extern _Bool Y_PD_EA; //Y轴 PD子开关
void RobomoveTo(void);
void toXY(int x,int y,int z);
void ControlAngle(void);
u8 CherkZitai(void); //检查是否到达目标位置  允许误差+ -10mm     + -2度
u8 CherkZuobiao(u16 bais); //检查是否到达目标位置  允许误差+ -bais  mm 
void SetSpeed(void);


#ifdef USE_xiaozhun         //如果开启坐标校准
int GetZuobiao(u8 c);  //获取坐标值  //获取坐标值  1 x轴 2 y轴  3 角度
void SetZuobiao(int x,int y,int z);   //重置坐标系

#define GetnowX -GetZuobiao(1)
#define GetnowY -GetZuobiao(2)
//#define GetnowZ GetZuobiao(3)
#define GetnowZ GetAngle()
#else

#define GetnowX GetPositionX()
#define GetnowY GetPositionY()
#define GetnowZ GetAngle()

#endif

typedef struct   //定义三轴坐标的结构体
{
  int16_t x;
  int16_t y;
  int16_t a;
}sanVTypeDef;
void GotoXY(sanVTypeDef v);



#if 0
typedef struct   //定义方块区域的结构体
{
  int16_t x1;
  int16_t y1;
  int16_t x2;
  int16_t y2; 
}fangkuaiTypeDef;
void cherkarea(void);
#endif

#endif


