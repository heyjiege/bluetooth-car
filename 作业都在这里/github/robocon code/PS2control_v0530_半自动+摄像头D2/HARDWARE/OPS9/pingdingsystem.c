/***********************************************************
文件名： pingdingsystem.c     pingdingsystem.h

版    本：v5.B   --发行版   --送球机器人定制版
修改历史：
20180419v4:将位置PID的参数改为宏定义，利用宏定义来控制速度与对应的PID参数
20180426v4.1:加入了坐标区域判定，用以防止撞墙(部分)
20180428v4.6:修正了Setzuobiao后  （-180+Zbais<GetnowZ<180+Zbais） 的BUG。    --》修改了Getzuobiao()函数
20180405v4.7:加入了按距离比例给x、y方向配速的定义 。可在头文件中开启或关闭。
20180510v5.1:使用新的配速函数，不再单纯的使用PID，设置了PD_EA使能开关
20180518v5.2:修改了配速关系，PD_EA=0时，使用比例配速，PD_EA=1时，由PD来配速
20180521v5.3:修改了PD参数，提高快速性
			bug隐患：Setzuobiao不能在任意位置Set，必须是90*k，k=0，1，2，-1，-2
			
20180528v5.B:PD_EA作为XY轴的PD总开关，且加入了X_PD_EA与Y_PD_EA子开关。
             角度控制从Robomoveto（）中分离出来封装成controlangle()
************************************************************/
#include "pingdingsystem.h"
#include "led.h"
#include "key.h"       

u8 RoundChoose(void);  //返回1，按默认方向走  返回0 ，需反转PwmZ


#if (MAXpwm==800)
#define Position_KP 5
#define Position_KI 0
#define Position_KD 10

#define Positio_KP 5
#define Positio_KI 0
#define Positio_KD 10

#define Direction_KP 50
#define Direction_KI 0
#define Direction_KD 60

#define Direction2_KP 50
#define Direction2_KI 0
#define Direction2_KD 100
#endif 
#if (MAXpwm==1500)
#define Position_KP 5
#define Position_KI 0
#define Position_KD 10

#define Positio_KP 5
#define Positio_KI 0
#define Positio_KD 10

#define Direction_KP 100
#define Direction_KI 0
#define Direction_KD 50

#define Direction2_KP 100
#define Direction2_KI 0
#define Direction2_KD 150
#endif 
#if (MAXpwm==2500)

#define Position_KP 5
#define Position_KI 0
#define Position_KD 10

#define Positio_KP 5
#define Positio_KI 0
#define Positio_KD 10

#define Direction_KP 90
#define Direction_KI 0
#define Direction_KD 100

#define Direction2_KP 250
#define Direction2_KI 0
#define Direction2_KD 100
#endif 
#if (MAXpwm==3500)

#define Position_KP 4
#define Position_KI 0
#define Position_KD 5 

#define Positio_KP 4
#define Positio_KI 0
#define Positio_KD 5

#define Direction_KP 250
#define Direction_KI 0
#define Direction_KD 100

#define Direction2_KP 250
#define Direction2_KI 0
#define Direction2_KD 100
#endif
#if (MAXpwm==6000)

#define Position_KP 4
#define Position_KI 0
#define Position_KD 8

#define Positio_KP 4
#define Positio_KI 0
#define Positio_KD 8

#define Direction_KP 250
#define Direction_KI 0
#define Direction_KD 100

#define Direction2_KP 300
#define Direction2_KI 0.01
#define Direction2_KD 0
#endif
int posX,posY,angleZ; //静态变量，存放目标坐标
int dposX=0,dposY=0;
_Bool PD_EA=0,PD_EA2=1;    // XY轴PD总开关
_Bool PD_angle_EA=1;//角度PD总开关
_Bool X_PD_EA=1;  // X轴PD子开关
_Bool Y_PD_EA=1;  // X轴PD子开关
_Bool AnglePID_EA=0; //角度PID分级标志
#ifdef USE_xiaozhun
static int Xbais=0,Ybais=0,Zbais=0;  //坐标偏差

int GetZuobiao(u8 c)  //获取坐标值  1 x轴 2 y轴  3 角度
{
	int tempxyz=0;
	switch (c)
	{
		case 1 : tempxyz=GetPositionX()+Xbais;break;
		case 2 : tempxyz=GetPositionY()+Ybais;break;
		case 3 : 
				 if(Zbais<0) 
				 {
					if(GetAngle()<(-180-Zbais))tempxyz=360+GetAngle()+Zbais;
					else tempxyz=GetAngle()+Zbais;
				 }
				 else if(Zbais>0)
				 {
					if(GetAngle()>(180-Zbais))tempxyz=-360+GetAngle()+Zbais;
					else tempxyz=GetAngle()+Zbais;
				 }
				 else tempxyz=GetAngle()+Zbais;
				 break;
	    default : break;
	}
	return tempxyz;
}
void SetZuobiao(int x,int y,int z)   //重置坐标系
{
    Xbais=x-GetPositionX();
	Ybais=y-GetPositionY();
	Zbais=z-GetAngle();
}
#endif

void toXY(int x,int y,int z)
{
    posX=x;
	posY=y;
	angleZ=z;
}
void GotoXY(sanVTypeDef v)
{
    posX=v.x;
	posY=v.y;
	angleZ=v.a;
}

u8 CherkZitai() //检查是否到达目标位置  允许误差+ -10mm     + -3度
{
	
	u8 temp;
	int xtemp=GetnowX;
	int ytemp=GetnowY;
	int ztemp=GetnowZ;
    if(xtemp>(posX-10)&&xtemp<(posX+10) && ytemp>(posY-10)&&ytemp<(posY+10) && ztemp>(angleZ-3)&&ztemp<(angleZ+3))     
   {
     delay_ms(100);
      if(xtemp>(posX-10)&&xtemp<(posX+10) && ytemp>(posY-10)&&ytemp<(posY+10) && ztemp>(angleZ-3)&&ztemp<(angleZ+3))     
	   {
	     temp=1;
	   } 
   }
   else  temp=0;
   return temp;
}
u8 CherkZuobiao(u16 bais) //检查是否到达目标位置  允许误差+ -bais  mm     
{
	u8 temp;
	int xtemp=GetnowX;
	int ytemp=GetnowY;
    if(xtemp>(posX-bais) && xtemp<(posX+bais) && ytemp>(posY-bais) && ytemp<(posY+bais))     
   {
     delay_ms(10);
       if(xtemp>(posX-bais) && xtemp<(posX+bais) && ytemp>(posY-bais) && ytemp<(posY+bais)) 
	   {
	     temp= 1;
	   }
   }
   else temp= 0;
   return temp;
}

float changeCankaoxi_Y(float x,float y);
float changeCankaoxi_X(float x,float y);
float PwmX,PwmY,PwmZ;
     static float Bias;   //中间变量
	 static float Last_YBias,Integral_Ybias;
	 static float Last_XBias,Integral_Xbias;
	 static float Last_ZBias,Integral_Zbias;
void RobomoveTo()
{	
#ifdef BiLiPeiSpeed
if(PD_EA&&PD_EA2)
{	
	 int chaX,chaY;
	 float kpX,kpY;
	posX=posX+dposX;
	posY=posY+dposY;
     chaX=posX-GetnowX;
     chaY=posY-GetnowY;
	 
	kpX=chaX/(sqrt(chaX*chaX+chaY*chaY));
	kpY=chaY/(sqrt(chaX*chaX+chaY*chaY));
//判断X速度方向
    if(chaX>10)PwmX=MAXpwm-50;
	else if(chaX<(-10))PwmX=MAXpwm-50;
    else PwmX=0;

//判断Y速度方向
    if(chaY>10)PwmY=MAXpwm-50;
	else if(chaY<(-10))PwmY=MAXpwm-50;
    else PwmY=0;
      //按比例配速
     PwmX=PwmX*kpX;
	 PwmY=PwmY*kpY;	
#else
//if(PD_EA)
//{
#endif	
    /*PID Y轴位置*/
	if(Y_PD_EA)
	{	
	Bias=posY-GetnowY;                                  //计算Y轴偏差
	 Integral_Ybias+=Bias;	                                 //求出偏差的积分
	 PwmY=Position_KP*Bias+Position_KI*Integral_Ybias+Position_KD*(Bias-Last_YBias);       //位置式PID控制器
	 Last_YBias=Bias;  	//保存上一次偏差 
	}
	if(X_PD_EA)
	{
	/*PID X轴位置*/	
		Bias=posX-GetnowX;                                  //计算X轴偏差
		 Integral_Xbias+=Bias;	                                 //求出偏差的积分
		 PwmX=Position_KP*Bias+Position_KI*Integral_Xbias+Position_KD*(Bias-Last_XBias);       //位置式PID控制器
		 Last_XBias=Bias; 
	}
}
else{PwmX=0;PwmY=0;}

#ifdef BiLiPeiSpeed  //按比例配速
//	PwmX=abs(PwmX);
//	PwmY=abs(PwmY);
//     PwmX=PwmX*kpX;
//	 PwmY=PwmY*kpY;	
//  printf("kpX:%f  kpY:%f  \r\n\r\n",kpX,kpY);  //调试使用
#endif
        //SetSpeed();
}
void ControlAngle()
{
	int chaA;
	chaA=angleZ-GetnowZ;
	
	if(abs(chaA)<5)AnglePID_EA=1;
	else AnglePID_EA=0;
if(1)
{
	if(!AnglePID_EA)
	{
	/*PID Z轴位置*/	
		 Bias=angleZ-GetnowZ;                                  //计算Z轴偏差
		 Integral_Zbias+=Bias;	                                 //求出偏差的积分
		 PwmZ=Direction_KP*Bias+Direction_KI*Integral_Zbias+Direction_KD*(Bias-Last_ZBias);       //位置式PID控制器
		 Last_ZBias=Bias;
	}
	else
	{
	/*PID Z轴位置   */	
		 Bias=angleZ-GetnowZ;                                  //计算Z轴偏差
		 Integral_Zbias+=Bias;	                                 //求出偏差的积分
		 PwmZ=Direction2_KP*Bias+Direction2_KI*Integral_Zbias+Direction2_KD*(Bias-Last_ZBias);       //位置式PID控制器
		 Last_ZBias=Bias;
	}
}
	 if(RoundChoose()==0)PwmZ=-PwmZ;
}
void SetSpeed()
{
    /*限幅*/
    if(PwmX>MAXpwm)PwmX=MAXpwm;
	if(PwmX<(-MAXpwm))PwmX=(-MAXpwm);
    if(PwmY>MAXpwm)PwmY=MAXpwm;
	if(PwmY<(-MAXpwm))PwmY=(-MAXpwm);
    if(PwmZ>MAXpwm*2)PwmZ=MAXpwm*2;
	if(PwmZ<(-MAXpwm*2))PwmZ=(-MAXpwm*2);
  
	 Speed_Moto_Control(changeCankaoxi_X(PwmX,PwmY),changeCankaoxi_Y(PwmX,PwmY),PwmZ);//带方向矫正
	delay_us(500);
	 Speed_Moto_Control(changeCankaoxi_X(PwmX,PwmY),changeCankaoxi_Y(PwmX,PwmY),PwmZ);//带方向矫正
	
//	 printf("PwmX:%0.2f  PwmY:%0.2f  ",PwmX,PwmY);  //调试使用
}
float changeCankaoxi_X(float x,float y)
{
   float temp,angle;
    angle=(Pi/180)*(-GetnowZ); //获取角度	

 	temp=x*cos(angle)-y*sin(angle); 
	return temp;
}
float changeCankaoxi_Y(float x,float y)
{
   float temp,angle;
    angle=(Pi/180)*(-GetnowZ); //获取角度	

	temp=(x)*sin(angle)+y*cos(angle);  
	return temp;
}
//旋转优化
u8 RoundChoose()  //返回1，按默认方向走  返回0 ，需反转PwmZ
{
	/*
	1.先判断两者在哪一边   if不同边则 选择向夹角小的方向走
                          else 同边则 按默认方向走
	2.输出
	*/
	int temp1,temp2;
    temp1=angleZ;
	temp2=GetnowZ;
	if((temp1*temp2)<0)   //两者之积<0,不同边
	{
	   
		if(abs(temp1)+abs(temp2)<180)return 1;
		else return 0;
	}
	else return 1;
}








