#include "stm32f10x.h"                  // Device header

// 引脚可修改
#define GPIO_PIN_LED1 GPIO_Pin_11
#define GPIO_PIN_LED2 GPIO_Pin_13
#define GPIO_PORT_LED1   GPIOA
#define GPIO_PORT_LED2   GPIOC
#define RCC_GPIO_LED1 RCC_APB2Periph_GPIOA
#define RCC_GPIO_LED2 RCC_APB2Periph_GPIOC


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_GPIO_LED1 | RCC_GPIO_LED2, ENABLE);	
    // LED1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);
	GPIO_SetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);    
    // LED2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);    	
	GPIO_SetBits(GPIO_PORT_LED2, GPIO_PIN_LED2);
}

// 低电平点亮LED, 负极接IO口
void LED1_ON(void)
{
	GPIO_ResetBits(GPIO_PORT_LED1, GPIO_PIN_LED1); 
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);
}

void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIO_PORT_LED1, GPIO_PIN_LED1) == 0) {
		GPIO_SetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);     // 输出高电平
	} else {
		GPIO_ResetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);   // 输出低电平
	}
}

void LED2_ON(void)
{
	GPIO_ResetBits(GPIO_PORT_LED2, GPIO_PIN_LED2);
}

void LED2_OFF(void)
{
	GPIO_SetBits(GPIO_PORT_LED2, GPIO_PIN_LED2);
}

void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIO_PORT_LED2, GPIO_PIN_LED2) == 0) {
		GPIO_SetBits(GPIO_PORT_LED2, GPIO_PIN_LED2);
	} else {
		GPIO_ResetBits(GPIO_PORT_LED2, GPIO_PIN_LED2);
	}
}
