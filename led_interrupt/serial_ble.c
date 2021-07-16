#include "stm32f4xx.h"
#include "sys.h"
#include "common.h"
#include<stdio.h>

static GPIO_InitTypeDef 		GPIO_InitStructure;
static NVIC_InitTypeDef 		NVIC_InitStructure;
static USART_InitTypeDef 		USART_InitStructure;



void usart1_init(uint32_t baud)
{
	
	//??PA????	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	

	//????1????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	////配置PA9和PA10为复用功能模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;		//?9 10???
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;	//?????
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//????,?????????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//?????????

	GPIO_Init(GPIOA,&GPIO_InitStructure);


	//?PA9?PA10???????1???
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	
	
	
	
	//配置串口1相关参数：波特率、无校验位、8位数据位、1个停止位......
	USART_InitStructure.USART_BaudRate = baud;										//???
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1????
	USART_InitStructure.USART_Parity = USART_Parity_No;								//?????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//??????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//??????
	USART_Init(USART1, &USART_InitStructure);
	
	
	//配置串口1的中断触发方法：接收一个字节触发中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	//set uart1 interrupt priority level
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	//make uart1 enable 
	USART_Cmd(USART1, ENABLE);
}

void usart3_init(uint32_t baud)
{
	
	//打开PB硬件时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	

	//打开串口3硬件时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//配置PB10和PB10为复用功能模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;		//第10 11根引脚
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;	//多功能模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出，增加输出电流能力。
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//高速响应
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//没有使能上下拉电阻

	GPIO_Init(GPIOB,&GPIO_InitStructure);


	//将PB10和PB11引脚连接到串口1的硬件
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);	
	
	
	
	//配置串口3相关参数：波特率、无校验位、8位数据位、1个停止位......
	USART_InitStructure.USART_BaudRate = baud;										//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//允许收发数据
	USART_Init(USART3, &USART_InitStructure);
	
	
	//配置串口3的中断触发方法：接收一个字节触发中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	
	//配置串口3的中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	//使能串口3工作
	USART_Cmd(USART3, ENABLE);
}

void usart3_send_str(char *pstr)
{
	char *p = pstr;
	
	while(*p!='\0')
	{
	
		USART_SendData(USART3,*p);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USART3,USART_FLAG_TXE);

		//指针偏移
		p++;
	}
	

}
//AT指令配置模块，蓝牙4.0模块不能跟手机进行连接
void ble_config_set(void)
{
	//发送AT的测试指令
	usart3_send_str("AT\r\n");
	delay_ms(500);

	//发送更改模块名字指令（需要复位生效，如果发送AT+RESET搜索蓝牙模块名字没有变更，那么请重新对蓝牙模块上电）
	usart3_send_str("AT+NAME zk\r\n");
	delay_ms(500);
	
	//发送复位模块的指令
	usart3_send_str("AT+RESET\r\n");
	delay_ms(2000);

}


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
	return ch;
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
	
	
	//set  baud rate:115200bps
	usart1_init(115200);
	
		//串口3波特率:9600bps
	usart3_init(9600);
	
	USART_SendData(USART1,'G');
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
	USART_SendData(USART1,'E');	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);	
	
	USART_SendData(USART1,'C');	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
  printf("yuyuyuyuyuyu  zkzkzkzkzkzk teacher.wenzzzzzzzzzzz\r\n");
		//蓝牙模块的配置
	ble_config_set();
	while(1)
	{
		
		
	}
}

static uint8_t g=0;

void USART1_IRQHandler(void)
{
	uint8_t d;
	
	//检测标志位
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		//接收数据
		d=USART_ReceiveData(USART1);
		
		
	
		//清空标志位
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}

}

void USART3_IRQHandler(void)
{
	uint8_t d;
	
	//检测标志位
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		//接收数据
		d=USART_ReceiveData(USART3);
		
		
		//通过串口1发送给PC
		USART_SendData(USART1,d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		USART_ClearFlag(USART1,USART_FLAG_TXE);
		

		//清空标志位
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}

}