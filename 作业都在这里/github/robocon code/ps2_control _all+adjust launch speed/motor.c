
unsigned char HighRH = 0; //高电平重载值的高字节
unsigned char HighRL = 0; //高电平重载值的低字节
unsigned char LowRH = 0; //低电平重载值的高字节
unsigned char LowRL = 0; //低电平重载值的低字节
char a1=0;

/*延时*/
void delay(int n_ms)               
{
 int j,k;
 for(j=0;j<n_ms;j++)
 for(k=0;k<1200;k++);
}


/* 配置并启动 PWM， dc-占空比 （取值-90至90）*/
void Servo_PWM(char dc)
{
	unsigned int high, low;
	unsigned long tmp;
	
	tmp = (11059200/12) / 50; //计算一个周期所需的计数值
	high = (tmp*(15+dc*0.1) )/ 200; //计算高电平所需的计数值
	low = tmp - high; //计算低电平所需的计数值
	high = 65536 - high + 12; //计算高电平的重载值并补偿中断延时
	low = 65536 - low + 12; //计算低电平的重载值并补偿中断延时
	HighRH = (unsigned char)(high>>8); //高电平重载值拆分为高低字节
	HighRL = (unsigned char)high;
	LowRH = (unsigned char)(low>>8); //低电平重载值拆分为高低字节
	LowRL = (unsigned char)low;
	TMOD &= 0xF0; //清零 T0 的控制位
	TMOD |= 0x01; //配置 T0 为模式 1
	TH0 = HighRH; //加载 T0 重载值
	TL0 = HighRL;
	ET0 = 1; //使能 T0 中断
	TR0 = 1; //启动 T0
	PWMOUT = 1; //输出高电平
}
///* 关闭 PWM */
//void ClosePWM()
//{
//	TR0 = 0; //停止定时器
//	ET0 = 0; //禁止中断
//	PWMOUT = 1; //输出高电平
//}
/* T0 中断服务函数，产生 PWM 输出 */
void InterruptTimer0() interrupt 1
{
	if (PWMOUT == 1) //当前输出为高电平时，装载低电平值并输出低电平
	{
	TH0 = LowRH;
	TL0 = LowRL;
	PWMOUT = 0;
	}
	else //当前输出为低电平时，装载高电平值并输出高电平
	{
	TH0 = HighRH;
	TL0 = HighRL;
	PWMOUT = 1;
	}
}
/* 舵机速度控制， b-速度值（0至100，值越大速度越快）， pwm（取值-90至90）*/
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

