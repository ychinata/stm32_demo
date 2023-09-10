#include "ENCODER.h"
#include "oled.h"
#include "usart.h"   				
#include "delay.h"

int main(void)
{
	delay_init();
	uart_init(9600);
	ENCODER_Init();
    OLED_Init();
    
	OLED_ShowString(1, 1, "Encoder Left:");
	OLED_ShowString(3, 1, "Encoder Right:");
    
    while(1) {		 
		delay_ms(200); //ÿ��200ms��ȡһ�α��������������ٶȡ�
		               //����ʹ�ö�ʱ�ж�ʵ�ָ���׼���ٶȼ��㣬�û����Զ���
        printf("Encoder=%d\r\n", ENCODER_ReadCnt(ENCODER_ID_LEFT));
        OLED_ShowNum(2, 1, ENCODER_ReadCnt(ENCODER_ID_LEFT), 5);
        OLED_ShowNum(4, 1, ENCODER_ReadCnt(ENCODER_ID_RIGHT), 5);
	}
}
