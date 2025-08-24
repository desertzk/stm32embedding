#include "Int_Encoder.h"

/**
 * @description: ��ʼ��
 * @return {*}
 */
void Int_Encoder_Init(void)
{
    /* ��ʼ����ʱ��Ϊ������ģʽ */
    Dri_TIM2_Init();
    Dri_TIM3_Init();
}

/**
 * @description: ��ȡ������ģʽ�ļ���ֵ������ɴ����ŵ�ֵ
 * @param {uint8_t} timx Ҫ��ȡ�Ķ�ʱ���ı��
 * @return {*} �����Ĵ����ŵļ���ֵ
 */
int Int_Encoder_ReadCounter(uint8_t timx)
{
    int encoder_value = 0;
    switch (timx)
    {
    case 2:
    {
        /* ��ȡ��ʱ��2�ļ���ֵ������ת���ɴ��������� */
        encoder_value = (short)TIM2->CNT;
        TIM2->CNT = 0;
        break;
    }
    case 3:
    {
        /* ��ȡ��ʱ��3�ļ���ֵ������ת���ɴ��������� */
        encoder_value = (short)TIM3->CNT;
        TIM3->CNT = 0;
        break;
    }
    default:
        break;
    }

    /* ���ڵ����װ�����180�ȣ�������ȡʱ��Ҫ�ֶ�ͳһ������� */
    return encoder_value;
}
