	
/********************************************************************************

//���빦�ܣ�  ��׼�� GPIO ����

//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/


#ifndef __gpio_H
#define __gpio_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f4xx.h"
#include "main.h"

//LED�˿ڶ���
	 
#define LED2_ON     GPIO_ResetBits(GPIOC,GPIO_Pin_13)  	//LED2�� IO����͵�ƽ--����
#define LED2_OFF    GPIO_SetBits(GPIOC,GPIO_Pin_13)   	//LED2�� IO����ߵ�ƽ--Ϩ��	 
	 
	 
void GPIO_Config(void);

#ifdef __cplusplus
}
#endif

#endif






























/********************************************************************************

//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/





