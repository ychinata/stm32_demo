// 2022.2.10
// h00421956

#include "led.h"
#include "stm32f10x.h"

void LED_Init(void) {

	GPIO_InitTypeDef GPIO_InitSturcture; 
	// DS0 PB5 (RED), DS1 PE5 (GREEN)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); // GPIOE
	
	// GPIOB
	GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitSturcture.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitSturcture);
	GPIO_SetBits(GPIOB, GPIO_Pin_5); // 输出高电平
	// GPIOE
	GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitSturcture.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitSturcture);
	GPIO_SetBits(GPIOE, GPIO_Pin_5); // 输出高电平
}
