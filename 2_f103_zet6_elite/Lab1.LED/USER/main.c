#include "stm32f10x.h"
#include "led.h"
#include "delay.h"

/************************************************
Date: 2022.2.10
Author: h00421956
Func: LED
************************************************/

int main(void) {
	delay_init();
	LED_Init();
	
	while (1) {
		GPIO_SetBits(GPIOB, GPIO_Pin_5); // 输出高电平
		GPIO_SetBits(GPIOE, GPIO_Pin_5); // 输出高电平
		delay_ms(500);
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_5); // 输出低电平
		GPIO_ResetBits(GPIOE, GPIO_Pin_5); // 输出低电平
		delay_ms(500);
	}
}
