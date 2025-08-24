#include "Dri_TIM.h"

/**
 * @description: ��ʱ��4��ʼ����CH3��CH4���pwm
 * @return {*}
 */
void Dri_TIM4_Init(void)
{
    /* ʱ�ӡ�gpio����ʱ��������pwmģʽ */

    /* 1.  ����ʱ��*/
    /* 1.1 ��ʱ��4��ʱ�� */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    /* 1.2 GPIO��ʱ�� PB */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 2. ����GPIO�ĸ���������� PA1 MODE=11,CNF=10 */
    GPIOB->CRH |= GPIO_CRH_MODE8;
    GPIOB->CRH |= GPIO_CRH_CNF8_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF8_0;

    GPIOB->CRH |= GPIO_CRH_MODE9;
    GPIOB->CRH |= GPIO_CRH_CNF9_1;
    GPIOB->CRH &= ~GPIO_CRH_CNF9_0;

    /* 3. ��ʱ������ͨ������ */
    /* pwm���Ƶ�� = 72Mʱ��Ƶ��/(��Ƶ*����ֵ) = 10k */
    /* 3.1 Ԥ��Ƶ�������� */
    TIM4->PSC = 1 - 1;
    /* 3.2 �Զ���װ�ؼĴ��������� */
    TIM4->ARR = 7200 - 1;
    /* 3.3 �������ļ������� 0=���� 1=����*/
    TIM4->CR1 &= ~TIM_CR1_DIR;

    /* 4. pwmģʽ����:ͨ��3 */
    /* 4.1 ����ͨ��3�Ĳ���ȽϼĴ��� */
    TIM4->CCR3 = 0;
    /* 4.2 ��ͨ��3����Ϊ���  CCMR2_CC3S=00 */
    TIM4->CCMR2 &= ~TIM_CCMR2_CC3S;
    /* 4.3 ����ͨ��������Ƚ�ģʽΪ��PWMģʽ1 ,CCMR2_OC3M=110*/
    TIM4->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM4->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM4->CCMR2 &= ~TIM_CCMR2_OC3M_0;
    /* 4.4 ʹ��ͨ��3  CCER_CC3E=1 */
    TIM4->CCER |= TIM_CCER_CC3E;
    /* 4.5 ����ͨ���ļ��� 0=�ߵ�ƽ��Ч  1=�͵�ƽ��Ч */
    TIM4->CCER &= ~TIM_CCER_CC3P;

    /* 5. pwmģʽ����:ͨ��4 */
    /* 5.1 ����ͨ��4�Ĳ���ȽϼĴ��� */
    TIM4->CCR4 = 0;
    /* 5.2 ��ͨ��4����Ϊ���  CCMR2_CC4S=00 */
    TIM4->CCMR2 &= ~TIM_CCMR2_CC4S;
    /* 5.3 ����ͨ��������Ƚ�ģʽΪ��PWMģʽ1 ,CCMR2_OC34M=110*/
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_2;
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_1;
    TIM4->CCMR2 &= ~TIM_CCMR2_OC4M_0;
    /* 5.4 ʹ��ͨ��3  CCER_CC4E=1 */
    TIM4->CCER |= TIM_CCER_CC4E;
    /* 5.5 ����ͨ���ļ��� 0=�ߵ�ƽ��Ч  1=�͵�ƽ��Ч */
    TIM4->CCER &= ~TIM_CCER_CC4P;

    /* 6.������ʱ�� */
    TIM4->CR1 |= TIM_CR1_CEN;
}

/**
 * @description: ��ʼ��Ϊ������ģʽ��A�����
 * @return {*}
 */
void Dri_TIM2_Init(void)
{
    /* ʱ�ӡ�GPIO����ʱ��ͨ�á�������ģʽ */
    /* 1.  ����ʱ��*/
    /* 1.1 ��ʱ��2��ʱ�� */
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /* 1.2 GPIO��ʱ�� PA��PB��AFIOʱ�� */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

    /* 2.GPIO��ӳ�� */
    /* 2.1 �ر�JTAG���ܣ�ע�Ᵽ��SWD��������������SWJ_CFG=010 */
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG_2;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG_0;
    /* 2.2 PA15��ӳ��ΪTIM2-CH1��PB3��ӳ��Ϊ TIM2-CH2, TIM2_REMAP=11 */
    AFIO->MAPR |= AFIO_MAPR_TIM2_REMAP_FULLREMAP;

    /* 2.3 ��ʼ�����ţ��������� mode=00��cnf=01 */
    GPIOA->CRH &= ~GPIO_CRH_MODE15;
    GPIOA->CRH &= ~GPIO_CRH_CNF15_1;
    GPIOA->CRH |= GPIO_CRH_CNF15_0;

    GPIOB->CRL &= ~GPIO_CRL_MODE3;
    GPIOB->CRL &= ~GPIO_CRL_CNF3_1;
    GPIOB->CRL |= GPIO_CRL_CNF3_0;

    /* 3. ��ʱ������ͨ������ */
    /* 3.1 Ԥ��Ƶ�������� */
    TIM2->PSC = 1 - 1;
    /* 3.2 �Զ���װ�ؼĴ��������� */
    TIM2->ARR = 65536 - 1;

    /* 4. ���ñ�����ģʽ */
    /* 4.1 ��������ͨ��ӳ�䣺IC1-TI1��IC2-TI2�� CC1S=01��CC2S=01 */
    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;

    TIM2->CCMR1 &= ~TIM_CCMR1_CC2S_1;
    TIM2->CCMR1 |= TIM_CCMR1_CC2S_0;

    /* 4.2 ���ò����� CC1P=0�� CC2P=0 */
    TIM2->CCER &= ~TIM_CCER_CC1P;
    TIM2->CCER &= ~TIM_CCER_CC2P;

    /* 4.3 ��·�źŶ�������������ģʽ3��SMS=011 */
    TIM2->SMCR &= ~TIM_SMCR_SMS_2;
    TIM2->SMCR |= TIM_SMCR_SMS_1;
    TIM2->SMCR |= TIM_SMCR_SMS_0;

    /* 5. ������ʱ�� */
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
 * @description: ��ʼ��Ϊ������ģʽ��B�����
 * @return {*}
 */
void Dri_TIM3_Init(void)
{
    /* ʱ�ӡ�GPIO����ʱ��ͨ�á�������ģʽ */
    /* 1.  ����ʱ��*/
    /* 1.1 ��ʱ��3��ʱ�� */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    /* 1.2 GPIO��ʱ�� PB��AFIOʱ�� */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

    /* 2.GPIO��ӳ�� */
    /* 2.1 �ر�JTAG���ܣ�ע�Ᵽ��SWD��������������SWJ_CFG=010 */
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG_2;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;
    AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG_0;
    /* 2.2 PB4��ӳ��ΪTIM3-CH1��PB5��ӳ��Ϊ TIM3-CH2, TIM3_REMAP=10 */
    AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_PARTIALREMAP;

    /* 2.3 ��ʼ�����ţ��������� mode=00��cnf=01 */
    GPIOB->CRL &= ~GPIO_CRL_MODE4;
    GPIOB->CRL &= ~GPIO_CRL_CNF4_1;
    GPIOB->CRL |= GPIO_CRL_CNF4_0;

    GPIOB->CRL &= ~GPIO_CRL_MODE5;
    GPIOB->CRL &= ~GPIO_CRL_CNF5_1;
    GPIOB->CRL |= GPIO_CRL_CNF5_0;

    /* 3. ��ʱ������ͨ������ */
    /* 3.1 Ԥ��Ƶ�������� */
    TIM3->PSC = 1 - 1;
    /* 3.2 �Զ���װ�ؼĴ��������� */
    TIM3->ARR = 65536 - 1;

    /* 4. ���ñ�����ģʽ */
    /* 4.1 ��������ͨ��ӳ�䣺IC1-TI1��IC2-TI2�� CC1S=01��CC2S=01 */
    TIM3->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;

    TIM3->CCMR1 &= ~TIM_CCMR1_CC2S_1;
    TIM3->CCMR1 |= TIM_CCMR1_CC2S_0;

    /* 4.2 ���ò����� CC1P=0�� CC2P=0 */
    TIM3->CCER &= ~TIM_CCER_CC1P;
    TIM3->CCER &= ~TIM_CCER_CC2P;

    /* 4.3 ��·�źŶ�������������ģʽ3��SMS=011 */
    TIM3->SMCR &= ~TIM_SMCR_SMS_2;
    TIM3->SMCR |= TIM_SMCR_SMS_1;
    TIM3->SMCR |= TIM_SMCR_SMS_0;

    /* 5. ������ʱ�� */
    TIM3->CR1 |= TIM_CR1_CEN;
}
