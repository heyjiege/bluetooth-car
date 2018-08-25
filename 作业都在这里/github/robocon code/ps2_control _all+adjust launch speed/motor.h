
typedef unsigned char u8;

typedef unsigned int u16;

//��������
sbit IN11 = P1^4;	//����ź���
sbit IN12 = P2^0;
sbit IN21 = P2^2;
sbit IN22 = P2^3;
sbit IN31 = P2^4;
sbit IN32 = P2^5;
sbit IN41 = P1^6;
sbit IN42 = P1^7;	  //����ź���


sbit OP = P3^5;	       //�̵��������̷��䣩
//sbit CL = P3^6;	   //  û��
//sbit IN52 = P3^6;	   //  û��

sbit PWM = P2^7;	 //�������PWM
sbit DIR = P2^6;	 //�������������
      
sbit PWM2 = P2^1;  	  //Ħ���ֵ���ֱ������ź���

 
  
void delay(int n_ms)               
{
 int j,k;
 for(j=0;j<n_ms;j++)
 for(k=0;k<1200;k++);
}

void stright()

{
IN11=IN21=1;
IN31=IN41=1;
IN12=IN22=0;
IN32=IN42=0; 
delay(2); }


void back()
{
IN12=IN22=1;
IN32=IN42=1;
IN11=IN21=0;
IN31=IN41=0;
delay(2);}

void left()
{
IN11=IN31=0;
IN12=IN32=1;
IN22=IN42=0;
IN21=IN41=1;
delay(2);}

void right()
{
IN11=IN31=1;
IN12=IN32=0;
IN22=IN42=1;
IN21=IN41=0;
delay(2);}

void stop()
{
IN11=IN31=0;
IN12=IN32=0;
IN22=IN42=0;
IN21=IN41=0;
delay(2);}

void stright1()
{
IN11=IN21=0;
IN12=IN42=0;
IN32=IN22=0;
IN31=IN41=1;
delay(2);	}

void back1()
{
IN11=IN21=0;
IN12=IN22=1;
IN32=IN42=0;
IN31=IN41=0;
delay(2);			}



// ����̨װ��


void Launchera()
{
	 delay(2);
	 OP=0;
	 delay(40);
	 OP=1;
				}

void Launcherb()						 																																																																																																																																																																																																																																																																						
{
	 delay(2);
	 OP=0;	  }
	
void Launcherc()

{
	delay(2);
	OP=1;	  }

 void Timer1_Init()         
{
	TMOD =TMOD|0x11;   //��ʱ��1������ʽ1   ��  ��ʱ��0������ʽ0      
	TH1  = 0xFF;		  					  
	TL1  = 0x47;    
	TR1=1; 			 
	EA=1;			
//	ET1=1;		   
}
void Timer1_Int() interrupt 3 
{
	TH1  = 0xFF;
	TL1  = 0x47;
	PWM = ~PWM;
}

//����̨���1

u16 count = 65, time;  
  
void init0()  
{  
    //TMOD = 0x01;    // ��ʽһ  
    TH0 = 0xFC;     // ����ֵ ��ʱ1ms 
    TL0 = 0x18;  
    TR0 = 1;       // ������ʱ��0 
//	EA=1;   
    ET0 = 1;       // ��ʱ��0�����ж�  
}  


void T0_inter() interrupt 1  
{  
    
    TH0 = 0xFF;  
    TL0 = 0x47;  
    
     
    time ++ ;  
    if (time < count)//  
    {  
        PWM2 = 1;  
    }  
    else  
    {  
        PWM2 = 0;  
    }  
    if (time >= 100)  
    {  
        time = 0;  
    }  
}  
				 