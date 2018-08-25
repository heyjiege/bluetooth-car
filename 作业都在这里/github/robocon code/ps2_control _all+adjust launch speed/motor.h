
typedef unsigned char u8;

typedef unsigned int u16;

//底盘驱动
sbit IN11 = P1^4;	//电机信号线
sbit IN12 = P2^0;
sbit IN21 = P2^2;
sbit IN22 = P2^3;
sbit IN31 = P2^4;
sbit IN32 = P2^5;
sbit IN41 = P1^6;
sbit IN42 = P1^7;	  //电机信号线


sbit OP = P3^5;	       //继电器（飞盘发射）
//sbit CL = P3^6;	   //  没用
//sbit IN52 = P3^6;	   //  没用

sbit PWM = P2^7;	 //步进电机PWM
sbit DIR = P2^6;	 //步进电机方向线
      
sbit PWM2 = P2^1;  	  //摩擦轮电机手柄调速信号线

 
  
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



// 发射台装置


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
	TMOD =TMOD|0x11;   //定时器1工作方式1   加  定时器0工作方式0      
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

//发射台电机1

u16 count = 65, time;  
  
void init0()  
{  
    //TMOD = 0x01;    // 方式一  
    TH0 = 0xFC;     // 赋初值 定时1ms 
    TL0 = 0x18;  
    TR0 = 1;       // 开启定时器0 
//	EA=1;   
    ET0 = 1;       // 定时器0允许中断  
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
				 