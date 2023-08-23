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
