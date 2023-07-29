#include "led.h"
#include "delay.h"
#include "oled.h"
#include "sys.h"


/* ���ŷ��䣺

    LED: led.h
    OLED: oled.h
    
 */

int main(void)
    {	
    // ģ���ʼ��bgn
    delay_init();	    	 //��ʱ������ʼ��	  
    LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
    OLED_Init();
    // ģ���ʼ��end
        
    OLED_ShowString(1, 1, "Hello,F103RCT6!");
    OLED_ShowString(3, 1, "2023.7.27");         
        
    while(1)
    {
        LED0=0;
        LED1=1;
        delay_ms(300);	 //��ʱ300ms
        LED0=1;
        LED1=0;
        delay_ms(300);	//��ʱ300ms
    }
}

