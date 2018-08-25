#ifndef _motorthree_H_
#define _motorthree_H_

#include "sys.h"
extern u8 ModeofNow;  //±£´æµ±Ç°Çı¶¯Æ÷ËùÉèÖÃµÄÄ£Ê½
void Speed_Moto_Control(float vx,float vy,float vz);   ////ÆäÖĞvx±íÊ¾x·½ÏòµÄËÙ¶È£¬vy±íÊ¾y·½ÏòµÄËÙ¶È£¬vz±íÊ¾ÒÔµç»úÖáÏßÎªÖĞĞÄµÄ½ÇËÙ¶È£¡
void car_3lun_Init(u8 mode,u8 t);
//void hengyiVP(int x);

u8 turn90(int k);

/**/
//½Ç¶ÈÖÆ×ª»»Îª»¡¶ÈÖÆÏµÊı
#define CHANGE_TO_RADIAN    (0.017453f)   
//»¡¶ÈÖÆ×ª»»Îª½Ç¶ÈÖÆÏµÊı
#define CHANGE_TO_ANGLE     (57.2958f)				
//Ô²ÖÜÂÊ
#define PI                  (3.1415926f)

#define NULL 0

//¼õËÙ±È
#define REDUCTION (1.0f)
//³µÂÖ°ë¾¶ µ¥Î»:mm
#define WHEEL_RADIUS (50.0f)
//Ã¿È¦Âö³åÊı°
#define PULSE_PER_CIRCLE (4096.0f)

static float Pulse2Vel(float pulse);//Âö³åËÙ¶È×ª»¯Îª±ê×¼µ¥Î»ËÙ¶È
static float Vel2Pulse(float vel);//±ê×¼µ¥Î»ËÙ¶È×ª»¯ÎªÂö³åËÙ¶È
#endif


