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
int TEST_TB6612(void);
int TEST_DRV8833(void);

int main(void)
{
    //TEST_TB6612();
    TEST_DRV8833();
}

//TB6612
//Author:��Ȥ
int TEST_TB6612(void)
{	    
    SystemInit(); //����ϵͳʱ��Ϊ72M   
    delay_init();    //��ʱ������ʼ��
    GPIO_Init_TB6612();    //��ʼ��gpio��B pin_7/pin_8
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
int TEST_DRV8833(void)
{  
    int speed = 500;//PWM_SPEED_MAX
    
    SystemInit(); //����ϵͳʱ��Ϊ72M   
    delay_init();    //��ʱ������ʼ��            
    MOTOR_Init();  
    
    while(1) {
        //MOTOR_TURN_DRV8833(0);         
        CAR_Run(CAR_RUN_MOED_FORWARD, speed);
        delay_ms(5000);
        CAR_Run(CAR_RUN_MOED_BACKWARD, speed);
        delay_ms(5000);
    }
}
