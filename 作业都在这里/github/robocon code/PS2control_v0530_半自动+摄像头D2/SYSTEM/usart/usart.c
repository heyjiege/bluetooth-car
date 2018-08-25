#include "sys.h"
#include "usart.h"
#include "stdarg.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//版权所有，盗版必究。
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
/**
修改历史：
20180429： 添加了USART3的初始化函数以及相应的接收中断服务       发送单字节示例：   USART_SendData(USART3,"f");   //发送"f"的asill码
20180430： 封装好机器人需要的接收服务，当收到"1"时，u8变量colorofball=1."2"时colorofball=2.  "3"时colorofball=3.
				****注意，发送的数据必须以"\n"结尾，且发送的字符串里不能含有"\" ****
**/ 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
 
void uart_init(u32 bound)
	{
    //GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

		//Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
		//USART 初始化设置

		USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
} 
#endif	
/**
  * @brief  Init USART1, used for ops9
  * @param  BaudRate Baudrate of usart 1
  * @retval void
  */
void USART1_Init(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);//使能USART1时钟
	
	//USART3端口配置
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  


   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	//USART1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
    USART_Cmd(USART1, ENABLE);  //使能串口1 	
}
/**
  * @brief  Init USART2, used for debug
  * @param  BaudRate Baudrate of usart 2 
  * @retval void
  */
void USART2_Init(u32 BaudRate)
{
	USART_InitTypeDef USART_InitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	

//------------------------------------------------------------
	 
  /* Enable GPIO clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIO时钟



//  /* Connect PXx to USARTx_Tx*/
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2);
//  /* Connect PXx to USARTx_Rx*/
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_USART2);

  /* Configure USART Tx as alternate function  */   //PA2
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//推挽复用输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */   //PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  
	//////////   设置USART2中断       ///////////////
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;   //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	/* USARTx configured as follow:
	- BaudRate = 57600 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = BaudRate; //设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
  /* USART configuration */
  USART_Init(USART2, & USART_InitStructure);//初始化串口
 
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断	 
 
	/* Enable USART */
	USART_Cmd(USART2, ENABLE); //使能串口 
}

//void USART2_IRQHandler(void)  //中断接收陀螺仪数据
//{
//  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
//  {   
//    USART_SendData(USART2, TxBuffer[TxCounter++]); 
//    USART_ClearITPendingBit(USART2, USART_IT_TXE);
//    if(TxCounter == count) USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
//  }
//	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//  {
//		CopeSerial2Data((unsigned char)USART2->DR);//处理数据
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//  }
//	
//	USART_ClearITPendingBit(USART2,USART_IT_ORE);
//}

/**
  * @brief  Init USART3, used for
  * @param  BaudRate Baudrate of usart 3 
  * @retval void
  */
void USART3_Init(u32 BaudRate)
{
	USART_InitTypeDef USART_InitStructure;  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	

//------------------------------------------------------------
	 
  /* Enable GPIO clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIO时钟



//  /* Connect PXx to USARTx_Tx*/
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2);
//  /* Connect PXx to USARTx_Rx*/
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_USART2);

  /* Configure USART Tx as alternate function  */   //PB10


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//推挽复用输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */   //PB11        
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  
	//////////   设置USART3中断       ///////////////
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;   //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);

	/* USARTx configured as follow:
	- BaudRate = 57600 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = BaudRate; //设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
  /* USART configuration */
  USART_Init(USART3, & USART_InitStructure);//初始化串口
 
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断	 
 
	/* Enable USART */
	USART_Cmd(USART3, ENABLE); //使能串口 
}


char *itoa(int value, char *string, int radix);

 /****************************************************************************
* 名    称：void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* 功    能：格式化串口输出函数
* 入口参数：USARTx:  指定串口
			Data：   发送数组
			...:     不定参数
* 出口参数：无
* 说    明：格式化串口输出函数
        	"\r"	回车符	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	换行符	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	字符串	   USART_OUT(USART1, "字符串是：%s","abcdefg")
			"%d"	十进制	   USART_OUT(USART1, "a=%d",10)
* 调用方法：无 
   	   	USART_OUT(USART2, (uint8_t *)"hello \r\n"); //发送字符串 示例
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 
	const char *s = NULL;
    int d = 0;
    char buf[16] = {0};
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //判断是否到达字符串结束符
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
			
			 
		}
		else if(*Data=='%'){									  //
			switch (*++Data){				
				case 's':										  //字符串
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //十进制
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}

/**
  * @brief  Transform int to assci, such as 11 to "11"
  * @param  value The integer to be transformed
  * @param  string The output of the transforming
  * @param  radix Defines how many decimal digital to be transformed
			such as value 1111, radix 100, the output will be 111 not 1111
  * @retval char * not used
  */
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} 

#if 0
void USART3_IRQHandler(void)     //
{
	u16 len;	
//	u16 times=0;
	u16 t;
    unsigned int Res;
    if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET) 
    { 
            USART_ReceiveData(USART3); 
            USART_ClearFlag(USART3, USART_FLAG_PE); 
    }
 
    if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET) 
    { 
            USART_ReceiveData(USART3); 
            USART_ClearFlag(USART3, USART_FLAG_ORE); 
    }
    if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET) 
    { 
            USART_ReceiveData(USART3); 
            USART_ClearFlag(USART3, USART_FLAG_FE); 
    }
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //
    {        
            USART_ClearFlag(USART3, USART_FLAG_RXNE);  
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);   //  
            Res =USART_ReceiveData(USART3); //
 //         g_Uart1_Timer=1;//
        if((USART_RX_STA&0x8000)==0)
          {       
                USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
                USART_RX_STA++;
                if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//
          }    
//		if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("\r\n您发送的消息为:\r\n\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//			}
//			printf("\r\n\r\n");//插入换行
//			USART_RX_STA=0;
//		}	
      /*用户中断服务*/

	//    USART_OUT(USART1,"usart1:%d \r\n",Res);

	} 
 
}
#endif
u8 USART3_RX_BUF[20];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	
u8 colorofball=0;
int inttemp=0;
void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;                                                /*接收中断(接收到的数据必须是0x5c 0x6e结尾)  即"\n"的ASII码   */
	u16 len;	
	u16 t;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART3);	//读取接收到的数据
		
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始//if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART3_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0a)USART3_RX_STA|=0x4000;//if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART_REC_LEN/20-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
		}
    /*用户内容*/
#if 0
			if(USART3_RX_STA&0x8000)
			{
			    len=USART3_RX_STA&0x3fff; //得到此次接收到的数据长度	
				for(t=0;t<len;t++)			
				{
					if(USART3_RX_BUF[t]==0x31){colorofball=1;} //收到 "1"
					if(USART3_RX_BUF[t]==0x32){colorofball=2;} //收到 "2"
					if(USART3_RX_BUF[t]==0x33){colorofball=3;} //收到 "2"
				}
				USART3_RX_STA=0;
			}
#endif
#if 0
		if(USART3_RX_STA&0x8000)
		{					   
			len=USART3_RX_STA&0x3fff;//得到此次接收到的数据长度
		//	printf("\r\n您发送的消息为:\r\n\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART3_RX_BUF[t]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				USART3_RX_BUF[t]=0;
			}
			printf("\r\n\r\n");//插入换行
			USART3_RX_STA=0;
		}
#endif
#if 0		//测试	
			if(USART3_RX_STA&0x8000)
			{
				len=USART3_RX_STA&0x3fff;
				inttemp=str2int(USART3_RX_BUF);//ascii转数值
				for(t=0;t<len;t++)  //清空接收缓存
				{
					USART3_RX_BUF[t]=0;
				}	
				printf("\r\n inttemp:%d ",inttemp); //插入换行
				USART3_RX_STA=0;
			}
#endif
#if 1			//正式
			if(USART3_RX_STA&0x8000)
			{
				len=USART3_RX_STA&0x3fff;
				inttemp=str2int(USART3_RX_BUF);//ascii转数值
				for(t=0;t<len;t++)  //清空接收缓存
				{
					USART3_RX_BUF[t]=0;
				}	
				USART3_RX_STA=0;
			}
#endif	
			
     } 
} 

int str2int(const char *str)
{

        int temp = 0;
        const char *ptr = str;  //ptr保存str字符串开头

       if (*str == '-' || *str == '+')  //如果第一个字符是正负号，
       {                      //则移到下一个字符

           str++;

       }
       while(*str != 0)
       {
           if ((*str < '0') || (*str > '9'))  //如果当前字符不是数字
          {                       //则退出循环

               break;

          }
           temp = temp * 10 + (*str - '0'); //如果当前字符是数字则计算数值

           str++;      //移到下一个字符
       }   
       if (*ptr == '-')     //如果字符串是以“-”开头，则转换成其相反数

     {

           temp = -temp;

       }

       return temp;
   }
