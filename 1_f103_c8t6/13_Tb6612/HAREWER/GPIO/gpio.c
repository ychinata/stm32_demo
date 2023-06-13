#include "gpio.h"
#include "moto.h"

void Gpio_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;            //����ṹ��GPIO_InitStructure

    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC, ENABLE); // ʹ��PB�˿�ʱ��  
    GPIO_InitStructure.GPIO_Pin =   MOTOR_PIN_AIN1 | MOTOR_PIN_AIN2 | MOTOR_PIN_BIN1 | MOTOR_PIN_BIN2;	  //PB7 PB8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     	//���죬��������������  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO���ٶ�
    GPIO_Init(MOTOR_IN_GPIO, &GPIO_InitStructure);          //GBIOB��ʼ��  
	
}
