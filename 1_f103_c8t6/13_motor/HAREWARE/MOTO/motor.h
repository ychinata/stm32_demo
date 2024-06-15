#ifndef __MOTO_H
#define	__MOTO_H

#include "stm32f10x.h"

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

//��Ȥƽ�⳵B570��TB6612
#define MOTOR_IN_RCC    RCC_APB2Periph_GPIOB
#define MOTOR_IN_GPIO      GPIOB    //�����ĸ�INΪͬһGPIO��
#define MOTOR_PIN_AIN1  GPIO_Pin_14
#define MOTOR_PIN_AIN2  GPIO_Pin_15
#define MOTOR_PIN_BIN1  GPIO_Pin_13
#define MOTOR_PIN_BIN2  GPIO_Pin_12

#define MOTOR_PWM_RCC       RCC_APB2Periph_GPIOA
#define MOTOR_PWM_GPIO      GPIOA       ////����2��PWMΪͬһGPIO��
#define MOTOR_PIN_PWMA      GPIO_Pin_8
#define MOTOR_PIN_PWMB      GPIO_Pin_11
#define MOTOR_PX_OUT PBout
//������ֽ����MOTOR_ǰ׺
#define AIN1   MOTOR_PX_OUT(MOTOR_PIN_AIN1)
#define AIN2   MOTOR_PX_OUT(MOTOR_PIN_AIN2)
#define BIN1   MOTOR_PX_OUT(MOTOR_PIN_BIN1)
#define BIN2   MOTOR_PX_OUT(MOTOR_PIN_BIN2)
#define PWMA   TIM1->CCR1  //PA8
#define PWMB   TIM1->CCR4  //PA11
//��Ȥƽ�⳵B570��TB6612

// ������������ֻ֧��һ��
//#define MOTOR_DRV8833
#define MOTOR_TB6612

// �궨��
#define MOTOR_TURN_MODE_FORWARD  0
#define MOTOR_TURN_MODE_REVERSAL 1
//
#define CAR_RUN_MOED_FORWARD  0
#define CAR_RUN_MOED_BACKWARD 1
#define CAR_RUN_MOED_TURNLEFT 2
#define CAR_RUN_MOED_TURNRIGHT 3

// �ڲ���������
void MOTOR_TURN_TB6612(int mode);
void MOTOR_TURN_DRV8833(int mode);
// �ⲿ��������
void MOTOR_Init(void);
void CAR_Run(int carRunMode, int speed);

#endif
