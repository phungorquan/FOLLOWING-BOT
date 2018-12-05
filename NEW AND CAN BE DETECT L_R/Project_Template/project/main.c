#include "main.h"

unsigned int check = 0;

//volatile int _readdata1 = 0 ; // Xin

//volatile int _readdata2 = 0; // Cui

volatile int flag1 = 0;
volatile int flag2 = 0;

volatile uint16_t _readdata1 = 0x00;
volatile uint16_t _readdata2 = 0x00;
int main(void)
{
  EN_UART1();
  EN_UART2();
	GPIOB_Configuration();
    while (1)
    {
//			 Phailui();
//		   delay(50000);
//			 Phaidung();
//			 Trailui();
//		   delay(50000);
//			 Traidung();
//		
			//delay(100);
			if(_readdata2 < _readdata1)
			{
				Phaitien();
				Traidung();
			}
			else if(_readdata2 > _readdata1)
			{
				Traitien();
				Phaidung();
			}
			 
		}
}

void USART1_IRQHandler(void) //BOARD XIN A10,A9
{
		
			//unsigned int plus = 40;
			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//enter interrupt when STM32 receice data.
      {
         USART_ClearITPendingBit(USART1, USART_IT_RXNE);
				//	_readdata1 = USART_ReceiveData(USART1);
				//USART_SendData(USART1, _readdata1);
				if(flag1 < 1)
				{
					_readdata1 = USART_ReceiveData(USART1);
				 _readdata1 |= (_readdata1 << 8);
					flag1++;
				}
						
				else if(flag1 > 0)
				{
					_readdata1 |= USART_ReceiveData(USART1);
					USART_SendData(USART1, _readdata1);
					flag1 = 0;
				}
      }
			

}

void USART2_IRQHandler(void) // BOARD CUI A3,A2
{
	
			if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//enter interrupt when STM32 receice data.
      {
         USART_ClearITPendingBit(USART2, USART_IT_RXNE);
				if(flag2 < 1)
				{
					_readdata2 = USART_ReceiveData(USART2);
				 _readdata2 |= (_readdata2 << 8);
					flag2++;
				}
						
				else if(flag2 > 0)
				{
					_readdata2 |= USART_ReceiveData(USART2);
					USART_SendData(USART2, _readdata2);
					flag2 = 0;
				}
			}
}
