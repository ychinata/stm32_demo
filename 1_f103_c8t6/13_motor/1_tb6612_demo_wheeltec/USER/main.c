#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "moto.h"
#include "pwm.h"

/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��5.7
�޸�ʱ�䣺2021-04-29

 
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update��2021-04-29

All rights reserved
***********************************************/



//------------����˵��---------------
//TB6612˿ӡ��ʶ--------STM32F1��������

//    PWMA    -----------    B6
//    AIN1    -----------    B8
//    AIN2    -----------    B7
//    STBY    -----------    5V
//    VM      -----------    5-12V����ӵ�Դ��
//    GND     -----------    GND  ����ӵ�Դ��
//    VCC     -----------    5V   ���߼���Դ��
//    GND     -----------    GND   ���߼����أ�
// 

//------------����˵��---------------

//TB6612˿ӡ��ʶ--------���
//    AO1   ------------  ����ߵ�Դ+
//    AO2   ------------  ����ߵ�Դ-
//------------����˵��---------------


 int main(void)
 {	
   SystemInit(); //����ϵͳʱ��Ϊ72M   
	 delay_init();    //��ʱ������ʼ��
   Gpio_Init();    //��ʼ��gpio��B pin_7/pin_8

   pwm_int(7199,0);      //��ʼ��pwm��� 72000 000 /7199+1=10000 
	
  while(1)
	{
	 moto(0);                //moto=1ʱ��ת
	 TIM_SetCompare1(TIM4,3000);   //����TIM4ͨ��1��ռ�ձ�  3000/7200
//	moto(1);                //moto=0ʱ��ת
//	TIM_SetCompare1(TIM4,4000);   //����TIM4ͨ��1��ռ�ձ�  4000/7200
	}
 }

