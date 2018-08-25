/**

修改历史：

**/
#include "Shoudong_move.h"

u8 MoveState=0;
int LastDidian[3];
u8 whereI=0;
u8 Shou_Move_0_1()  //直接前往
{
  u8 tempr=0;
	switch(MoveState)
	{
		case 0 : PD_EA=1;X_PD_EA=1;
		         toXY(0,0,0);
		        if(CherkZitai()==1)
				{
					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					//0,0,0
					MoveState++;
				}
				 break;
		case 1 : toXY(O2A.x,O2A.y,O2A.a);
		        if(CherkZitai()==1)
				{
					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					MoveState++;
				}
				  //0,-2740,0
		         break;
		default : break;
	}
    if(MoveState==2){tempr=1;whereI=1;PD_EA2=0;}
	else tempr=0;
	return tempr;
}

u8 Shou_Move_1_11()  //先直走出，再向外侧运动，再旋转
{
  u8 tempr=0;
	switch(MoveState)
	{
		case 0 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
//		         toXY(LastDidian[0],LastDidian[1],LastDidian[2]);
//		        if(CherkZitai()==1)
//				{
//					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					MoveState++;
//				}
				 break;
		case 1 : PD_EA=1;X_PD_EA=0;Y_PD_EA=0;
		         toXY(LastDidian[0],LastDidian[1]+1000,LastDidian[2]);
		         if(CherkZuobiao(200)==1)MoveState++;
				 break;
		case 2 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
			     toXY(A2Yi.x,A2Yi.y,A2Yi.a);
		        if(CherkZitai()==1)
				{
					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					MoveState++;
				}//-557,5500,90
		default : break;
	}
    if(MoveState==3){tempr=1;whereI=11;PD_EA2=0;}//可以考虑在这里开启摄像头
	else tempr=0;
	return tempr;
}

u8 Shou_Move_11_1()  //交接区1  去取球
{
  u8 tempr=0;
	switch(MoveState)  //先向右平移2米且旋转至0度，再向
	{
		case 0 : PD_EA=1;X_PD_EA=0;Y_PD_EA=0;
//		         toXY(-50,5500,90);
//		        if(CherkZuobiao(200)==1)
//				{
					MoveState++;
//				}
				 break;
		case 1 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
		         toXY(-500,4000,0);
		        if(CherkZuobiao(300)==1)
				{
					MoveState++;
				}
				break;
		case 2 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
		         toXY(O2A.x,O2A.y,O2A.a);
		        if(CherkZitai()==1)
				{
					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					MoveState++;
				}break;
		default : break;
	}
    if(MoveState==3){tempr=1;whereI=1;PD_EA2=0;}
	else tempr=0;
	return tempr;
}

u8 Shou_Move_1_22()
{
  u8 tempr=0;
	switch(MoveState)
	{
		case 0 :  PD_EA=1;X_PD_EA=0;Y_PD_EA=0;
		         toXY(LastDidian[0],LastDidian[1]+1000,LastDidian[2]);
		         if(CherkZuobiao(200)==1)MoveState++;

				 break;
//		case 1 : PD_EA=1;X_PD_EA=0;Y_PD_EA=0;
//		         toXY(LastDidian[0]-180,LastDidian[1]+700,LastDidian[2]);
//		         if(CherkZuobiao(200)==1)MoveState++;
//				 break;
		case 1 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
				 toXY(A2Er.x,A2Er.y,A2Er.a);
		        if(CherkZitai()==1)
				{
					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					MoveState++;
				}
		         break;
		default : break;
	}
    if(MoveState==2){tempr=1;whereI=22;PD_EA2=0;}
	else tempr=0;
	return tempr;
}
u8 Shou_Move_22_1()  //交接区2  去取球
{
  u8 tempr=0;
	switch(MoveState)  //先向右平移2米且旋转至0度，再向
	{
		case 0 : if(GetnowY<5800){MoveState=2;break;}//不在2区
				 PD_EA=1;X_PD_EA=0;Y_PD_EA=0;
		         toXY(LastDidian[0]+100,LastDidian[1]-500,LastDidian[2]);
		        if(CherkZuobiao(200)==1)
				{
					MoveState++;
				}
				 break;
		case 1 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
		         toXY(-500,LastDidian[1]-1000,LastDidian[2]-90);
		        if(CherkZuobiao(200)==1)
				{
					MoveState++;
				}
				break;
		case 2 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
		         toXY(O2A.x,O2A.y-50,O2A.a);
		        if(CherkZitai()==1)
				{
					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					MoveState++;
				}break;
		default : break;
	}
    if(MoveState==3){tempr=1;whereI=1;PD_EA2=0;}
	else tempr=0;
	return tempr;
}
u8 Shou_Move_any_1()
{
  u8 tempr=0;
	switch(MoveState)
	{
		case 0 : PD_EA=1;X_PD_EA=0;Y_PD_EA=0;
//		        toXY(O2A.x,O2A.y+800,O2A.a);
//		        if(CherkZuobiao(200)==1)
//				{
					MoveState++;
//    			}
				break;    //原先忘记加了，现在补上，检查一下效果
		case 1 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
		         toXY(O2A.x,O2A.y,O2A.a);
		        if(CherkZitai()==1)
				{
					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					MoveState++;
				}
		         break;
		default : break;
	}
    if(MoveState==2){tempr=1;whereI=1;PD_EA2=0;}
	else tempr=0;
	return tempr;
}
u8 Shou_Move_any_11()
{
  u8 tempr=0;
	switch(MoveState)
	{
		case 0 : PD_EA=1;X_PD_EA=0;Y_PD_EA=0;
//		        toXY(A2Yi.x,A2Yi.y-800,A2Yi.a);
//		        if(CherkZuobiao(200)==1)
//				{
					MoveState++;
//				}
				break;
		case 1 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
		         toXY(A2Yi.x,A2Yi.y,A2Yi.a);
		        if(CherkZitai()==1)
				{
					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					MoveState++;
				}
		         break;
		default : break;
	}
    if(MoveState==2){tempr=1;whereI=11;PD_EA2=0;PS2_flag=1;} //可以考虑在这里开启摄像头
	else tempr=0;
	return tempr;
}
u8 Shou_Move_any_22()
{
  u8 tempr=0;
	switch(MoveState)
	{
		case 0 : PD_EA=1;X_PD_EA=0;Y_PD_EA=0;
//		        toXY(A2Er.x,A2Er.y-800,A2Er.a);
//		        if(CherkZuobiao(200)==1)
//				{
					MoveState++;
//				}
				 break;
		case 1 : PD_EA=1;X_PD_EA=1;Y_PD_EA=1;
		         toXY(A2Er.x,A2Er.y,A2Er.a);
		        if(CherkZitai()==1)
				{
					LastDidian[0]=posX;LastDidian[1]=posY;LastDidian[2]=angleZ;
					MoveState++;
				}
		         break;
		default : break;
	}
    if(MoveState==2){tempr=1;whereI=22;PD_EA2=0;PS2_flag=1;} //可以考虑在这里开启摄像头
	else tempr=0;
	return tempr;
}
