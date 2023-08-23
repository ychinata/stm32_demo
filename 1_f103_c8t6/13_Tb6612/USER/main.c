#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "moto.h"
#include "pwm.h"

/***********************************************
Author����Ȥ�Ƽ�����ݸ�����޹�˾
Version��5.7
Date��2021-04-29
***********************************************/



//------------TB6612����˵��---------------
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
//TB6612˿ӡ��ʶ--------���
//    AO1   ------------  ����ߵ�Դ+
//    AO2   ------------  ����ߵ�Դ-
//------------DRV8833����˵��---------------
//DRV8833
//AIN1 <-> PA7
//AIN2 <-> PA3
//BIN1 <-> PA6
//BIN2 <-> PA4
//------------����˵��---------------

//TB6612
//Author:��Ȥ
int main1(void)
{	    
    SystemInit(); //����ϵͳʱ��Ϊ72M   
    delay_init();    //��ʱ������ʼ��
    Gpio_Init();    //��ʼ��gpio��B pin_7/pin_8
    TIM4_PWM_Init(7199,0);      //��ʼ��pwm��� 72000 000 /7199+1=10000 

    while(1) {       
        MOTOR_TURN_TB6612(0);                //moto=1ʱ��ת
        TIM_SetCompare1(TIM4,3000);   //����TIM4ͨ��1��ռ�ձ�  3000/7200
        MOTOR_TURN_TB6612(1);                //moto=0ʱ��ת
        TIM_SetCompare1(TIM4,4000);   //����TIM4ͨ��1��ռ�ձ�  4000/7200
    }
}

/********************************************
 *@Func: DRV8833����С��ǰ��/����
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
int main(void)
{  
    int speed = 300;//PWM_SPEED_MAX
    
    SystemInit(); //����ϵͳʱ��Ϊ72M   
    delay_init();    //��ʱ������ʼ��            
    MOTOR_DRV8833Init();  
    
    while(1) {
        //MOTOR_TURN_DRV8833(0);         
        CAR_Run_DRV8833With2PWM(CAR_RUN_MOED_FORWARD, speed);
        delay_ms(5000);
        CAR_Run_DRV8833With2PWM(CAR_RUN_MOED_BACKWARD, speed);
        delay_ms(5000);
    }
}
