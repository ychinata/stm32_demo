#include "gpio.h"
#include "motor.h"

void GPIO_Init_TB6612(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;            //定义结构体GPIO_InitStructure

    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC, ENABLE); // 使能PB端口时钟  
    GPIO_InitStructure.GPIO_Pin =   MOTOR_PIN_AIN1 | MOTOR_PIN_AIN2 | MOTOR_PIN_BIN1 | MOTOR_PIN_BIN2;	  //PB7 PB8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     	//推挽，增大电流输出能力  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO口速度
    GPIO_Init(MOTOR_IN_GPIO, &GPIO_InitStructure);          //GBIOB初始化  	
}

/********************************************
 *@Func: GPIO初始化TB6612两个引脚PWMA/B
 *@Author: xxy
 *@Date: 2024.6.15
*********************************************/
void GPIO_Init_TB6612Pwm(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 根据TIMx选择不同的APBx
    RCC_APB2PeriphClockCmd(MOTOR_PWM_RCC , ENABLE);  //使能GPIO外设时钟使能
    
    //设置该引脚为复用输出功能,输出TIM1 CH1 CH4的PWM脉冲波形
    GPIO_InitStructure.GPIO_Pin = MOTOR_PIN_PWMA | MOTOR_PIN_PWMB; //TIM_CH1 //TIM_CH4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PWM_GPIO, &GPIO_InitStructure);
}

/********************************************
 *@Func: 仿照HAL库风格将GPIO初始化单独实现
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
void GPIO_Init_DRV8833(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC,ENABLE);

    GPIO_InitStructure.GPIO_Pin=MOTOR_PIN_AIN2|MOTOR_PIN_BIN2;//AIN2/BIN2作为方向控制
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_IN_GPIO,&GPIO_InitStructure);
    GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2|MOTOR_PIN_BIN2);
}
