#ifndef _shoudong_move_H_
#define _shoudong_move_H_

#include "sys.h"
extern sanVTypeDef O2A,A2Yi,A2Er;
extern u8 MoveState;
extern int LastDidian[3];
extern u8 whereI;
u8 Shou_Move_0_1(void);//前往取球区 取红球
u8 Shou_Move_1_11(void);   //从取球区 前往 1区
u8 Shou_Move_11_1(void);   //从1区 前往取球区 
u8 Shou_Move_1_22(void);   //从取球区 前往 2区
u8 Shou_Move_22_1(void);
u8 Shou_Move_any_1(void); //直接前往取球区
u8 Shou_Move_any_11(void);//直接前往1区
u8 Shou_Move_any_22(void);//直接前往2区
#endif

