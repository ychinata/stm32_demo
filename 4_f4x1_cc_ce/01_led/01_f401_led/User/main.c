#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"

int main(void)
{  
    delay_init();
    LED_Init();

    while (1) {
        //拉高灭，拉低亮，硬件决定的
        LED0=1;
        delay_ms(1000);
        LED0=0;
        delay_ms(1000);
    }
}


