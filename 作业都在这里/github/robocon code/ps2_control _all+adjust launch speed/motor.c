
unsigned char HighRH = 0; //�ߵ�ƽ����ֵ�ĸ��ֽ�
unsigned char HighRL = 0; //�ߵ�ƽ����ֵ�ĵ��ֽ�
unsigned char LowRH = 0; //�͵�ƽ����ֵ�ĸ��ֽ�
unsigned char LowRL = 0; //�͵�ƽ����ֵ�ĵ��ֽ�
char a1=0;

/*��ʱ*/
void delay(int n_ms)               
{
 int j,k;
 for(j=0;j<n_ms;j++)
 for(k=0;k<1200;k++);
}


/* ���ò����� PWM�� dc-ռ�ձ� ��ȡֵ-90��90��*/
void Servo_PWM(char dc)
{
	unsigned int high, low;
	unsigned long tmp;
	
	tmp = (11059200/12) / 50; //����һ����������ļ���ֵ
	high = (tmp*(15+dc*0.1) )/ 200; //����ߵ�ƽ����ļ���ֵ
	low = tmp - high; //����͵�ƽ����ļ���ֵ
	high = 65536 - high + 12; //����ߵ�ƽ������ֵ�������ж���ʱ
	low = 65536 - low + 12; //����͵�ƽ������ֵ�������ж���ʱ
	HighRH = (unsigned char)(high>>8); //�ߵ�ƽ����ֵ���Ϊ�ߵ��ֽ�
	HighRL = (unsigned char)high;
	LowRH = (unsigned char)(low>>8); //�͵�ƽ����ֵ���Ϊ�ߵ��ֽ�
	LowRL = (unsigned char)low;
	TMOD &= 0xF0; //���� T0 �Ŀ���λ
	TMOD |= 0x01; //���� T0 Ϊģʽ 1
	TH0 = HighRH; //���� T0 ����ֵ
	TL0 = HighRL;
	ET0 = 1; //ʹ�� T0 �ж�
	TR0 = 1; //���� T0
	PWMOUT = 1; //����ߵ�ƽ
}
///* �ر� PWM */
//void ClosePWM()
//{
//	TR0 = 0; //ֹͣ��ʱ��
//	ET0 = 0; //��ֹ�ж�
//	PWMOUT = 1; //����ߵ�ƽ
//}
/* T0 �жϷ����������� PWM ��� */
void InterruptTimer0() interrupt 1
{
	if (PWMOUT == 1) //��ǰ���Ϊ�ߵ�ƽʱ��װ�ص͵�ƽֵ������͵�ƽ
	{
	TH0 = LowRH;
	TL0 = LowRL;
	PWMOUT = 0;
	}
	else //��ǰ���Ϊ�͵�ƽʱ��װ�ظߵ�ƽֵ������ߵ�ƽ
	{
	TH0 = HighRH;
	TL0 = HighRL;
	PWMOUT = 1;
	}
}
/* ����ٶȿ��ƣ� b-�ٶ�ֵ��0��100��ֵԽ���ٶ�Խ�죩�� pwm��ȡֵ-90��90��*/
void Speed_servo(char b,char pwm)
{
	b = 101 - b;	
	while(1)
	{
		delay(b);				
		if(a1>pwm)
		{
			a1--;
			Servo_PWM(a1);			
			if(a1 == pwm)
			{
				a1=pwm;
				break;
			}
		}
		else if(a1<pwm)
		{
			a1++;
			Servo_PWM(a1);			 
			if(a1 == pwm)
			{
				a1=pwm;
				break;
			}
		}
		else
		{
			a1=pwm;
			Servo_PWM(a1);
			break;
		}
	}
}

