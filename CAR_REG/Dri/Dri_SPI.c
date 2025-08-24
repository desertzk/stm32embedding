#include "Dri_SPI.h"

void Driver_SPI_Init(void)
{
    /* 1. ����GPIOʱ�� PA*/
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* 2. �������ŵĹ���ģʽ */
    /* 2.1 cs: ������� PA4* MODE=11 CNF=00  */
    GPIOA->CRL |= GPIO_CRL_MODE4;
    GPIOA->CRL &= ~GPIO_CRL_CNF4;
    /* 2.2 sck: ������� PA5*/
    /* 2.3 mosi: ������� PA7*/
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
    GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
    /* 2.4 miso: �������� PA6  CNF=01 MODE=00*/
    GPIOA->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    /* 3. spi��ģʽ0  sck����״̬�� 0   */
    SCK_LOW;
    /* 4. ƬѡĬ�ϲ�ѡ�� */
    CS_HIGH;
}

uint8_t Driver_SPI_SwapByte(uint8_t byte)
{
    /* ����ѡ�� */
    CS_LOW;

    for (uint8_t i = 0; i < 8; i++)
    {
        SCK_LOW;
        /* 1. �Ȱ����ݷ��뵽MOSI�� */
        if (byte & 0x80)
        {
            MOSI_HIGH;
        }
        else
        {
            MOSI_LOW;
        }
        byte <<= 1;
        /* 2. ����ʱ�� (����������)*/
        SCK_HIGH;
    }

    /* ���߲�ѡ�� */
    CS_HIGH;

    return 0;
}
