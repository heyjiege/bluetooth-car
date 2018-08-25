/***********************************************************
�ļ����� pingdingsystem.c     pingdingsystem.h

��    ����v5.B   --���а�   --��������˶��ư�
�޸���ʷ��
20180419v4:��λ��PID�Ĳ�����Ϊ�궨�壬���ú궨���������ٶ����Ӧ��PID����
20180426v4.1:���������������ж������Է�ֹײǽ(����)
20180428v4.6:������Setzuobiao��  ��-180+Zbais<GetnowZ<180+Zbais�� ��BUG��    --���޸���Getzuobiao()����
20180405v4.7:�����˰����������x��y�������ٵĶ��� ������ͷ�ļ��п�����رա�
20180510v5.1:ʹ���µ����ٺ��������ٵ�����ʹ��PID��������PD_EAʹ�ܿ���
20180518v5.2:�޸������ٹ�ϵ��PD_EA=0ʱ��ʹ�ñ������٣�PD_EA=1ʱ����PD������
20180521v5.3:�޸���PD��������߿�����
			bug������Setzuobiao����������λ��Set��������90*k��k=0��1��2��-1��-2
			
20180528v5.B:PD_EA��ΪXY���PD�ܿ��أ��Ҽ�����X_PD_EA��Y_PD_EA�ӿ��ء�
             �Ƕȿ��ƴ�Robomoveto�����з��������װ��controlangle()
************************************************************/
#include "pingdingsystem.h"
#include "led.h"
#include "key.h"       

u8 RoundChoose(void);  //����1����Ĭ�Ϸ�����  ����0 ���跴תPwmZ


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
int posX,posY,angleZ; //��̬���������Ŀ������
int dposX=0,dposY=0;
_Bool PD_EA=0,PD_EA2=1;    // XY��PD�ܿ���
_Bool PD_angle_EA=1;//�Ƕ�PD�ܿ���
_Bool X_PD_EA=1;  // X��PD�ӿ���
_Bool Y_PD_EA=1;  // X��PD�ӿ���
_Bool AnglePID_EA=0; //�Ƕ�PID�ּ���־
#ifdef USE_xiaozhun
static int Xbais=0,Ybais=0,Zbais=0;  //����ƫ��

int GetZuobiao(u8 c)  //��ȡ����ֵ  1 x�� 2 y��  3 �Ƕ�
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
void SetZuobiao(int x,int y,int z)   //��������ϵ
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

u8 CherkZitai() //����Ƿ񵽴�Ŀ��λ��  �������+ -10mm     + -3��
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
u8 CherkZuobiao(u16 bais) //����Ƿ񵽴�Ŀ��λ��  �������+ -bais  mm     
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
     static float Bias;   //�м����
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
//�ж�X�ٶȷ���
    if(chaX>10)PwmX=MAXpwm-50;
	else if(chaX<(-10))PwmX=MAXpwm-50;
    else PwmX=0;

//�ж�Y�ٶȷ���
    if(chaY>10)PwmY=MAXpwm-50;
	else if(chaY<(-10))PwmY=MAXpwm-50;
    else PwmY=0;
      //����������
     PwmX=PwmX*kpX;
	 PwmY=PwmY*kpY;	
#else
//if(PD_EA)
//{
#endif	
    /*PID Y��λ��*/
	if(Y_PD_EA)
	{	
	Bias=posY-GetnowY;                                  //����Y��ƫ��
	 Integral_Ybias+=Bias;	                                 //���ƫ��Ļ���
	 PwmY=Position_KP*Bias+Position_KI*Integral_Ybias+Position_KD*(Bias-Last_YBias);       //λ��ʽPID������
	 Last_YBias=Bias;  	//������һ��ƫ�� 
	}
	if(X_PD_EA)
	{
	/*PID X��λ��*/	
		Bias=posX-GetnowX;                                  //����X��ƫ��
		 Integral_Xbias+=Bias;	                                 //���ƫ��Ļ���
		 PwmX=Position_KP*Bias+Position_KI*Integral_Xbias+Position_KD*(Bias-Last_XBias);       //λ��ʽPID������
		 Last_XBias=Bias; 
	}
}
else{PwmX=0;PwmY=0;}

#ifdef BiLiPeiSpeed  //����������
//	PwmX=abs(PwmX);
//	PwmY=abs(PwmY);
//     PwmX=PwmX*kpX;
//	 PwmY=PwmY*kpY;	
//  printf("kpX:%f  kpY:%f  \r\n\r\n",kpX,kpY);  //����ʹ��
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
	/*PID Z��λ��*/	
		 Bias=angleZ-GetnowZ;                                  //����Z��ƫ��
		 Integral_Zbias+=Bias;	                                 //���ƫ��Ļ���
		 PwmZ=Direction_KP*Bias+Direction_KI*Integral_Zbias+Direction_KD*(Bias-Last_ZBias);       //λ��ʽPID������
		 Last_ZBias=Bias;
	}
	else
	{
	/*PID Z��λ��   */	
		 Bias=angleZ-GetnowZ;                                  //����Z��ƫ��
		 Integral_Zbias+=Bias;	                                 //���ƫ��Ļ���
		 PwmZ=Direction2_KP*Bias+Direction2_KI*Integral_Zbias+Direction2_KD*(Bias-Last_ZBias);       //λ��ʽPID������
		 Last_ZBias=Bias;
	}
}
	 if(RoundChoose()==0)PwmZ=-PwmZ;
}
void SetSpeed()
{
    /*�޷�*/
    if(PwmX>MAXpwm)PwmX=MAXpwm;
	if(PwmX<(-MAXpwm))PwmX=(-MAXpwm);
    if(PwmY>MAXpwm)PwmY=MAXpwm;
	if(PwmY<(-MAXpwm))PwmY=(-MAXpwm);
    if(PwmZ>MAXpwm*2)PwmZ=MAXpwm*2;
	if(PwmZ<(-MAXpwm*2))PwmZ=(-MAXpwm*2);
  
	 Speed_Moto_Control(changeCankaoxi_X(PwmX,PwmY),changeCankaoxi_Y(PwmX,PwmY),PwmZ);//���������
	delay_us(500);
	 Speed_Moto_Control(changeCankaoxi_X(PwmX,PwmY),changeCankaoxi_Y(PwmX,PwmY),PwmZ);//���������
	
//	 printf("PwmX:%0.2f  PwmY:%0.2f  ",PwmX,PwmY);  //����ʹ��
}
float changeCankaoxi_X(float x,float y)
{
   float temp,angle;
    angle=(Pi/180)*(-GetnowZ); //��ȡ�Ƕ�	

 	temp=x*cos(angle)-y*sin(angle); 
	return temp;
}
float changeCankaoxi_Y(float x,float y)
{
   float temp,angle;
    angle=(Pi/180)*(-GetnowZ); //��ȡ�Ƕ�	

	temp=(x)*sin(angle)+y*cos(angle);  
	return temp;
}
//��ת�Ż�
u8 RoundChoose()  //����1����Ĭ�Ϸ�����  ����0 ���跴תPwmZ
{
	/*
	1.���ж���������һ��   if��ͬ���� ѡ����н�С�ķ�����
                          else ͬ���� ��Ĭ�Ϸ�����
	2.���
	*/
	int temp1,temp2;
    temp1=angleZ;
	temp2=GetnowZ;
	if((temp1*temp2)<0)   //����֮��<0,��ͬ��
	{
	   
		if(abs(temp1)+abs(temp2)<180)return 1;
		else return 0;
	}
	else return 1;
}








