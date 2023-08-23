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



//------------TB6612接线说明---------------
//TB6612丝印标识--------STM32F1主板引脚
//    PWMA    -----------    B6
//    AIN1    -----------    B8
//    AIN2    -----------    B7
//    STBY    -----------    5V
//    VM      -----------    5-12V（外接电源）
//    GND     -----------    GND  （外接电源）
//    VCC     -----------    5V   （逻辑电源）
//    GND     -----------    GND   （逻辑共地）
// 
//TB6612丝印标识--------电机
//    AO1   ------------  电机线电源+
//    AO2   ------------  电机线电源-
//------------DRV8833接线说明---------------
//DRV8833
//AIN1 <-> PA7
//AIN2 <-> PA3
//BIN1 <-> PA6
//BIN2 <-> PA4
//------------接线说明---------------

//TB6612
//Author:轮趣
int main1(void)
{	    
    SystemInit(); //配置系统时钟为72M   
    delay_init();    //延时函数初始化
    Gpio_Init();    //初始化gpio口B pin_7/pin_8
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
int main(void)
{  
    int speed = 300;//PWM_SPEED_MAX
    
    SystemInit(); //配置系统时钟为72M   
    delay_init();    //延时函数初始化            
    MOTOR_DRV8833Init();  
    
    while(1) {
        //MOTOR_TURN_DRV8833(0);         
        CAR_Run_DRV8833With2PWM(CAR_RUN_MOED_FORWARD, speed);
        delay_ms(5000);
        CAR_Run_DRV8833With2PWM(CAR_RUN_MOED_BACKWARD, speed);
        delay_ms(5000);
    }
}
