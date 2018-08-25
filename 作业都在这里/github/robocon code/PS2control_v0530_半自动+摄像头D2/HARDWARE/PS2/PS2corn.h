/**************************
修改历史
20180503：
**********************/
#ifndef _PS2corn_H_
#define _PS2corn_H_
#include "sys.h"

extern int motor[3];
extern u8 PS2_flag;
extern _Bool DiveBall_EA;
#define NEW_PS2 //使用新的按键处理函数   全键无冲
//#define zuoyouxian    //使左边摇杆有更高优先级，并将可变速摇杆设置在左摇杆
#define DoubleBox    //使用双球箱

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

