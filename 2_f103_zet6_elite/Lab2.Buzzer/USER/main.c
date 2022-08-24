#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"



/************************************************
Date: 2022.2.10
Author: h00421956
Func: LED
************************************************/

int main(void) {

	delay_init();
	LED_Init();
	BEEP_Init();
	while (1) {
		LED0 = 0; // 位操作
		BEEP = 0; // 位操作
		delay_ms(300);
		LED0 = 1;
		BEEP = 1;
		delay_ms(300);
	}
}
