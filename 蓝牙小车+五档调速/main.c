#include "reg52.h"

typedef unsigned char u8;
typedef unsigned int u16;

sbit lnL1=P2^0;	//l298n�źŹܽŶ���
sbit lnL2=P2^1;

sbit lnR1=P2^2;
sbit lnR2=P2^3;

sbit PWM = P2^4;  	  //��������ź���
sbit pwm = P2^5;

u16 ReceiveDate;

u8 count = 70, time=0;
  
void UsartInit()   //���ڳ�ʼ������
{
	SCON=0X50;		//����Ϊ������ʽ1
	TMOD=0X20|0X01;  	//���ü�����������ʽ2
	PCON=0X00;		//�����ʲ��ӱ�

	TH1=0XFD;		//��������ʼֵ���ã�ע�Ⲩ������9600��
	TL1=0XFD;

	ES=1;			//�򿪽����ж�
	EA=1;			//�����ж�
	TR1=1;			//�򿪼�����
}



  
void init0()  
{  
    //TMOD = 0x01;    // ��ʽһ  
    TH0 = 0xFC;     // ����ֵ ��ʱ1ms 
    TL0 = 0x18;  
    TR0 = 1;       // ������ʱ��0 
	//EA=1;   
    ET0 = 1;       // ��ʱ��0�����ж�  
}  
	

void left()
{ 
   lnL1=0;
   lnL2=1;

   lnR1=1;
   lnR2=0;

}

void right()
{ 
  	lnL1=1;
    lnL2=0;

    lnR1=0;
    lnR2=1;
}

void forward()
{ 
   lnL1=1;
   lnL2=0;

   lnR1=1;
   lnR2=0;
}

void back()
{ 
  	lnL1=0;
    lnL2=1;

    lnR1=0;
    lnR2=1;
}

void stop()
{ 
   lnL1=0;
   lnL2=0;

   lnR1=0;
   lnR2=0;
}


void func()
{
  switch (ReceiveDate)
   {
     case '2':forward();ReceiveDate=100;break;
     case '4':left();ReceiveDate=100;break;
     case '6':right();ReceiveDate=100;break;
     case '8':back();ReceiveDate=100;break;
     case '0':stop();ReceiveDate=100;break;

     case 'A':forward();ReceiveDate=100;break;
     case 'G':left();ReceiveDate=100;break;
     case 'C':right();ReceiveDate=100;break;
     case 'E':back();ReceiveDate=100;break;
     case 'Z':stop();ReceiveDate=100;break;

	 case '1': 
	    count = 20;  
		ReceiveDate=100;break;
	 case '3': 
	    count = 40; 
		ReceiveDate=100;break;
     case '5': 
	    count = 50;  
		ReceiveDate=100;break;
	 case '7': 
	    count = 80;  
		ReceiveDate=100;break;
	 case '9':
	    count = 100; 
		ReceiveDate=100;   break;
   }

}

void main()
{
    UsartInit();
	init0();
    while(1)
   {
     func();
   }

}


void Usart() interrupt 4
{ 
   if(RI==1)
   {
      ReceiveDate=SBUF;
      RI=0;
      SBUF=ReceiveDate;
      while(!TI);  
      TI = 0; 
   }
}

void T0_inter() interrupt 1  
{  
    
    TH0 = 0xFF;  
    TL0 = 0x47;  
    
    time ++ ;  
    if (time <= count)
	{
	   PWM = 1;
	   pwm = 1;
	}     
    if (time > count)   
	 {
	   PWM = 0;
	   pwm = 0;
	 }  

    if (time >= 100)   time = 0; 
      
}  

