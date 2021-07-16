#include"stm32f4xx.h"
#include<stdio.h>
#include"sys.h"
#include"common.h"

void delay(void)
{
	uint32_t i=0x1000000;
	 
	while(i--);
}





void exti0_init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
  EXTI_InitTypeDef   	EXTI_InitStructure;
  NVIC_InitTypeDef   	NVIC_InitStructure;
	//使能打开端口E的硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	
	//enable system clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//init PE3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		//?0???
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN;		//????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//?????????
	GPIO_Init(GPIOE,&GPIO_InitStructure);	


	//bind PE3 EXTI3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);

	//config extern interupt
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;		//interrupt 3
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//??
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//?????,?????????;?????,?????????;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//??
	EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//open interrupt3 request
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;//?????
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;		//?????
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//?????
	NVIC_Init(&NVIC_InitStructure);
	
}


int main01(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //第7引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//

	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	exti0_init();
	
	while(1)
	{
		//PA6
		//char *odr6 = (char *)(&GPIOA->ODR+GPIO_Pin_6);
		//*odr6 = 0;
		
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		//PAout(7)=0;

		
		delay_ms(1000);
		
		//PA6
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		//PAout(7)=1;
	  //*odr6 = 1;
		delay_ms(1000);
			
		
		
	}
}

void EXTI3_IRQHandler(void)
{

	//?????????
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //第7引脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		
		delay();
		
		//PA6
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	  //*odr6 = 1;
		delay();	
	
		/*  offer CPU to access other interupt 
		
		???????,????????
		*/
		EXTI_ClearITPendingBit(EXTI_Line3);
	}

}
