#include "led.h"
#include "key.h"
#include "beep.h"
#include "sys.h"
#include "delay.h"
#include "iwdg.h"
 
/************************************************
Date: 2022.3.1
Author: h00421956
Func: 独立看门狗
		每秒至少按一个key_up键，否则就会一直饿狗
		复位，按一下就是喂一次狗
************************************************/

int main(void)
{	
	delay_init();
	LED_Init();
	BEEP_Init();
	KEY_Init();
	
	delay_ms(200);
	LED0 = 0;
	
	// 溢出时间计算M3
	// T_out_ms = ((4*2^prer)*rlr)/40
	IWDG_Init(4, 625); // 1秒
	
	while (1) {
		if (KEY_Scan(0) == WKUP_PRES) {
			IWDG_Feed(); // 没有按，就不喂狗
		}
	}
}
