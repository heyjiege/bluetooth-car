#ifndef _shoudong_move_H_
#define _shoudong_move_H_

#include "sys.h"
extern sanVTypeDef O2A,A2Yi,A2Er;
extern u8 MoveState;
extern int LastDidian[3];
extern u8 whereI;
u8 Shou_Move_0_1(void);//ǰ��ȡ���� ȡ����
u8 Shou_Move_1_11(void);   //��ȡ���� ǰ�� 1��
u8 Shou_Move_11_1(void);   //��1�� ǰ��ȡ���� 
u8 Shou_Move_1_22(void);   //��ȡ���� ǰ�� 2��
u8 Shou_Move_22_1(void);
u8 Shou_Move_any_1(void); //ֱ��ǰ��ȡ����
u8 Shou_Move_any_11(void);//ֱ��ǰ��1��
u8 Shou_Move_any_22(void);//ֱ��ǰ��2��
#endif

