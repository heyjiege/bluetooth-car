#ifndef _motorfour_H_
#define _motorfour_H_

#include "sys.h"
#include "can.h"
#include "delay.h"


void Speed_Moto_Control(float vx,float vy,float vz);   ////����vx��ʾx������ٶȣ�vy��ʾy������ٶȣ�vz��ʾ�Ե������Ϊ���ĵĽ��ٶȣ�����
void car_4lun_Init(u8 mode,u8 t);
#endif
