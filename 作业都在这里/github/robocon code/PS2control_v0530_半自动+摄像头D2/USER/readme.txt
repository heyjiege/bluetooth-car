

angle = (float)stcAngle.Angle[2]/32768*180; //角度赋值

void PS2control()
{
     u16 key;
//	u8 i,t;
//	key=PS2_keyScan();
//	for(i=16;i>0;i--)
//	{
//		if((key&0x8000)==0)t=0;
//		else t=1;
//	  printf("%d",t);
//		key=key<<1;
//	} printf("\r\n\r\n");	
	
		key=PS2_keyScan();
       if(key==0xffff)//无按键按下时清除相关变量及标志
	   {
		     motor[0]=0,motor[1]=0;  //motor[2]=0;
		     Fan_flag=0,O_flag=0,SETECL_flag=0,START_flag=0,R3_flag=0,L3_flag=0,R2_flag=0,L2_flag=0;
	   }
       else
      {		  
		if((key&0x8000)==0){motor[0]=-tempwm*2/3;motor[2]=0;}//LEFT
		if((key&0x4000)==0){motor[1]=tempwm;motor[2]=0;}//DOWN
		if((key&0x2000)==0){motor[0]=tempwm*2/3;motor[2]=0;}//RIGHT
		if((key&0x1000)==0){motor[1]=-tempwm;motor[2]=0;}//UP
		if((key&0x0800)==0)START_task();//START
		  else START_flag=0;
//		if((key&0x0400)==0);//R3
		  	
		if((key&0x0200)==0)L3_task();//L3
		  else L3_flag=0;	
		if((key&0x0100)==0)SETECL_task();//SELECT
		  else SETECL_flag=0;
		if((key&0x0080)==0)Fang_task();//fang 
		 else Fan_flag=0;
		if((key&0x0040)==0){tempwm=tempwm-10;}//cha
		if((key&0x0020)==0)O_task();//yuan
		 else O_flag=0;
		if((key&0x0010)==0){tempwm=tempwm+10;}//san jiao
		if((key&0x0008)==0){motor[2]=-300*3;if(motor[1]!=0)motor[2]=motor[2]*3;}//R1

		if((key&0x0004)==0){motor[2]=300*3;if(motor[1]!=0)motor[2]=motor[2]*3;}//L1
	
		if((key&0x0002)==0)R2_task();//R2
		 else R2_flag=0;
		if((key&0x0001)==0)L2_task();//L2
		 else L2_flag=0;
	  }
	  
	  if(Anglecontrol)//选择反向
	  {
	    motor[0]=(-motor[0]); motor[1]=(-motor[1]);  
#ifdef zuoyouxian
			  if(Data[1]==0x73)// ID   0x41(65)绿灯模式(灯灭)；0x73(115)红灯模式。
			  {
				  
				 if(Data[8]<124||Data[8]>132||Data[7]<124||Data[7]>132)
				  {
					  if(Data[8]<124){motor[1]=(128-Data[8])*(tempwm/200);}
					  else if(Data[8]>132){motor[1]=-(Data[8]-128)*(tempwm/200);}
					 // else motor[1]=0;
					  if(Data[7]<124){motor[0]=(128-Data[7])*(tempwm/300);}
					  else if(Data[7]>132){motor[0]=-(Data[7]-128)*(tempwm/300);}
					 // else motor[0]=0;
				  }	
				 else if((Data[6]<124||Data[6]>132||Data[5]<124||Data[5]>132)&&(key==0xffff))
				{
					   
					  if(Data[6]<124){motor[1]=(128-Data[6])*3+50;}
					  else if(Data[6]>132){motor[1]=-(Data[6]-128)*3+50;}
					//  else motor[1]=0;
					  if(Data[5]<124){motor[0]=((128-Data[5])*2);}
					  else if(Data[5]>132){motor[0]=-(Data[5]-128)*2;}	
				 }
			 }
#else
			  if(Data[1]==0x73)// ID   0x41(65)绿灯模式(灯灭)；0x73(115)红灯模式。
			  {
				 if(Data[6]<124||Data[6]>132||Data[5]<124||Data[5]>132)
				  {
					  if(Data[6]<124){motor[1]=(128-Data[6])*(tempwm/200);}
					  else if(Data[6]>132){motor[1]=-(Data[6]-128)*(tempwm/200);}
					//  else motor[1]=0;
					  if(Data[5]<124){motor[0]=(128-Data[5])*(tempwm/300);}
					  else if(Data[5]>132){motor[0]=-(Data[5]-128)*(tempwm/300);}
					//  else motor[0]=0;
				  }	
				 else if((Data[8]<124||Data[8]>132||Data[7]<124||Data[7]>132)&&(key==0xffff))
				{
					   
					  if(Data[8]<124){motor[1]=(128-Data[8])*3+50;}
					  else if(Data[8]>132){motor[1]=-(Data[8]-128)*3+50;}
					//  else motor[1]=0;
					  if(Data[7]<124){motor[0]=((128-Data[7])*2);}
					  else if(Data[7]>132){motor[0]=-(Data[7]-128)*2;}	
				 }
			
				//  if(((key&0x1000)==0)||((key&0x4000)==0))motor[0]=0;	
				  
			  }	
#endif				 
	  }
	  else
	  {
#ifdef zuoyouxian
			  if(Data[1]==0x73)// ID   0x41(65)绿灯模式(灯灭)；0x73(115)红灯模式。
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
			  if(Data[1]==0x73)// ID   0x41(65)绿灯模式(灯灭)；0x73(115)红灯模式。
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
		 }
	  
	  		if(tempwm>4000)tempwm=4000;  //速度限幅
			 if(tempwm<500)tempwm=500;

	  if(PS2_flag==1)
	  {
	   
		 static float Bias,PwmX,Last_YBias,Integral_Ybias;	
         static float Position_KP=4, Position_KI=0, Position_KD=5;		  
		  	 Bias=0-inttemp;                                  //计算Y轴偏差
			 Integral_Ybias+=Bias;	                                 //求出偏差的积分
			 PwmX=Position_KP*Bias+Position_KI*Integral_Ybias+Position_KD*(Bias-Last_YBias);       //位置式PID控制器
			 Last_YBias=Bias; 
         if(PwmX>700)PwmX=700;
		 if(PwmX<(-700))PwmX=(-700);		  
	      motor[0]=PwmX;
		  
		  printf(" PWMx:%d  \r\n",motor[0]);
	  }
	  if(Slow_Y)  //限制旋转后的Y轴速度
	  {
	     if(motor[1]>400)motor[1]=500;

	  }
	  Speed_Moto_Control(motor[0],motor[1],motor[2]); //发送电机控制指令
}