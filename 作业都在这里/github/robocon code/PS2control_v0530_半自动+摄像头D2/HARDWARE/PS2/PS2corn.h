/**************************
�޸���ʷ
20180503��
**********************/
#ifndef _PS2corn_H_
#define _PS2corn_H_
#include "sys.h"

extern int motor[3];
extern u8 PS2_flag;
extern _Bool DiveBall_EA;
#define NEW_PS2 //ʹ���µİ���������   ȫ���޳�
//#define zuoyouxian    //ʹ���ҡ���и������ȼ��������ɱ���ҡ����������ҡ��
#define DoubleBox    //ʹ��˫����

#ifdef NEW_PS2
u16 PS2_keyScan(void);
#endif

void yaoganfangwei(void);
void PS2control(void);


#ifdef ReadKeyStatus
typedef   enum
{
    NoKeyDown = 0,
    KeySureDown ,
    OnceKeyDown,
    ContiousKeyDown
}StateStatus;
StateStatus ReadKeyStatus(void);
#endif

#endif

