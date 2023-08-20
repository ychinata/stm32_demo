#include "led.h"
#include "key.h"
#include "beep.h"
#include "sys.h"
#include "delay.h"
#include "iwdg.h"
 
/************************************************
Date: 2022.3.1
Author: h00421956
Func: �������Ź�
		ÿ�����ٰ�һ��key_up��������ͻ�һֱ����
		��λ����һ�¾���ιһ�ι�
************************************************/

int main(void)
{	
	delay_init();
	LED_Init();
	BEEP_Init();
	KEY_Init();
	
	delay_ms(200);
	LED0 = 0;
	
	// ���ʱ�����M3
	// T_out_ms = ((4*2^prer)*rlr)/40
	IWDG_Init(4, 625); // 1��
	
	while (1) {
		if (KEY_Scan(0) == WKUP_PRES) {
			IWDG_Feed(); // û�а����Ͳ�ι��
		}
	}
}
