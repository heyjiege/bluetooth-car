
#ifndef	__PS2_H__
#define __PS2_H__


sbit  DI=P1^0;	
sbit  D0=P1^1;	
sbit  CS=P1^2;	
sbit  CLK=P1^3;	


#define uchar unsigned char
#define uint unsigned int

#define DO_H D0=1        
#define DO_L D0=0        

#define CS_H CS=1      
#define CS_L CS=0       

#define CLK_H CLK=1     
#define CLK_L CLK=0     

#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16

#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      16




#define PSS_RX 5                //��ҡ��X������
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8


uchar PS2_RedLight(void);   //�ж��Ƿ�Ϊ���ģʽ
void PS2_ReadData(void); //���ֱ�����
void PS2_Cmd(uchar CMD);		  //���ֱ���������
uchar PS2_DataKey(void);		  //����ֵ��ȡ
uchar PS2_AnologData(uchar button); //�õ�һ��ҡ�˵�ģ����
void PS2_ClearData(void);	  //������ݻ�����
void PS2_Vibration(uchar motor1, uchar motor2);//������motor1  0xFF���������أ�motor2  0x40~0xFF
void PS2_EnterConfing(void);	 //��������
void PS2_TurnOnAnalogMode(void); //����ģ����
void PS2_VibrationMode(void);    //������
void PS2_ExitConfing(void);	     //�������
void PS2_SetInit(void);		     //���ó�ʼ��


#include "ps2.c"

#endif

