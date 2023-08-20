#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "OLED.h"

#define BUFLEN 10

void UART_Task(void);
void UART_Task_onlysend(void);

/************************************************
 ���� ʵ��    
 ���ߣ�����ԭ�� @ALIENTEK
 usrt��usart��ʲô����?
 2022.5.14
************************************************/
int main(void) {		
	
	// ��ʼ��
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200,���Ϊ������
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	OLED_Init();
	 
	OLED_ShowString(4, 1, "2022.5.14"); 
	
 	while (1) {
		//UART_Task();
		UART_Task_onlysend(); //����5�����ڻ�ˢ��
		//����6����������ʱ�ٷ��ͣ�
	}
 
}

//2022.5.14
void UART_Task_onlysend(void) {
 	u16 t;
	for (t = 0; t < BUFLEN; t++) {
		// ����4��
		USART_SendData(USART1, 0x61);//�򴮿�1��������
		
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
	printf("\r\n");//���뻻��
}

//2022.5.14
void UART_Task(void) {
 	u16 t;  
	u16 len;	
	static u16 times=0;
	static u8 send_buf[BUFLEN] = {0x61,0x63,0x65,0x67,0x69,0x6b,0x6d,0x6f,0x71,0x73};	
	
	// ������յ�����
	if (USART_RX_STA & 0x8000) { // ����״̬���					   
		len = USART_RX_STA & 0x3fff;//�õ��˴ν��յ������ݳ���
		//printf("\r\n�����͵���ϢΪ:\r\n\r\n");
		printf("\r\nsend\r\n");
		/* //����1�����Խӵ������ݳ��ȣ��Լ����ݱ���ĸ�ʽ��ΪASCII��
		OLED_ShowNum(1, 1, len, 3);
		OLED_ShowNum(2, 1, USART_RX_BUF[0], 3);//���յ��ĵ�1���ַ���ʧ�ˣ���λ���޷����յ�����λ������4���ַ���ֻ�ܷ���3���ַ�������λ
		OLED_ShowHexNum(2, 4, USART_RX_BUF[0], 2);
		OLED_ShowHexNum(3, 1, USART_RX_BUF[1], 2);
		*/
		
		//for (t = 0; t < len; t++) {
		for (t = 0; t < BUFLEN; t++) {
			// ����1��
			//USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
			// ����2��
			//USART_SendData(USART1, 0x61+t);//�򴮿�1��������
			// ����3��
			USART_SendData(USART1, send_buf[t]);//�򴮿�1��������
			
			while (USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
		printf("\r\n");//���뻻��
		USART_RX_STA = 0;
	// ���û���յ�����
	} else {
		times++;
		if (times % 5000 == 0) {
			//printf("\r\n��ӢSTM32������ ����ʵ��\r\n");
			//printf("����ԭ��@ALIENTEK\r\n\r\n");
			printf("\r\ntest\r\n");				
		}
		if (times % 200 == 0) {
			//printf("����������,�Իس�������\r\n");  
			printf("input\r\n");				
		}
		if (times % 30==0) {
			LED0 =! LED0;//��˸LED,��ʾϵͳ��������.
		}
		delay_ms(10);   
	}

}

