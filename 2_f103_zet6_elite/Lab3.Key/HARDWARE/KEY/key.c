/************************************************
Date: 2022.2.11
Author: h00421956
Func: KEY
************************************************/

#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"

void KEY_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE); //ʹ��PORTA,PORTEʱ��
	
	// KEY0&1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;
	// GPIO_InitStructure.GPIO_Speed ���벻��Ҫ���ã�
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	// KEY_UP
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	// �������룬Ϊʲô�����治һ������Ƶû�н���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	// GPIO_InitStructure.GPIO_Speed ���벻��Ҫ���ã�
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

u8 KEY_Scan(u8 mode) {
	static u8 key_up = 1;
	if (mode == KEY_MODE_SPT_CONTI) {
		key_up = 1;
	}
	if (key_up == 1 && (KEY0 == 0 || KEY1 == 0 || WK_UP == 1)) {
		delay_ms(10);
		key_up = 0;
		if (KEY0 == 0) {
			return KEY0_PRES;
		} else if (KEY1 == 0) {
			return KEY1_PRES;
		} else if (WK_UP == 1) {
			return WKUP_PRES;
		}
	} else if (KEY0 == 1 && KEY1 == 1 && WK_UP == 0) {
		key_up = 1;
	}
	return 0; // �ް�������
}
