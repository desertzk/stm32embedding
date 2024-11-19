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
	//ʹ�ܴ򿪶˿�E��Ӳ��ʱ��
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

//led turn on/off
int main01(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //��7����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //output
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//Push output, increase output power.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//no pull-up resistor or pull-down resistor

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

//led turn on/off
int main04(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //��7����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//

	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	exti0_init();
	
	while(1)
	{

		//PAout(7)=1;
	  //*odr6 = 1;
		delay_ms(1000);
			
		
		
	}
}

//interrupt
void EXTI3_IRQHandler(void)
{

	//?????????
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //Pin_6
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


//PWM
static GPIO_InitTypeDef 		GPIO_InitStructure;
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef  		TIM_OCInitStructure;

void tim3_init(void)
{
	
	//Square wave frequency of the timer output
	int frequency=20;
	//enable TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	
	//Configure the prescaler value of Timer 3
	TIM_TimeBaseStructure.TIM_Period = (10000/frequency)-1;	//  ����ֵ��0~499���������Ƶ��Ϊ20Hz  interrupt frequency -> Output frequency
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;		//4200-1+1=4200,����4200��Ԥ��Ƶֵ�����е�һ�η�Ƶ
	//TIM_TimeBaseStructure.TIM_ClockDivision = 0;		//��F407�ǲ�֧��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//���ϼ����ķ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	
	/* CH1 work in  PWM1  mode*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//��/�ر��������
	TIM_OCInitStructure.TIM_Pulse = 490;						 //compare value
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	 //��Ч״̬Ϊ�ߵ�ƽ������Ч״̬Ϊ�͵�ƽ
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	
	
	//ʹ�ܶ�ʱ��3����
	TIM_Cmd(TIM3, ENABLE);

}

//breathing light
int main(void)
{
	
	int32_t pwm_cmp=0;
	
	//ʹ��(��)�˿�C��Ӳ��ʱ�ӣ����ǶԶ˿�C����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//��ʼ��GPIO����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		//��6������
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;		//���ù���ģʽ�������Ž�������Ӳ���Զ�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������������������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//������Ӧ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//û��ʹ������������

	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//set PA6 to TIM3 mode
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	
	
	//��ʱ��3�ĳ�ʼ��
	tim3_init();
	
	while(1)
	{
		for(pwm_cmp=0;pwm_cmp<=499;pwm_cmp++)
		{
			TIM_SetCompare1(TIM3,pwm_cmp);
			delay_ms(20);
		}
		delay_ms(1000);
		for(pwm_cmp=499;pwm_cmp>0;pwm_cmp--)
		{
			TIM_SetCompare1(TIM3,pwm_cmp);
			delay_ms(20);
		}
	}
}
