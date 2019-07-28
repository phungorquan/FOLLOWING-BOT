#include "main.h"

//volatile int _readdata1 = 0 ; // Xin

//volatile int _readdata2 = 0; // Cui

volatile unsigned int flag3 = 0;
volatile unsigned int flag2 = 0;

volatile uint16_t _readdata3 = 0x0000;
volatile uint16_t tmp_readdata3 = 0x0000;
volatile uint16_t _readdata2 = 0x0000;
volatile uint16_t tmp_readdata2 = 0x0000;

unsigned int a3 = 0;
unsigned int a2 = 0;

unsigned int count3 = 0;	
unsigned int count2 = 0;	
int main(void)
{
 
	
	GPIOA_Configuration();
	GPIOB_Configuration();
	GPIOC_Configuration();
				
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 1)
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 1);
		
				EN_UART3();
				EN_UART2();
				delay(3000);
				GPIOC->BSRR = GPIO_Pin_13;
				delay(3000);
				GPIOC->BRR = GPIO_Pin_13;
				delay(3000);
				GPIOC->BSRR = GPIO_Pin_13;
				delay(3000);
				GPIOC->BRR = GPIO_Pin_13;
				delay(3000);
    while (1)
    {
			
			
		
		
			//_send(USART3, 'Q');
			//	delay(50000);
			//_send(USART2, 'X');
			
			//	Phaitien();		  
			//	delay(50000);
			//	Phaidung();
			//	Traitien();
			//	delay(50000);
			//	Traidung();
		
			//delay(100);
			

		
		if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == 1)//BOARD CUI A10,A9 RX,TX
			{
				while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == 1)
			{
					_readdata3 = USART_ReceiveData(USART3);
				

					if(flag3 < 1)
				{
				 tmp_readdata3 = (_readdata3 << 8);
					_send(USART3, _readdata3);
					flag3++;
				}
						
				else if(flag3 > 0)
				{
					tmp_readdata3 |= _readdata3;
					a3 += (int)tmp_readdata3;
					count3++;
					if(count3 == 5)
					{
						a3 = a3/5;
						_send(USART3, a3);
						count3 = 0;
						
					}
					
					flag3 = 0;
					break;
				}
			
			}
		}
			
if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == 1) // BOARD XIN A3,A2 RX,TX
			{
				while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == 1)
			{
					_readdata2 = USART_ReceiveData(USART2);
				
					if(flag2 < 1)
				{
				 tmp_readdata2 = (_readdata2 << 8);
					_send(USART2, _readdata2);
					flag2++;
				}
						
					else if(flag2 > 0)
				{
					tmp_readdata2 |= _readdata2;
					a2 += (int)tmp_readdata2;
					count2++;
					if(count2 == 5)
					{
						a2 = a2/5;
						_send(USART2, a2);
						count2 = 0;
						
					}
					
					flag2 = 0;
					break;
				}
			
			}
		}
			
		
		
	//	if(a2 > 0x50 && a3 > 0x50)
		//	{
//				if(abs(_readdata1 - _readdata2) < 3)
//					{
//						Phaitien();
//						Traitien();
//					}
//					else 
						//{
							if(a2 < a3)
							{
									Phaitien();
									Traidung();
							}
							else if(a2 > a3)
							{
									Traitien();
									Phaidung();
							}
						//}
		//	}
		//	else 
		//		{
		//			Traidung();
		//			Phaidung();
		//		}
			
			 
				a3 = 0;
				a2 = 0;
		
		
		

		
		}

}

//void USART1_IRQHandler(void) 
//{
//		
//			//unsigned int plus = 40;
//			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//enter interrupt when STM32 receice data.
//      {
//         USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//					//_readdata1 = USART_ReceiveData(USART1);
//			
//				if(flag1 < 1)
//				{
//					_readdata1 = USART_ReceiveData(USART1);
//				 _readdata1 = (_readdata1 << 8);
//					flag1++;
//				}
//						
//				else if(flag1 > 0)
//				{
//					_readdata1 |= USART_ReceiveData(USART1);
//					//USART_SendData(USART1, _readdata1);
//						_send(USART1, _readdata1);
//					flag1 = 0;
//				}
//      }
//			

//}

//void USART2_IRQHandler(void) 
//{
//	
//			if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//enter interrupt when STM32 receice data.
//      {
//         USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//						//_readdata2 = USART_ReceiveData(USART2);
//			
//				if(flag2 < 1)
//				{
//					_readdata2 = USART_ReceiveData(USART2);
//				 _readdata2 = (_readdata2 << 8);
//					flag2++;
//				}
//						
//				else if(flag2 > 0)
//				{
//					_readdata2 |= USART_ReceiveData(USART2);
//					//USART_SendData(USART2, _readdata2);
//						_send(USART2, _readdata2);
//					flag2 = 0;
//				}
//			}
//}