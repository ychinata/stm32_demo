#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "moto.h"
#include "pwm.h"

/***********************************************
Author：轮趣科技（东莞）有限公司
Version：5.7
Date：2021-04-29
***********************************************/
int TEST_TB6612(void);
int TEST_DRV8833(void);

int main(void)
{
    //TEST_TB6612();
    TEST_DRV8833();
}

//TB6612
//Author:轮趣
int TEST_TB6612(void)
{	    
    SystemInit(); //配置系统时钟为72M   
    delay_init();    //延时函数初始化
    GPIO_Init_TB6612();    //初始化gpio口B pin_7/pin_8
    TIM4_PWM_Init(7199,0);      //初始化pwm输出 72000 000 /7199+1=10000 

    while(1) {       
        MOTOR_TURN_TB6612(0);                //moto=1时正转
        TIM_SetCompare1(TIM4,3000);   //设置TIM4通道1的占空比  3000/7200
        MOTOR_TURN_TB6612(1);                //moto=0时反转
        TIM_SetCompare1(TIM4,4000);   //设置TIM4通道1的占空比  4000/7200
    }
}

/********************************************
 *@Func: DRV8833驱动小车前进/后退
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
int TEST_DRV8833(void)
{  
    int speed = 500;//PWM_SPEED_MAX
    
    SystemInit(); //配置系统时钟为72M   
    delay_init();    //延时函数初始化            
    MOTOR_Init();  
    
    while(1) {
        //MOTOR_TURN_DRV8833(0);         
        CAR_Run(CAR_RUN_MOED_FORWARD, speed);
        delay_ms(5000);
        CAR_Run(CAR_RUN_MOED_BACKWARD, speed);
        delay_ms(5000);
    }
}
