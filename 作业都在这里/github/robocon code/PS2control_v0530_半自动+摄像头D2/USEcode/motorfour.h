#ifndef _motorfour_H_
#define _motorfour_H_

#include "sys.h"
#include "can.h"
#include "delay.h"


void Speed_Moto_Control(float vx,float vy,float vz);   ////其中vx表示x方向的速度，vy表示y方向的速度，vz表示以电机轴线为中心的角速度！！！
void car_4lun_Init(u8 mode,u8 t);
#endif
