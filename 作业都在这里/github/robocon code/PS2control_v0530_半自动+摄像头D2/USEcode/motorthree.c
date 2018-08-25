/********************************************************
�޸���ʷ
20180401��
20180503��ʹģʽ�л���Speed_Moto_Control��Ȼ����
****************************************************/
#include "motorthree.h"
#include "math.h"
#define VX_VALUE           (0.5f)
#define VY_VALUE           (sqrt(3)/2.f)      
#define L_value            (20*0.01f)  
//#define RADIUS_value       (1.0/12.5*0.01f)

static u8 ModeofNow;
void Speed_Moto_Control(float vx,float vy,float vz)   ////����vx��ʾx������ٶȣ�vy��ʾy������ٶȣ�vz��ʾ�Ե������Ϊ���ĵĽ��ٶȣ�����
{
	    short motor_one,motor_two,motor_three;
	    vy=(-vy);
        motor_one   = (-VX_VALUE*vx + VY_VALUE*vy + L_value*vz);
        motor_two   = (-VX_VALUE*vx - VY_VALUE*vy + L_value*vz);
        motor_three = (vx + L_value*vz);
	
		switch(ModeofNow)
		{
			case 0x03 :
					//�ٶ�ģʽ
				CAN_RoboModule_DRV_Velocity_Mode(0,1,5000,motor_one);
				CAN_RoboModule_DRV_Velocity_Mode(0,2,5000,motor_two);
				CAN_RoboModule_DRV_Velocity_Mode(0,3,5000,motor_three);
				break;
			case 0x04 :
				   //λ��
				CAN_RoboModule_DRV_Position_Mode(0,1,2000,motor_one);	
				CAN_RoboModule_DRV_Position_Mode(0,2,2000,motor_two);
				CAN_RoboModule_DRV_Position_Mode(0,3,2000,motor_three);
				break;
			case 0x06 :
				   //�����ٶ�
				CAN_RoboModule_DRV_Current_Velocity_Mode(0,1,3500,(short)motor_one);	
				CAN_RoboModule_DRV_Current_Velocity_Mode(0,2,3500,(short)motor_two);	
				CAN_RoboModule_DRV_Current_Velocity_Mode(0,3,3500,(short)motor_three);	
				break;
			default : break;
		}
	//printf("x: %d y: %d  z:%d \t\n",motor_one,motor_two,motor_three);
}
void car_3lun_Init(u8 mode,u8 t)
{
	ModeofNow=mode;
    CAN1_Configuration();        //CAN1��ʼ��	
    delay_ms(300);                                      //�տ�ʼҪ���㹻����ʱ��ȷ���������Ѿ���ʼ����� 
    CAN_RoboModule_DRV_Reset(0,0);                      //��0���������������и�λ 
    
    delay_ms(400);                                      //���͸�λָ������ʱ����Ҫ�У��ȴ��������ٴγ�ʼ�����
    
    CAN_RoboModule_DRV_Config(0,1,t,0);               //1������������Ϊtms����һ������
    delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��
    CAN_RoboModule_DRV_Config(0,2,t,0);               //2������������Ϊtms����һ������
    delay_us(200);                                      //�˴���ʱΪ�˲��ô�������ʱ��4����һ��
    CAN_RoboModule_DRV_Config(0,3,t,0);               //3������������Ϊtms����һ������

    CAN_RoboModule_DRV_Mode_Choice(0,0,mode);  //0������������� ������modeģʽ
    delay_ms(500);          	//����ģʽѡ��ָ���Ҫ�ȴ�����������ģʽ������������ʱҲ������ȥ����

}

u8 turn90(int k)
{
	int tempa=k*61700;
		car_3lun_Init(Position_Mode,100);

		CAN_RoboModule_DRV_Position_Mode(0,1,2000,tempa);	
		CAN_RoboModule_DRV_Position_Mode(0,2,2000,tempa);
		CAN_RoboModule_DRV_Position_Mode(0,3,2000,tempa);	
	while(!(Real_Position_Value[1]<(tempa+10)&&Real_Position_Value[1]>(tempa-10)));
	    car_3lun_Init(Velocity_Mode,0);
	return 0;
}


///**
//  * @brief  ��׼��λ�ٶ�ת��Ϊ�����ٶ�
//  * @param  vel:�ٶ� mm/s
//  * @retval velpulse:�ٶ� ����/s
//  */
//static float Vel2Pulse(float vel)
//{
//	float pulse = 0.0f;
//	pulse = vel / (2.0f * PI * WHEEL_RADIUS) * PULSE_PER_CIRCLE * REDUCTION;
//	return pulse;
//}
///**
//  * @brief  �����ٶ�ת��Ϊ��׼��λ�ٶ�
//  * @param  pulse:�ٶ� ����/s
//  * @retval vel:�ٶ� mm/s
//  */
//static float Pulse2Vel(float pulse)
//{
//	float vel = 0.0f;
//	vel = pulse * (2.0f * PI * WHEEL_RADIUS) / REDUCTION / PULSE_PER_CIRCLE;
//	return vel;
//}
