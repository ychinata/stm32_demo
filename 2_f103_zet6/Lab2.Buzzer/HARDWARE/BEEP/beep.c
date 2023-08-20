// 2022.2.10
// h00421956

#include "beep.h"
#include "stm32f10x.h"

void BEEP_Init(void) {
	
	GPIO_InitTypeDef GPIO_InitSturcture;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // GPIOB
	
	// BEEP PB8
	GPIO_InitSturcture.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitSturcture);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_8); // ¹Ø±Õ·äÃùÆ÷Êä³ö

}
