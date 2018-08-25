/***********************************
�ļ���motorfour.c   motorfour.h
˵���� ����ȫ���ֵ��̵���ٶȼ���

�汾���޸�����  v201804018
*************************************/

#include "motorfour.h"
#include "math.h"
#define VX_VALUE           (0.5f)
#define VY_VALUE           (sqrt(3)/2.f)      
#define L_value            (20*0.01f)  
#define RADIUS_value       (1.0/12.5*0.01f)

//#define j1 (5*3.1415/6f)
//#define j2 (5*3.1415/4f)
//#define j3 (7*3.1415/4f)
//#define j4 (3.1415/6f)
#define cosj1 (0.9989562f)
#define cosj2 (0.9976521f)
#define cosj3 (0.9953998f)
#define cosj4 (0.9999582f)
#define sinj1 (0.0456767f)
#define sinj2 (0.0684485f)
#define sinj3 (0.9580731f)
#define sinj4 (0.0091383f)
//cos(pi/4)=0.99990605f
//sin(pi/4)=0.01370735f
void Speed_Moto_Control(float vx,float vy,float vz)   ////����vx��ʾx������ٶȣ�vy��ʾy������ٶȣ�vz��ʾ�Ե������Ϊ���ĵĽ��ٶȣ�����
{
	    int motor_one,motor_two,motor_three,motor_four;
	
        motor_one   = (-cosj1*vx + sinj1*vy + L_value*vz);
        motor_two   = (-cosj2*vx + sinj2*vy + L_value*vz);
        motor_three = (-cosj3*vx + sinj4*vy + L_value*vz);
        motor_four  = (-cosj4*vx + sinj5*vy + L_value*vz);
//		CAN_RoboModule_DRV_OpenLoop_Mode(0,1,motor_one);
//		CAN_RoboModule_DRV_OpenLoop_Mode(0,2,motor_two);
//		CAN_RoboModule_DRV_OpenLoop_Mode(0,3,motor_three);
CAN_RoboModule_DRV_Velocity_Mode(0,1,3000,motor_one);
CAN_RoboModule_DRV_Velocity_Mode(0,2,3000,motor_two);
CAN_RoboModule_DRV_Velocity_Mode(0,3,3000,motor_three);	
CAN_RoboModule_DRV_Velocity_Mode(0,4,3000,motor_four);	
//printf("x: %d y: %d  z:%d \t\n",motor_one,motor_two,motor_three);
}
void car_4lun_Init(u8 mode,u8 t)
{
    CAN1_Configuration();        //CAN1��ʼ��	
    delay_ms(500);                                      //�տ�ʼҪ���㹻����ʱ��ȷ���������Ѿ���ʼ����� 
    CAN_RoboModule_DRV_Reset(0,0);                      //��0���������������и�λ 
    
    delay_ms(500);                                      //���͸�λָ������ʱ����Ҫ�У��ȴ��������ٴγ�ʼ�����
    
    CAN_RoboModule_DRV_Config(0,1,t,0);               //1������������Ϊtms����һ������
    delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��
    CAN_RoboModule_DRV_Config(0,2,t,0);               //2������������Ϊtms����һ������
    delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��
    CAN_RoboModule_DRV_Config(0,3,t,0);               //3������������Ϊtms����һ������
    delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��
    CAN_RoboModule_DRV_Config(0,4,t,0);               //4������������Ϊtms����һ������
	
    CAN_RoboModule_DRV_Mode_Choice(0,0,mode);  //0������������� �����뿪��ģʽ
    delay_ms(500);          	//����ģʽѡ��ָ���Ҫ�ȴ�����������ģʽ������������ʱҲ������ȥ����
}