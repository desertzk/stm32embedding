#include "Dri_USART2.h"

/**
 * @description: ��ʼ������2
 */
void Driver_USART2_Init(void)
{
    /* 1. ����ʱ�� */
    /* 1.1 ����2�����ʱ�� */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    /* 1.2 GPIOʱ�� */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2. ����GPIO���ŵĹ���ģʽ  PA2=Tx(�������� CNF=10 MODE=11)  PA3=Rx(�������� CNF=01 MODE=00)*/
    GPIOA->CRL |= GPIO_CRL_CNF2_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF2_0;
    GPIOA->CRL |= GPIO_CRL_MODE2;

    GPIOA->CRL &= ~GPIO_CRL_CNF3_1;
    GPIOA->CRL |= GPIO_CRL_CNF3_0;
    GPIOA->CRL &= ~GPIO_CRL_MODE3;

    /* 3. ���ڵĲ������� */
    /* 3.1 ���ò����� 9600, ֵΪ234.375 */
    USART2->BRR = 0xEA6;
    /* 3.2 ����һ���ֵĳ��� 8λ */
    USART2->CR1 &= ~USART_CR1_M;
    /* 3.3 ���ò���ҪУ��λ */
    USART2->CR1 &= ~USART_CR1_PCE;
    /* 3.4 ����ֹͣλ�ĳ��� */
    USART2->CR2 &= ~USART_CR2_STOP;
    /* 3.5 ʹ�ܽ��պͷ��� */
    USART2->CR1 |= USART_CR1_TE;
    USART2->CR1 |= USART_CR1_RE;

    /* 3.6 ʹ�ܴ��ڵĸ����ж� */
    USART2->CR1 |= USART_CR1_RXNEIE; /* ���շǿ��ж� */
    
    /* ʹ������ʱ�����ÿ������жϣ�û���õ���һ����Ҫ�� */
    // USART2->CR1 |= USART_CR1_IDLEIE; /* �����ж� */

    /* 4. ����NVIC */
    /* 4.1 �������ȼ��� */
    NVIC_SetPriorityGrouping(3);
    /* 4.2 �������ȼ� */
    NVIC_SetPriority(USART2_IRQn, 2);
    /* 4.3 ʹ�ܴ���2���ж� */
    NVIC_EnableIRQ(USART2_IRQn);

    /* 4. ʹ�ܴ��� */
    USART2->CR1 |= USART_CR1_UE;
}

/**
 * @description: ����һ���ֽ�
 * @param {uint8_t} byte Ҫ���͵��ֽ�
 */
void Driver_USART2_SendChar(uint8_t byte)
{
    /* 1. �ȴ����ͼĴ���Ϊ�� */
    while ((USART2->SR & USART_SR_TXE) == 0)
        ;

    /* 2. ����д�������ݼĴ��� */
    USART2->DR = byte;
}

/**
 * @description: ����һ���ַ���
 * @param {uint8_t} *str Ҫ���͵��ַ���
 * @param {uint16_t} len �ַ������ֽڵĳ���
 * @return {*}
 */
void Driver_USART2_SendString(uint8_t *str, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        Driver_USART2_SendChar(str[i]);
    }
}

/**
 * @description: ����һ���ֽڵ�����
 * @return {*} ���յ����ֽ�
 */
uint8_t Driver_USART2_ReceiveChar(void)
{
    /* �ȴ����ݼĴ����ǿ� */
    while ((USART2->SR & USART_SR_RXNE) == 0)
        ;
    return USART2->DR;
}

/**
 * @description: ���ձ䳤����.���յ������ݴ��뵽buff��
 * @param {uint8_t} buff ��Ž��յ�������
 * @param {uint8_t} *len ����յ������ݵ��ֽڵĳ���
 */
void Driver_USART2_ReceiveString(uint8_t buff[], uint8_t *len)
{
    uint8_t i = 0;
    while (1)
    {
        // �ȴ����շǿ�
        while ((USART2->SR & USART_SR_RXNE) == 0)
        {
            // �ڵȴ��ڼ�, �ж��Ƿ��յ�����֡
            if (USART2->SR & USART_SR_IDLE)
            {
                
                *len = i;
                return;
            }
        }
        buff[i] = USART2->DR;
        i++;
    }
}

// /* ������յ������� */
// uint8_t buff2[100] = {0};
// /* �洢���յ����ֽڵĳ��� */
// uint8_t len2 = 0;
// uint8_t isToSend2 = 0;
// void USART2_IRQHandler(void)
// {
//     /* ���ݽ��ռĴ����ǿ� */
//     if (USART2->SR & USART_SR_RXNE)
//     {
//         // ��USART_DR�Ķ��������Խ����շǿյ��ж�λ���㡣 ���Բ��õ��������.
//         //USART1->SR &= ~USART_SR_RXNE;
//         buff2[len2] = USART2->DR;
//         len2++;
//     }
//     else if (USART2->SR & USART_SR_IDLE)
//     {
//         /* ��������жϱ�־λ: �ȶ�sr,�ٶ�dr.�Ϳ���ʵ������� */
//         USART2->SR;
//         USART2->DR;
//         /* �䳤���ݽ������ */
//         Driver_USART1_SendString(buff2, len2);
//         isToSend2 = 1;
//         /* �ѽ����ֽڵĳ�����0 */
//         len2 = 0;
//     }
// }


