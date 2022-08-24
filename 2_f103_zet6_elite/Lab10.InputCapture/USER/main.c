#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

/************************************************
 ALIENTEK��ӢSTM32������ʵ��10
 ���벶��ʵ��  
 ���ߣ�����ԭ�� @ALIENTEK
 2022.3.14
 Func������key_up�����Ჶ���µ�ʱ�䣬��ͨ�����ڷ��͵�����
************************************************/

extern u8  TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ	
int main(void) {		
 	u32 temp = 0; 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
 
 	TIM3_PWM_Init(899,0); 		//����Ƶ��PWMƵ��=72000/(899+1)=80Khz
 	TIM5_Cap_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
   	while(1) {
 		delay_ms(10);
		TIM_SetCompare2(TIM3,TIM_GetCapture2(TIM3)+1);
	
		if (TIM_GetCapture2(TIM3)==300) {
			TIM_SetCompare2(TIM3,0);	
		}
		 		 
 		if (TIM5CH1_CAPTURE_STA & 0X80) { //�ɹ�������һ��������
			temp = TIM5CH1_CAPTURE_STA & 0X3F;
			temp *= 65536;//���ʱ���ܺ�
			temp += TIM5CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%d us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH1_CAPTURE_STA = 0;//������һ�β���
		}
	}
}

