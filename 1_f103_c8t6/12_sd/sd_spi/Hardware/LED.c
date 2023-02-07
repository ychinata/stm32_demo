#include "stm32f10x.h"                  // Device header

#define LED_1_PIN GPIO_Pin_8
#define LED_2_PIN GPIO_Pin_2

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED_1_PIN | LED_2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, LED_1_PIN | LED_2_PIN);
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, LED_1_PIN);
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, LED_1_PIN);
}

void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, LED_1_PIN) == 0) {
		GPIO_SetBits(GPIOA, LED_1_PIN);
	} else {
		GPIO_ResetBits(GPIOA, LED_1_PIN);
	}
}

void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA, LED_2_PIN);
}

void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA, LED_2_PIN);
}

void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, LED_2_PIN) == 0) {
		GPIO_SetBits(GPIOA, LED_2_PIN);
	} else {
		GPIO_ResetBits(GPIOA, LED_2_PIN);
	}
}
