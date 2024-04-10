#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNum;

//°´¼ü¿ØÖÆLED 
int main1(void)
{
	LED_Init();
	Key_Init();
	
	while (1) {
		KeyNum = Key_GetNum();
		if (KeyNum == 1) {
			LED1_Turn();
		}
		if (KeyNum == 2) {
			LED2_Turn();
		}
	}
}

//LEDÑ­»·ÁÁÃð
int main(void)
{
	LED_Init();
	Key_Init();
    
    LED1_ON();
    LED2_ON();
    	
	while (1) {
        LED1_Turn();
        LED2_Turn();
        Delay_ms(500);      
	}
}


