#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"

int main(void)
{  
    delay_init();
    LED_Init();

    while (1) {
        //��������������Ӳ��������
        LED0=1;
        delay_ms(1000);
        LED0=0;
        delay_ms(1000);
    }
}


