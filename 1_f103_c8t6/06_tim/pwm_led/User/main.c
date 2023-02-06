#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"

uint8_t i;

/*********************
 * Func.: ��ʱ������PWMʵ�ֺ�����
 * Author:���ƴ��Ի�Э
 * Date:2023.2.6
 *********************/ 
int main(void)
{
	OLED_Init();
	PWM_Init();
	
    OLED_ShowString(1, 1, "TIM:PWM LED");
    
	while (1) {
		for (i = 0; i <= 100; i++) {
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
		for (i = 0; i <= 100; i++) {
			PWM_SetCompare1(100 - i);
			Delay_ms(10);
		}
	}
}
