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
    uint32_t Delay = ms * 72000 / 9; /* 72M时钟频率 */
    do
    {
        __NOP(); /* 空指令（NOP）来占用 CPU 时间 */
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
        第一个参数：x坐标，水平方向
        第二个参数：y坐标，垂直方向
        第三个参数：要显示的字符串
        第四个参数：字体高度（字库支持）
        第五个参数：显示模式，0反显（白底黑子），1正显（黑底白字）
     */
    // OLED_ShowString(0,10,"atguigu",16,1);
    OLED_ShowString(0,0,"BAT:       V",16,1);
    OLED_ShowString(0,16,"EA:",16,1);
    OLED_ShowString(0,32,"EB:",16,1);
    OLED_ShowString(0,48,"Angle:",16,1);
    OLED_Refresh();

    /* 进入FreeRTOS调度 */
    App_Task_Init();


    // Int_TB6612_SetPWM(3600,-3600);//测试得出：A是左轮，B是右轮

    // short gx=0,gy=0,gz=0;
    // short ax=0,ay=0,az=0;
    while (1)
    {
        /* =========================测试编码器读取值============================== */
        // printf("tim2 cnt=%d\r\n",Int_Encoder_ReadCounter(2));
        // printf("tim3 cnt=%d\r\n",Int_Encoder_ReadCounter(3));
        // for_delay_ms(1000);

        /* =========================测试MPU6050============================== */
        // Int_MPU6050_Get_Accel(&ax,&ay,&az);
        // Int_MPU6050_Get_Gyro(&gx,&gy,&gz);
        // printf("gx=%d\r\n",gx);
        // printf("gy=%d\r\n",gy);
        // printf("gz=%d\r\n",gz);
        // printf("ax=%d\r\n",ax);
        // printf("ay=%d\r\n",ay);
        // printf("az=%d\r\n",az);
        // for_delay_ms(100);


        /* =========================测试计算角度============================ */
        // App_Car_GetAngle();
        // for_delay_ms(10);//注意测试的时候，延迟时间不要太大，影响观察效果

        /* =========================测试计算角度============================ */
        // printf("电池电压=%.1f\r\n",Driver_ADC1_ReadV());
        // for_delay_ms(1000);

        /* =========================测试OLED显示============================ */
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
