#ifndef _pingdingsystem_H_
#define _pingdingsystem_H_
/*
��ʷ��20180518
*/
//***��λ������ļ�
#include "ops9.h"
//***�û�/Debugͷ�ļ�  *** ����Ҫʱ��ɾ��
#include "led.h"       //LED   ȡ 0 ʱ��
#include "key.h"  
#include "usart.h"     //Ĭ��Ϊ RX1=PA9 TX1=PA10  
#include "ST7735.h"
//***��Ҫͷ�ļ�
#include <math.h>
#include "delay.h"       //��ʱ����   delay_us(int);   delay_ms(int);
#include "motorthree.h"  //����ٶȷ���

#define Pi (3.1415926f)

//�û�����
#define USE_xiaozhun   //��������У׼/����
#define MAXpwm 6000    //�ٶ��޷�   800/1500/2500 /3500           ����������ٶ�
#define BiLiPeiSpeed  //����������x,y

extern int posX,posY,angleZ; //���Ŀ������
extern int dposX,dposY;
extern float PwmX,PwmY,PwmZ;

extern _Bool PD_EA,PD_EA2; //XY PD�ܿ���
extern _Bool PD_angle_EA;//�Ƕ�PD�ܿ���
extern _Bool X_PD_EA; //X�� PD�ӿ���
extern _Bool Y_PD_EA; //Y�� PD�ӿ���
void RobomoveTo(void);
void toXY(int x,int y,int z);
void ControlAngle(void);
u8 CherkZitai(void); //����Ƿ񵽴�Ŀ��λ��  �������+ -10mm     + -2��
u8 CherkZuobiao(u16 bais); //����Ƿ񵽴�Ŀ��λ��  �������+ -bais  mm 
void SetSpeed(void);


#ifdef USE_xiaozhun         //�����������У׼
int GetZuobiao(u8 c);  //��ȡ����ֵ  //��ȡ����ֵ  1 x�� 2 y��  3 �Ƕ�
void SetZuobiao(int x,int y,int z);   //��������ϵ

#define GetnowX -GetZuobiao(1)
#define GetnowY -GetZuobiao(2)
//#define GetnowZ GetZuobiao(3)
#define GetnowZ GetAngle()
#else

#define GetnowX GetPositionX()
#define GetnowY GetPositionY()
#define GetnowZ GetAngle()

#endif

typedef struct   //������������Ľṹ��
{
  int16_t x;
  int16_t y;
  int16_t a;
}sanVTypeDef;
void GotoXY(sanVTypeDef v);



#if 0
typedef struct   //���巽������Ľṹ��
{
  int16_t x1;
  int16_t y1;
  int16_t x2;
  int16_t y2; 
}fangkuaiTypeDef;
void cherkarea(void);
#endif

#endif


