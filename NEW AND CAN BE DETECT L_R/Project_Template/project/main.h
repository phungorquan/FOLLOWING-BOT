#include "stm32f10x.h"
#include <stdio.h>



USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;


void delay(int itime){	   
	
	int icompTime = 0;

   for(icompTime=itime*1000;icompTime!=0;icompTime--);
}

void NVIC_Configuration1()
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


void NVIC_Configuration2()
{
	
	
	
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void _send(uint8_t data)
{
 USART_SendData(USART1, data);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void _SendString(char str[])
{
   int i =0;
  
  while (str[i] != 0x00)
  {
    _send(str[i]);
    i++;
  }
}

void EN_UART2()
{
	NVIC_Configuration2();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* PA10 = floating input */ //Enable RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
	// CONFIG USART
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);   
}

void EN_UART1()
{
	

	NVIC_Configuration1();
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 ,ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* PA10 = floating input */ //Enable RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
	// CONFIG USART
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);  
  
}
	
void Phaitien(void)
{
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

void Phailui(void)
{
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void Phaidung(void)
{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

void Traitien(void)
{
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void Trailui(void)
{
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
}

void Traidung(void)
{
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void GPIOB_Configuration(void){
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 
 /* Configure PC13 in output pushpull mode */
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_1|GPIO_Pin_0;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);                        
}



