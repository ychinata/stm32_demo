#include "gpio.h"
#include "moto.h"

void Gpio_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;            //定义结构体GPIO_InitStructure

    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC, ENABLE); // 使能PB端口时钟  
    GPIO_InitStructure.GPIO_Pin =   MOTOR_PIN_AIN1 | MOTOR_PIN_AIN2 | MOTOR_PIN_BIN1 | MOTOR_PIN_BIN2;	  //PB7 PB8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     	//推挽，增大电流输出能力  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO口速度
    GPIO_Init(MOTOR_IN_GPIO, &GPIO_InitStructure);          //GBIOB初始化  
	
}
