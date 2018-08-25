#include "sys.h"
#include "usart.h"
#include "stdarg.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��Ȩ���У�����ؾ���
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
/**
�޸���ʷ��
20180429�� �����USART3�ĳ�ʼ�������Լ���Ӧ�Ľ����жϷ���       ���͵��ֽ�ʾ����   USART_SendData(USART3,"f");   //����"f"��asill��
20180430�� ��װ�û�������Ҫ�Ľ��շ��񣬵��յ�"1"ʱ��u8����colorofball=1."2"ʱcolorofball=2.  "3"ʱcolorofball=3.
				****ע�⣬���͵����ݱ�����"\n"��β���ҷ��͵��ַ����ﲻ�ܺ���"\" ****
**/ 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
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
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
 
void uart_init(u32 bound)
	{
    //GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

		//Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
		//USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);//ʹ��USART1ʱ��
	
	//USART3�˿�����
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  


   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	//USART1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 	
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOʱ��



//  /* Connect PXx to USARTx_Tx*/
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2);
//  /* Connect PXx to USARTx_Rx*/
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_USART2);

  /* Configure USART Tx as alternate function  */   //PA2
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//���츴�����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */   //PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  
	//////////   ����USART2�ж�       ///////////////
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;   //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	/* USARTx configured as follow:
	- BaudRate = 57600 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = BaudRate; //���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
  /* USART configuration */
  USART_Init(USART2, & USART_InitStructure);//��ʼ������
 
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�	 
 
	/* Enable USART */
	USART_Cmd(USART2, ENABLE); //ʹ�ܴ��� 
}

//void USART2_IRQHandler(void)  //�жϽ�������������
//{
//  if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
//  {   
//    USART_SendData(USART2, TxBuffer[TxCounter++]); 
//    USART_ClearITPendingBit(USART2, USART_IT_TXE);
//    if(TxCounter == count) USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
//  }
//	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//  {
//		CopeSerial2Data((unsigned char)USART2->DR);//��������
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOʱ��



//  /* Connect PXx to USARTx_Tx*/
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_USART2);
//  /* Connect PXx to USARTx_Rx*/
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_USART2);

  /* Configure USART Tx as alternate function  */   //PB10


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//���츴�����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */   //PB11        
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  
	//////////   ����USART3�ж�       ///////////////
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;   //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	/* USARTx configured as follow:
	- BaudRate = 57600 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = BaudRate; //���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
  /* USART configuration */
  USART_Init(USART3, & USART_InitStructure);//��ʼ������
 
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�	 
 
	/* Enable USART */
	USART_Cmd(USART3, ENABLE); //ʹ�ܴ��� 
}


char *itoa(int value, char *string, int radix);

 /****************************************************************************
* ��    �ƣ�void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* ��    �ܣ���ʽ�������������
* ��ڲ�����USARTx:  ָ������
			Data��   ��������
			...:     ��������
* ���ڲ�������
* ˵    ������ʽ�������������
        	"\r"	�س���	   USART_OUT(USART1, "abcdefg\r")   
			"\n"	���з�	   USART_OUT(USART1, "abcdefg\r\n")
			"%s"	�ַ���	   USART_OUT(USART1, "�ַ����ǣ�%s","abcdefg")
			"%d"	ʮ����	   USART_OUT(USART1, "a=%d",10)
* ���÷������� 
   	   	USART_OUT(USART2, (uint8_t *)"hello \r\n"); //�����ַ��� ʾ��
****************************************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...){ 
	const char *s = NULL;
    int d = 0;
    char buf[16] = {0};
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //�ж��Ƿ񵽴��ַ���������
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //���з�
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
				case 's':										  //�ַ���
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //ʮ����
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
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//			}
//			printf("\r\n\r\n");//���뻻��
//			USART_RX_STA=0;
//		}	
      /*�û��жϷ���*/

	//    USART_OUT(USART1,"usart1:%d \r\n",Res);

	} 
 
}
#endif
u8 USART3_RX_BUF[20];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	
u8 colorofball=0;
int inttemp=0;
void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;                                                /*�����ж�(���յ������ݱ�����0x5c 0x6e��β)  ��"\n"��ASII��   */
	u16 len;	
	u16 t;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
		
		if((USART3_RX_STA&0x8000)==0)//����δ���
		{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ//if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART3_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0a)USART3_RX_STA|=0x4000;//if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART_REC_LEN/20-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
		}
    /*�û�����*/
#if 0
			if(USART3_RX_STA&0x8000)
			{
			    len=USART3_RX_STA&0x3fff; //�õ��˴ν��յ������ݳ���	
				for(t=0;t<len;t++)			
				{
					if(USART3_RX_BUF[t]==0x31){colorofball=1;} //�յ� "1"
					if(USART3_RX_BUF[t]==0x32){colorofball=2;} //�յ� "2"
					if(USART3_RX_BUF[t]==0x33){colorofball=3;} //�յ� "2"
				}
				USART3_RX_STA=0;
			}
#endif
#if 0
		if(USART3_RX_STA&0x8000)
		{					   
			len=USART3_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		//	printf("\r\n�����͵���ϢΪ:\r\n\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART3_RX_BUF[t]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				USART3_RX_BUF[t]=0;
			}
			printf("\r\n\r\n");//���뻻��
			USART3_RX_STA=0;
		}
#endif
#if 0		//����	
			if(USART3_RX_STA&0x8000)
			{
				len=USART3_RX_STA&0x3fff;
				inttemp=str2int(USART3_RX_BUF);//asciiת��ֵ
				for(t=0;t<len;t++)  //��ս��ջ���
				{
					USART3_RX_BUF[t]=0;
				}	
				printf("\r\n inttemp:%d ",inttemp); //���뻻��
				USART3_RX_STA=0;
			}
#endif
#if 1			//��ʽ
			if(USART3_RX_STA&0x8000)
			{
				len=USART3_RX_STA&0x3fff;
				inttemp=str2int(USART3_RX_BUF);//asciiת��ֵ
				for(t=0;t<len;t++)  //��ս��ջ���
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
        const char *ptr = str;  //ptr����str�ַ�����ͷ

       if (*str == '-' || *str == '+')  //�����һ���ַ��������ţ�
       {                      //���Ƶ���һ���ַ�

           str++;

       }
       while(*str != 0)
       {
           if ((*str < '0') || (*str > '9'))  //�����ǰ�ַ���������
          {                       //���˳�ѭ��

               break;

          }
           temp = temp * 10 + (*str - '0'); //�����ǰ�ַ��������������ֵ

           str++;      //�Ƶ���һ���ַ�
       }   
       if (*ptr == '-')     //����ַ������ԡ�-����ͷ����ת�������෴��

     {

           temp = -temp;

       }

       return temp;
   }
