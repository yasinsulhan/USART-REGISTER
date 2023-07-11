/* Includes */
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void RCC_Config()
{
	RCC->CR |= 0x00010000;		// HEON
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;			// CSS Enable
	RCC->CR |= 0x01000000;			// PLL On
	RCC->PLLCFGR |= 0x00400000;		// PLL HSE Selected
	RCC->PLLCFGR |= 0x00000004;		// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;		// PLL N = 168
	RCC->PLLCFGR |= 0x00000000;		// PLL P = 2
	RCC->CFGR |= 0x00000000;		// AHB Prescaler 1
	RCC->CFGR |= 0x00080000;		// APB2 Prescaler 2
	RCC->CFGR |= 0x00001400;		// AHB1 Prescaler 4
	RCC->CIR |= 0x00080000;			// HSERDY Flag Clear
	RCC->CIR |= 0x00800000;			// CSS Flag Clear
}

void GPIO_Congig()
{
	RCC->AHB1ENR |= 0x00000002;					// Port B enable
	GPIOB->MODER |= (2 << 20) | (2 << 22);		//PB10 And PB11 AF
	GPIOB->AFR[1] |= (7 << 8) | (7 << 12);		//PB10 and PB11 AF7 (USART3)
}

void USART_Config()
{
	RCC->APB1ENR |= 1 << 18;

	USART3->BRR = 0x1112;		// BaudRate 9600
	USART3->CR1 |= 1 << 2;		// Rx enable
	USART3->CR1 |= 1 << 3;		// Tx enable
	USART3->CR1 |= 1 << 5;		// Rx interrupt enable
	USART3->CR1 |= 0 << 10;		// Parity control disable
	USART3->CR1 |= 0 << 12;		// Word length 8bit
	USART3->CR2 |= 0 << 12;		// Stop bit 1
	USART3->BRR = 0x1112;		// BaudRate 9600
	USART3->CR1 |= 1 << 13;		// Usart enable
}

void Send_Char(uint8_t data)
{
	//----- Steps for Sending Characters -----//

	/*
		1.Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
		for each data to be transmitted in case of single buffer.

		2.After writing the last data into the USART_DR register, ***wait until TC=1. This indicates***
		that the transmission of the last frame is complete. This is required for instance when
		the USART is disabled or enters the Halt mode to avoid corrupting the last
		transmission.
	*/

	USART3->DR = data;	// the data is written into Data Register. Also this clears TXE bit
	while(!(USART3->SR & (1 << 6)));	// Wait till TC is SET!. The data transmitter can be indicated.
}

void Send_Str(char *Str)
{
	while(*Str)
	{
		Send_Char(*Str);
		Str++;
	}
}

uint8_t Get_Char(void)
{

	//----- Steps for Receiving Chararacters -----//

		/*
			1.The RXNE bit is set. It ***indicates*** that the content of the shift register is transferred to the
			  RDR. In other words, data has been received and can be read.

			2.In single buffer mode, clearing the RXNE bit is performed by a software read to the
			  USART_DR register. The RXNE flag can also be cleared by writing a zero to it. The
			  RXNE bit must be cleared before the end of the reception of the next character to avoid
			  an overrun error.
		*/

	uint8_t temp;
	while(!(USART3->SR & (1 <<5)));		// wait until RXNE bit is set.
	temp = USART3->DR;			// read the data register. It also clears RXNE bit.
}


int main(void)
{

  RCC_Config();
  GPIO_Congig();
  USART_Config();
  while (1)
  {
	  //Sending Data
	  Send_Str("Hello World\n");
	  for(int a=0; a<1000000; a++);	// delay...

	  //Receiveing Data
	  uint8_t data = Get_Char();
	  Send_Char(data);
  }
}


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
