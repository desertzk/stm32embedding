#include "main.h"
#include "freertos.h"
#include "task.h"
#include "Int_TB6612.h"
#include "Int_Encoder.h"
#include "Dri_USART1.h"
#include "Dri_USART2.h"
#include "Int_MPU6050.h"
#include "App_Car.h"
#include "Dri_ADC.h"
#include "oled.h"
#include "App_Task.h"

void for_delay_ms(uint32_t ms)
{
    uint32_t Delay = ms * 72000 / 9; /* 72Mʱ��Ƶ�� */
    do
    {
        __NOP(); /* ��ָ�NOP����ռ�� CPU ʱ�� */
    } while (Delay--);
}



int main(void)
{
    Driver_USART1_Init();
    Driver_USART2_Init();

	Int_TB6612_Init();

    Int_Encoder_Init();

    Int_MPU6050_Init();

    Driver_ADC1_Init();

    OLED_Init();
    OLED_Clear();
    /* 
        ��һ��������x���꣬ˮƽ����
        �ڶ���������y���꣬��ֱ����
        ������������Ҫ��ʾ���ַ���
        ���ĸ�����������߶ȣ��ֿ�֧�֣�
        �������������ʾģʽ��0���ԣ��׵׺��ӣ���1���ԣ��ڵװ��֣�
     */
    // OLED_ShowString(0,10,"atguigu",16,1);
    OLED_ShowString(0,0,"BAT:       V",16,1);
    OLED_ShowString(0,16,"EA:",16,1);
    OLED_ShowString(0,32,"EB:",16,1);
    OLED_ShowString(0,48,"Angle:",16,1);
    OLED_Refresh();

    /* ����FreeRTOS���� */
    App_Task_Init();


    // Int_TB6612_SetPWM(3600,-3600);//���Եó���A�����֣�B������

    // short gx=0,gy=0,gz=0;
    // short ax=0,ay=0,az=0;
    while (1)
    {
        /* =========================���Ա�������ȡֵ============================== */
        // printf("tim2 cnt=%d\r\n",Int_Encoder_ReadCounter(2));
        // printf("tim3 cnt=%d\r\n",Int_Encoder_ReadCounter(3));
        // for_delay_ms(1000);

        /* =========================����MPU6050============================== */
        // Int_MPU6050_Get_Accel(&ax,&ay,&az);
        // Int_MPU6050_Get_Gyro(&gx,&gy,&gz);
        // printf("gx=%d\r\n",gx);
        // printf("gy=%d\r\n",gy);
        // printf("gz=%d\r\n",gz);
        // printf("ax=%d\r\n",ax);
        // printf("ay=%d\r\n",ay);
        // printf("az=%d\r\n",az);
        // for_delay_ms(100);


        /* =========================���Լ���Ƕ�============================ */
        // App_Car_GetAngle();
        // for_delay_ms(10);//ע����Ե�ʱ���ӳ�ʱ�䲻Ҫ̫��Ӱ��۲�Ч��

        /* =========================���Լ���Ƕ�============================ */
        // printf("��ص�ѹ=%.1f\r\n",Driver_ADC1_ReadV());
        // for_delay_ms(1000);

        /* =========================����OLED��ʾ============================ */
        // App_Car_GetAngle();
        // App_Car_Display();
        // for_delay_ms(10);
    }
}



extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}
