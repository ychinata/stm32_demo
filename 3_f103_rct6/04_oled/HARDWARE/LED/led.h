#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

/*Ӳ��˵����
    ����ԭ��F103mini��LED0(PA8)��LED1(PD2)
    ��Խ����MiniPro���İ壺LED0(PA8)

 */

//�޸�LED IO - Bgn
//LED0 -> PA8
#define PINNo_LED_0   8
#define GPIO_LED_0 GPIOA
#define RCCAPB2PeriphGPIO_LED_0 RCC_APB2Periph_GPIOA
#define LED0 PAout(PINNo_LED_0)
//LED1 -> PD2
#define PINNo_LED_1   2
#define GPIO_LED_1 GPIOD
#define RCCAPB2PeriphGPIO_LED_1 RCC_APB2Periph_GPIOD
#define LED1 PDout(PINNo_LED_1)
//�޸�LED IO - End

#define GPIO_Pin_LED0                 ((uint16_t)(0x1<<PINNo_LED_0))
#define GPIO_Pin_LED1                 ((uint16_t)(0x1<<PINNo_LED_1))

void LED_Init(void);//��ʼ��

		 				    
#endif
