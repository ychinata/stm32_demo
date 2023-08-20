#include "led.h"
#include "key.h"
#include "beep.h"
#include "sys.h"
#include "delay.h"
 
/************************************************
Date: 2022.2.11
Author: h00421956
Func: KEY
KEY_UP¿ØÖÆ·äÃùÆ÷
KEY1¿ØÖÆÂÌµÆ
KEY0¿ØÖÆºìÂÌµÆ
************************************************/

int main(void)
{	
	u8 key = 0;
	delay_init();
	LED_Init();
	BEEP_Init();
	KEY_Init();
	LED0 = 0;
	while (1) {
		key = KEY_Scan(KEY_MODE_NOT_SPT_CONTI); // KEY_MODE_SPT_CONTI
		if (key != 0) {
			switch (key) {
				case WKUP_PRES: {
					BEEP = !BEEP;
					break;
				}
				case KEY1_PRES: {
					LED1 = !LED1;
					break;
				}
				case KEY0_PRES: {
					LED0 = !LED0;
					LED1 = !LED1;
					break;
				}
			}
		} else {
			delay_ms(10);
		}
	}
	
}
