#include "stm32f4xx.h"
#include "sys.h"


static GPIO_InitTypeDef 		GPIO_InitStructure;
static NVIC_InitTypeDef 		NVIC_InitStructure;
static USART_InitTypeDef 		USART_InitStructure;

void delay_us(uint32_t n)
{
	SysTick->CTRL = 0; 			// Disable SysTick,???????
	SysTick->LOAD = (168*n)-1; // ?????(168*n)-1 ~ 0
	SysTick->VAL  = 0; 		// Clear current value as well as count flag
	SysTick->CTRL = 5; 		// Enable SysTick timer with processor clock
	while ((SysTick->CTRL & 0x10000)==0);// Wait until count flag is set
	SysTick->CTRL = 0; 		// Disable SysTick	
}

void delay_ms(uint32_t n)
{
	while(n--)
	{
		SysTick->CTRL = 0; 				// Disable SysTick,???????
		SysTick->LOAD = (168000)-1; 	// ?????(168000)-1 ~ 0
		SysTick->VAL  = 0; 		// Clear current value as well as count flag
		SysTick->CTRL = 5; 		// Enable SysTick timer with processor clock
		while ((SysTick->CTRL & 0x10000)==0);// Wait until count flag is set
	}
	
	SysTick->CTRL = 0; 		// Disable SysTick	

}

void usart1_init(uint32_t baud)
{
	
	//??PA????	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	

	//????1????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//??PA9?PA10???????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;		//?9 10???
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;	//?????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//????,?????????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//?????????

	GPIO_Init(GPIOA,&GPIO_InitStructure);


	//?PA9?PA10???????1???
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	
	
	
	
	//????1????:?????????8?????1????......
	USART_InitStructure.USART_BaudRate = baud;										//???
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1????
	USART_InitStructure.USART_Parity = USART_Parity_No;								//?????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//??????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//??????
	USART_Init(USART1, &USART_InitStructure);
	
	
	//????1???????:??????????
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	//????1??????
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	//????1??
	USART_Cmd(USART1, ENABLE);


}



int main(void)
{
	
	//enable port F
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	//???GPIO??
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//?9???
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;	//????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//????,?????????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//?????????

	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	PFout(9)=1;
	
	
	//??1???:115200bps
	usart1_init(115200);
	
	
	
	USART_SendData(USART1,'G');
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
	USART_SendData(USART1,'E');	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
	
	USART_SendData(USART1,'C');	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);

	while(1)
	{
		
		
	}
}

static uint8_t g=0;

void USART1_IRQHandler(void)
{
	uint8_t d;
	
	//?????
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//????
		g=d=USART_ReceiveData(USART1);
		
		
		//???????,???PC
		USART_SendData(USART1,d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	


		//??LED1
		if(d == 0x01)PFout(9)=0;
		if(d == 0xF1)PFout(9)=1;		
	
	
		//?????
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}

}