#include "ENCODER.h"
#include "oled.h"
#include "usart.h"   				
#include "delay.h"

int main(void)
{
	delay_init();
	uart_init(9600);
	Encoder_Init_TIM4(0xffff,0);
    OLED_Init();
    
	OLED_ShowString(1, 1, "Encoder:");
	
    
    while(1) {		 
		delay_ms(200); //每隔200ms读取一次编码器计数，即速度。
		               //可以使用定时中断实现更精准的速度计算，用户可自定义
        printf("Encoder=%d\r\n", Read_Encoder_TIM4());
        OLED_ShowNum(2, 1, Read_Encoder_TIM4(), 5);
	}
}
