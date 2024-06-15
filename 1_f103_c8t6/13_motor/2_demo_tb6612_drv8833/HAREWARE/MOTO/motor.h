#ifndef __MOTO_H
#define	__MOTO_H

#include "stm32f10x.h"

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

/*
//huifei-DRV8833
#define MOTOR_IN_GPIO      GPIOA
// left
#define MOTOR_PIN_AIN1 GPIO_Pin_7//T3C2
#define MOTOR_PIN_AIN2 GPIO_Pin_3
//right
#define MOTOR_PIN_BIN1 GPIO_Pin_6//T3C1
#define MOTOR_PIN_BIN2 GPIO_Pin_4
#define MOTOR_IN_RCC    RCC_APB2Periph_GPIOA
//huifei-DRV8833
*/

//轮趣平衡车B570：TB6612
#define MOTOR_IN_RCC    RCC_APB2Periph_GPIOB
#define MOTOR_IN_GPIO      GPIOB    //假设四个IN为同一GPIO组
#define MOTOR_PIN_AIN1  GPIO_Pin_14
#define MOTOR_PIN_AIN2  GPIO_Pin_15
#define MOTOR_PIN_BIN1  GPIO_Pin_13
#define MOTOR_PIN_BIN2  GPIO_Pin_12

#define MOTOR_PWM_RCC       RCC_APB2Periph_GPIOA
#define MOTOR_PWM_GPIO      GPIOA       ////假设2个PWM为同一GPIO组
#define MOTOR_PIN_PWMA      GPIO_Pin_8
#define MOTOR_PIN_PWMB      GPIO_Pin_11
#define MOTOR_PX_OUT PBout
//宏的名字建议加MOTOR_前缀
#define AIN1   MOTOR_PX_OUT(MOTOR_PIN_AIN1)
#define AIN2   MOTOR_PX_OUT(MOTOR_PIN_AIN2)
#define BIN1   MOTOR_PX_OUT(MOTOR_PIN_BIN1)
#define BIN2   MOTOR_PX_OUT(MOTOR_PIN_BIN2)
#define PWMA   TIM1->CCR1  //PA8
#define PWMB   TIM1->CCR4  //PA11
//轮趣平衡车B570：TB6612

// 以下两种驱动只支持一种
//#define MOTOR_DRV8833
#define MOTOR_TB6612

// 宏定义
#define MOTOR_TURN_MODE_FORWARD  0
#define MOTOR_TURN_MODE_REVERSAL 1
//
#define CAR_RUN_MOED_FORWARD  0
#define CAR_RUN_MOED_BACKWARD 1
#define CAR_RUN_MOED_TURNLEFT 2
#define CAR_RUN_MOED_TURNRIGHT 3

// 内部函数声明
void MOTOR_TURN_TB6612(int mode);
void MOTOR_TURN_DRV8833(int mode);
// 外部函数声明
void MOTOR_Init(void);
void CAR_Run(int carRunMode, int speed);

#endif
