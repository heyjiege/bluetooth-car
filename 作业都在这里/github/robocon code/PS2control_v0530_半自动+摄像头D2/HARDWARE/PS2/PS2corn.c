/*******************************************
˵����PS2�ֱ���������
�޸���ʷ
20180401��
20180430������˰� R3 ���ÿ��Ʒ�����ת180�ȵ��趨
20180503��(1)�����Ż�
          (2)����˰�L2R2ʹ�л�λ��ģʽ����̬��ת90�ȵ�����
20180521:��״̬���ķ�ʽ��������Ĳ������ϵ������������ԭ�����ĸ�������Ϊ2��������
********************************************/
#include "stm32f10x.h"
#include "sys.h" 
#include "PS2corn.h"

//#include "exti.h"
#include "Shoudong_move.h"
static u16 tempwm=500;
u8 PS2_flag=0;
u8 Quqiu_flag=3;
_Bool Slow_Y=0;
_Bool Anglecontrol=0,L2_EA=1,R2_EA=1;                          
_Bool FastBack=0;
_Bool Fan_flag=0,O_flag=0,SETECL_flag=0,START_flag=0,R3_flag=0,L3_flag=0,R2_flag=0,L2_flag=0;//������־  δ����0������1
_Bool SentFlag=0,SentFlag1=1;
_Bool Dont2Er=1,DiveBall_EA=1;
u8 GetBallflag=1;
int motor[3]={0,0,0};
u8 wheretogo=0;
//#define Position_KP 3
//#define Position_KI 0
//#define Position_KD 10

u16 PS2_keyScan()   //16λ ��Ӧ16������
{
//	u8 keytemp;
	u8 l;
	u16 h;
//   keytemp =PS2_DataKey();
	PS2_ClearData();
	PS2_ReadData();
	h=Data[3]<<8;
    l=Data[4];
   return h+l;
}
/*
u16 key;
	key=PS2_keyScan(); 
       if(key==0xffff)motor[0]=0,motor[1]=0,motor[2]=0,Fan_flag=0,O_flag=0;
       else
      {
		if((key&0x8000)==0);//LEFT
		if((key&0x4000)==0);//DOWN
		if((key&0x2000)==0);//RIGHT
		if((key&0x1000)==0);//UP
		if((key&0x0800)==0);//START
		if((key&0x0400)==0);//R3
		if((key&0x0200)==0);//L3
		if((key&0x0100)==0);//SELECT
		if((key&0x0080)==0);//fang 
		if((key&0x0040)==0);//cha
		if((key&0x0020)==0);//yuan
		if((key&0x0010)==0);//san jiao
		if((key&0x0008)==0);//R1
		if((key&0x0004)==0);//L1
		if((key&0x0002)==0);//R2
		if((key&0x0001)==0);//L2
	 }
*/
void Fang_task()
{
   
   if(!Fan_flag)  //ȷ����һ�� ����ִֻ��1��
   {  
	  if(GetnowY<7100) 
	  {
		   switch(Quqiu_flag )   
		   {                                             
			   case 3 :IOA7=!IOA7;IOA6=!IOA6;Quqiu_flag--;//�պ�����
						wheretogo=2;PD_EA=1;PD_EA2=1;MoveState=0;dposX=0;dposY=0;Dont2Er=0; //ǰ��1��
						break;
			   case 2 :IOA5=1;Quqiu_flag--;wheretogo=7;PD_EA2=1;MoveState=0;break;   		//̧������
			   case 1 : IOA5=1;IOA7=!IOA7;Quqiu_flag--;wheretogo=7;break;   	 	//�뿪���
			   default :IOA5=1;IOA7=!IOA7;IOA6=!IOA6;wheretogo=7; break;    	    //����
		   }
	  }
	   Fan_flag=1;
   }
}
void O_task()
{
   if(!O_flag) //ȷ����һ�� ����ִֻ��1��
   {  
	   switch(Quqiu_flag)   
	   {
		   case 3 :IOA7=!IOA7;IOA6=!IOA6;Quqiu_flag--;//�պ�����
		            //if(Dont2Er){wheretogo=2;Dont2Er=0;}//ִֻ�е�һ��
					//else{wheretogo=4;}
		            wheretogo=4;
					PD_EA=1;PD_EA2=1;MoveState=0;dposX=0;dposY=0;  //ǰ��2��
		           break;
		   case 2 :IOA5=1;Quqiu_flag--;wheretogo=8;PD_EA2=1;MoveState=0;break;   		//̧������
		   case 1 : IOA5=1;IOA7=!IOA7;Quqiu_flag--;wheretogo=8;break;   	 	//�뿪���
		   default :IOA5=1;IOA7=!IOA7;IOA6=!IOA6;wheretogo=8;//����
		           // IOA4=!IOA4; IOA1=!IOA1;IOA0=!IOA0; IOB0=!IOB0; 
		            break;    	    
	   }	   
	 O_flag=1;
   }
}
void START_task()  //���临λ����ȡ��״̬
{
   if(!START_flag) //ȷ����һ�� ����ִֻ��1��
   { 

     IOA7=0;
	 IOA6=1;
	 IOA5=0;
    Quqiu_flag=3;
     switch(whereI)	   
	 {
	   case 0 :wheretogo=1;break;  //��������
	   case 11 :wheretogo=3;break; //��1������
	   case 22 :wheretogo=6;break; //��2������
	   case 33 :wheretogo=5;break; //������λ�ó���
	   default : break;
	 }
      dposX=0;dposY=0;
	  PD_EA=1;PD_EA2=1;
	 MoveState=0;
	 PS2_flag=0;
	 START_flag=1;
   }
}
void SETECL_task()
{
   if(!SETECL_flag) //ȷ����һ�� ����ִֻ��1��
   {
		//   USART_OUT(USART3,"f");  //��ʼ������ݮ������
		  printf("PID");
		   PS2_flag=!PS2_flag;   //X������ͷPIDʹ��

	      // PD_EA=!PD_EA; 
	 SETECL_flag=1;
   }
}
void L3_task()
{
   if(!L3_flag)  //ȷ����һ�� ����ִֻ��1��
   {
//		  USART_OUT(USART3,"f");  //��ʼ������ݮ������
//		  printf("PID");
//		   PS2_flag=!PS2_flag;   //X������ͷPIDʹ��
//		   D4=!D4; 
	   dposY=dposY-200;
	   L3_flag=1;
   }
}
u16 timecount;
void R2_task()
{
   if(!R2_flag)  //ȷ����һ�� ����ִֻ��1��
   {
//	   angleZ=0;
//	   Slow_Y=0;
      PD_EA2=0;wheretogo=0;
	   PwmX=0;
	   PwmY=0;
	   whereI=33;
	   R2_flag=1;
   }
}
void L2_task()
{
   if(!L2_flag)  //ȷ����һ�� ����ִֻ��1��
   {
//	   angleZ=90;
//	   Slow_Y=1;   
     IOA7=0;
	 IOA6=1;
	 IOA5=0;
    Quqiu_flag=3;
     switch(whereI)	   
	 {
	   case 0 :wheretogo=1;break;  //��������
	   case 11 :wheretogo=3;break; //��1������
	   case 22 :wheretogo=6;break; //��2������
	   case 33 :wheretogo=5;break; //������λ�ó���
	   default : break;
	 }
      dposX=0;dposY=0;
	  PD_EA=1;PD_EA2=1;
	 MoveState=0;
	 PS2_flag=0;
	   L2_flag=1;
   }
}
static u8 timedec=30;
/*
void PS2control()
{
     u16 key;
	key=PS2_keyScan();
       if(key==0xffff)//�ް�������ʱ�����ر���
	   {
		     motor[0]=0,motor[1]=0,motor[2]=0;
		     Fan_flag=0,O_flag=0,SETECL_flag=0,START_flag=0;
	   }
       else
      {		  
		if((key&0x8000)==0)motor[0]=-tempwm;//LEFT
		if((key&0x4000)==0)motor[1]=tempwm;//DOWN
		if((key&0x2000)==0)motor[0]=tempwm;//RIGHT
		if((key&0x1000)==0)motor[1]=-tempwm;//UP
		if((key&0x0800)==0)START_task();//START
		  else START_flag=0;
//		if((key&0x0400)==0);//L3
//		if((key&0x0200)==0);//R3
		if((key&0x0100)==0)SETECL_task();//SELECT
		  else SETECL_flag=0;
		if((key&0x0080)==0)Fang_task();//fang 
		 else Fan_flag=0;
		if((key&0x0040)==0){tempwm=tempwm-100;}//cha
		if((key&0x0020)==0)O_task();//yuan
		 else O_flag=0;
		if((key&0x0010)==0){tempwm=tempwm+100;}//san jiao
		if((key&0x0008)==0)motor[2]=-tempwm*3;//R1
		if((key&0x0004)==0)motor[2]=tempwm*3;//L1
//		if((key&0x0002)==0);//L2
//		if((key&0x0001)==0);//
	  }

      if(Data[1]==0x73)// ID   0x41(65)�̵�ģʽ(����)��0x73(115)���ģʽ��
	  {
          if(Data[8]<124){motor[1]=-(128-Data[8])*(tempwm/200);}
		  else if(Data[8]>132){motor[1]=(Data[8]-128)*(tempwm/200);}
		//  else motor[1]=0;
		  if(Data[7]<124){motor[0]=-(128-Data[7])*(tempwm/200);}
		  else if(Data[7]>132){motor[0]=(Data[7]-128)*(tempwm/200);}
         // else motor[0]=0;			  
	  }
	  		if(tempwm>2000)tempwm=2000;  //�ٶ��޷�
			 if(tempwm<200)tempwm=200;
	  Speed_Moto_Control(motor[0],motor[1],motor[2]); //���͵������ָ��
}
*/
		 static float Bias1,Last_1_Bias,Integral_1_bias;	
         static float Position1_KP=1, Position1_KI=0, Position1_KD=1;	
void PS2control()
{
     u16 key;
/*	���ڵ��԰���
	u8 i,t;
	key=PS2_keyScan();
	for(i=16;i>0;i--)
	{
		if((key&0x8000)==0)t=0;
		else t=1;
	  printf("%d",t);
		key=key<<1;
	} printf("\r\n\r\n");	
	*/

	//AreaSpeed();
	
      key=PS2_keyScan();
	if(PD_EA2)  //
	{
		   if(key==0xffff)//�ް�������ʱ�����ر�������־
		   {
				 if(!PD_EA){PwmX=0,PwmY=0;PD_EA=1;X_PD_EA=1;Y_PD_EA=1;}
				 if(!PD_angle_EA){PwmZ=0;PD_angle_EA=1;}
				 Fan_flag=0,O_flag=0,SETECL_flag=0,START_flag=0,R3_flag=0,L3_flag=0,R2_flag=0,L2_flag=0;
				
		   }
		   else
		  {		  
			if((key&0x8000)==0){ timedec--;
								 if(timedec==0){timedec=1;dposY=dposY-4;}  //����
								else dposY=dposY-2;
							      PS2_flag=0;
				                // wheretogo=0;
				                 whereI=33;//��֪λʧЧ				
								}else timedec=40;//LEFT
			if((key&0x4000)==0){
									dposX=dposX+2;
				                   DiveBall_EA=1;
								//	wheretogo=0;
									PS2_flag=0;
									whereI=33;
								}//DOWN
			if((key&0x2000)==0){ dposY=dposY+2;
									//wheretogo=0;
									PS2_flag=0;
									whereI=33;}//RIGHT  
			if((key&0x1000)==0){ dposX=dposX-2;
				                     DiveBall_EA=1;
									//wheretogo=0;
									whereI=33;
				                  PS2_flag=0;
								}//UP
			
			if((key&0x0800)==0)START_task();//START
			  else START_flag=0;
			
			if((key&0x0400)==0)PD_EA2=!PD_EA2;//R3
			if((key&0x0200)==0)L3_task();
			  else L3_flag=0;	
			if((key&0x0100)==0)SETECL_task();//SELECT
			  else SETECL_flag=0;
			if((key&0x0080)==0){
									delay_ms(5);
				                  key=PS2_keyScan();
									if((key&0x0080)==0)Fang_task();
								}//fang 
			 else Fan_flag=0;
			if((key&0x0040)==0){
									dposX=dposX+12;
									PS2_flag=0;
				                    dposY=0;
				                    posY=GetnowY;
									//wheretogo=0;
							   }//cha
			if((key&0x0020)==0){
									delay_ms(5);
				                  key=PS2_keyScan();
				                  if((key&0x0020)==0)O_task();//yuan
			                    }//yuan
			 else O_flag=0;
			if((key&0x0010)==0){
								 if(IN_E2==0&&IN_E3==0)dposX=dposX-10;//0529,16:48 ��������δ���赲ʱ����ǰ��
									PS2_flag=1;
								}//san jiao
			if((key&0x0008)==0){angleZ--;}//R1
			if((key&0x0004)==0){angleZ++;}//L1
			if((key&0x0002)==0)R2_task();//R2
			 else R2_flag=0;
			if((key&0x0001)==0)L2_task();//L2
			 else L2_flag=0;
		  }

	  }
	else   //���ֶ�
	{
		   if(key==0xffff)//�ް�������ʱ�����ر�������־
		   {
				
				 if(!PD_angle_EA){PD_angle_EA=1;}//ȷ���Ƕ��ɶ�λ������
				 Fan_flag=0,O_flag=0,SETECL_flag=0,START_flag=0,R3_flag=0,L3_flag=0,R2_flag=0,L2_flag=0;
				dposX=0;dposY=0;
				 timedec=30;
		   }
		   else
		  {		  
			if((key&0x8000)==0){ 
								 timedec--;
								 if(timedec==0){timedec=1;PwmY=-870;}
								 else PwmY=-400;
								wheretogo=0;whereI=33;//��֪λʧЧ	
                                 PS2_flag=0;				
								}//else timedec=30;//LEFT
			if((key&0x4000)==0){ timedec--;
								 if(timedec==0){timedec=1; PwmX=900;}
								 else PwmX=500;
								 wheretogo=0;whereI=33;DiveBall_EA=1;}//else timedec=30;//DOWN
			if((key&0x2000)==0){  timedec--;
								 if(timedec==0){timedec=1;PwmY=870;}
								 else PwmY=400;
									wheretogo=0;whereI=33;PS2_flag=0;}//else timedec=30;//RIGHT  
			if((key&0x1000)==0){ timedec--;
								 if(timedec==0){timedec=1; PwmX=-900;}
								 else PwmX=-500;
									wheretogo=0;whereI=33;DiveBall_EA=1;}//else timedec=30;//UP
			
			if((key&0x0800)==0)START_task();//START
			  else START_flag=0;
			
			if((key&0x0400)==0)PD_EA2=!PD_EA2;//R3
			if((key&0x0200)==0)L3_task();
			  else L3_flag=0;	
			if((key&0x0100)==0)SETECL_task();//SELECT
			  else SETECL_flag=0;
			if((key&0x0080)==0){
									delay_ms(5);
				                  key=PS2_keyScan();
									if((key&0x0080)==0)Fang_task();
								}//fang 
			 else Fan_flag=0;
			if((key&0x0040)==0){
								 PwmX=900;wheretogo=0;PS2_flag=0;
							   }//cha
			if((key&0x0020)==0){
									delay_ms(5);
				                  key=PS2_keyScan();
				                  if((key&0x0020)==0)O_task();//yuan
			                    }
			 else O_flag=0;
			if((key&0x0010)==0){
								 PwmX=-800;wheretogo=0;PS2_flag=1;
							//	PS2_flag=1;if(SentFlag){USART_OUT(USART3,"f");SentFlag=0;}
								}//san jiao
			if((key&0x0008)==0){angleZ--;}//R1
			if((key&0x0004)==0){angleZ++;}//L1
			if((key&0x0002)==0)R2_task();//R2
			 else R2_flag=0;
			if((key&0x0001)==0)L2_task();//L2
			 else L2_flag=0;
		}
		 posX=GetnowX;posY=GetnowY;
		
		if(Data[1]==0x73)// ID   0x41(65)�̵�ģʽ(����)��0x73(115)���ģʽ��
		{
				  if(Data[8]<124||Data[8]>132||Data[7]<124||Data[7]>132)
				  {
					  if(Data[8]<124){PwmX=-(128-Data[8])*3;}
					  else if(Data[8]>132){PwmX=(Data[8]-128)*3;}
					//  else motor[1]=0;
					  if(Data[7]<124){PwmY=-(128-Data[7])*3;}
					  else if(Data[7]>132){PwmY=(Data[7]-128)*3;}
					//  else motor[0]=0;
				  }
		}		
		
	}
/*
#ifdef zuoyouxian
			  if(Data[1]==0x73)// ID   0x41(65)�̵�ģʽ(����)��0x73(115)���ģʽ��
			  {
				  if(Data[8]<124||Data[8]>132||Data[7]<124||Data[7]>132)
				  {
					  if(Data[8]<124){motor[1]=-(128-Data[8])*(tempwm/200);}
					  else if(Data[8]>132){motor[1]=(Data[8]-128)*(tempwm/200);}
					//  else motor[1]=0;
					  if(Data[7]<124){motor[0]=-(128-Data[7])*(tempwm/300);}
					  else if(Data[7]>132){motor[0]=(Data[7]-128)*(tempwm/300);}
					//  else motor[0]=0;
				  }
				 else if((Data[6]<124||Data[6]>132||Data[5]<124||Data[5]>132)&&(key==0xffff))
				   {
					   
					  if(Data[6]<124){motor[1]=-(128-Data[6])*3+50;}
					  else if(Data[6]>132){motor[1]=(Data[6]-128)*3+50;}
					//  else motor[1]=0;
					  if(Data[5]<124){motor[0]=-((128-Data[5])*2);}
					  else if(Data[5]>132){motor[0]=(Data[5]-128)*2;}	
				   }
			   }
#else
			  if(Data[1]==0x73)// ID   0x41(65)�̵�ģʽ(����)��0x73(115)���ģʽ��
			  {
				 if(Data[6]<124||Data[6]>132||Data[5]<124||Data[5]>132)
				  {
					  if(Data[6]<124){motor[1]=-(128-Data[6])*(tempwm/200);}
					  else if(Data[6]>132){motor[1]=(Data[6]-128)*(tempwm/200);}
					//  else motor[1]=0;
					  if(Data[5]<124){motor[0]=-(128-Data[5])*(tempwm/300);}
					  else if(Data[5]>132){motor[0]=(Data[5]-128)*(tempwm/300);}
					//  else motor[0]=0;
				  }	
				 else if((Data[8]<124||Data[8]>132||Data[7]<124||Data[7]>132)&&(key==0xffff))
				{
					   
					  if(Data[8]<124){motor[1]=-(128-Data[8])*3+50;}
					  else if(Data[8]>132){motor[1]=(Data[8]-128)*3+50;}
					//  else motor[1]=0;
					  if(Data[7]<124){motor[0]=-((128-Data[7])*2);}
					  else if(Data[7]>132){motor[0]=(Data[7]-128)*2;}	
				 }
			   
				 //  if(((key&0x1000)==0)||((key&0x4000)==0))motor[0]=0;		  
			  }
#endif				  
*/



	  if(PS2_flag==1)
	  {
//		  if(inttemp==0)
//		  {
			  USART_OUT(USART3,"f");  //���Խ�����ݮ������
		      delay_ms(2);//�����㹻ʱ��������
//		  }
		  	 Bias1=(0-inttemp);                                  //����Y��ƫ��
			 Integral_1_bias+=Bias1;	                                 //���ƫ��Ļ���
			 PwmY=Position1_KP*Bias1+Position1_KI*Integral_1_bias+Position1_KD*(Bias1-Last_1_Bias);       //λ��ʽPID������
			 Last_1_Bias=Bias1; 
         if(PwmY>900)PwmY=900;
		 if(PwmY<(-900))PwmY=(-900);		  

		 printf("|$| inttemp:%d",inttemp);
	  }
}

void AreaSpeed()
{
	// X���ٶ�     nowX      <-2000|-2000~-100|-100<
	  //            Vx           500|  1500    | 300
   if(GetnowX<-2000)tempwm=500;
	else if(GetnowX>-2000&&GetnowX<(-100))tempwm=1500; 
	else tempwm=300; 
	
	// Y���ٶ�     nowY      <-3000|-3000~6400|6400<
	  //            Vy           1000|  1500    | 1000
   if(GetnowX<-2000)tempwm=500;
	else if(GetnowX>-2000&&GetnowX<(-100))tempwm=1500; 
	else tempwm=300; 
	
	if(GetnowY<(-2700))tempwm=800;
	else if(GetnowY>(-2700)&&GetnowY<6400)tempwm=1500;
	else tempwm=800;
}
