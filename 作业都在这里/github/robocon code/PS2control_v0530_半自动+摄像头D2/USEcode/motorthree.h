#ifndef _motorthree_H_
#define _motorthree_H_

#include "sys.h"
extern u8 ModeofNow;  //���浱ǰ�����������õ�ģʽ
void Speed_Moto_Control(float vx,float vy,float vz);   ////����vx��ʾx������ٶȣ�vy��ʾy������ٶȣ�vz��ʾ�Ե������Ϊ���ĵĽ��ٶȣ�
void car_3lun_Init(u8 mode,u8 t);
//void hengyiVP(int x);

u8 turn90(int k);

/**/
//�Ƕ���ת��Ϊ������ϵ��
#define CHANGE_TO_RADIAN    (0.017453f)   
//������ת��Ϊ�Ƕ���ϵ��
#define CHANGE_TO_ANGLE     (57.2958f)				
//Բ����
#define PI                  (3.1415926f)

#define NULL 0

//���ٱ�
#define REDUCTION (1.0f)
//���ְ뾶 ��λ:mm
#define WHEEL_RADIUS (50.0f)
//ÿȦ�������
#define PULSE_PER_CIRCLE (4096.0f)

static float Pulse2Vel(float pulse);//�����ٶ�ת��Ϊ��׼��λ�ٶ�
static float Vel2Pulse(float vel);//��׼��λ�ٶ�ת��Ϊ�����ٶ�
#endif


