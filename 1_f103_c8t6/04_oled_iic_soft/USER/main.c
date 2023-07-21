#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "oled_soft.h"
#include "led.h"
#include "sys.h"

int main(void)
{
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ� 
    
	OLED_Init();
	
//	OLED_ShowChar(1, 1, 'A');
//    OLED_ShowChar(0,0, 'A',OLED_FONTSIZE);  //��һ��
//    OLED_ShowChar(0,16,'B',OLED_FONTSIZE);  //�ڶ���
//    OLED_ShowChar(0,32,'C',OLED_FONTSIZE);  //������
    OLED_ShowString(0,0,(u8 *)"ABCDEFGHIJKLMNO",OLED_FONTSIZE);  //������
    OLED_ShowString(0,16,(u8 *)"ABCDEFGHIJKLMNO",OLED_FONTSIZE);  //������
    OLED_ShowString(0,32,(u8 *)"IIC SOFT",OLED_FONTSIZE);  //������
    OLED_ShowString(0,48,(u8 *)"2023.7.21",OLED_FONTSIZE);  //������
    OLED_Refresh_Gram();
    
	
	while(1) {
		LED0=0;
		LED1=1;
        //OLED_ShowString(2, 1, "123456--");  
		delay_ms(300);	 //��ʱ300ms
        
		//LED0=1;
		//LED1=0;
        //OLED_ShowString(2, 1, "->abcdef");
		delay_ms(300);	//��ʱ300ms
	}
    
}
