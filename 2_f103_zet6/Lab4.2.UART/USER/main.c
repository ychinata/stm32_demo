#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
/************************************************
 ALIENTEK��ӢSTM32������ʵ��4
 ���� ʵ��    
 ���ߣ�����ԭ�� @ALIENTEK
 usrt��usart��ʲô����?
 2022.2.28�߶�,���гɹ�
************************************************/
 int main(void)
 {		
 	u16 t;  
	u16 len;	
	u16 times=0;
	// ��ʼ��
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200,���Ϊ������
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	
 	while (1) {
		// ������յ�����
		if (USART_RX_STA & 0x8000) { // ����״̬���					   
			len = USART_RX_STA & 0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
			for (t = 0; t < len; t++) {
				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
				while (USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA = 0;
		// ���û���յ�����
		} else {
			times++;
			if (times % 5000 == 0) {
				printf("\r\n��ӢSTM32������ ����ʵ��\r\n");
				printf("����ԭ��@ALIENTEK\r\n\r\n");
			}
			if (times % 200 == 0) {
				printf("����������,�Իس�������\r\n");  
			}
			if (times % 30==0) {
				LED0 =! LED0;//��˸LED,��ʾϵͳ��������.
			}
			delay_ms(10);   
		}
	}	 
 }

