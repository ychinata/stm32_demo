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
		GPIO_SetBits(GPIOB, GPIO_Pin_5); // ����ߵ�ƽ
		GPIO_SetBits(GPIOE, GPIO_Pin_5); // ����ߵ�ƽ
		delay_ms(500);
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_5); // ����͵�ƽ
		GPIO_ResetBits(GPIOE, GPIO_Pin_5); // ����͵�ƽ
		delay_ms(500);
	}
}
