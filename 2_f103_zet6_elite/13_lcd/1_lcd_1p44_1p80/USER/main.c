#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h" 
 
#include "lcd.h"
#include "Dis_Picture.h" 
#include "Text.h"	
#include "GBK_LibDrive.h"	 
 
 
/************************************************/
//DevEBox  ��Խ���� mcudev.taobao.com
//STM32F103 ������  1.44��Һ���� SPI �ӿ�   ʵ��

/* LED */
// LED_D0 -- PB9     --�͵�ƽ����      
// LED_D1 -- PE5	 --�͵�ƽ����    

/* KEY */
// WKUP -- PA0        WKUP ���� --���º󣬸ߵ�ƽ  
// K0   -- PE4        K0   ���� --���º󣬵͵�ƽ    

/* USART0����ͨ�� */
//USART0_TX---PA9     --��Ƭ���������
//USART0_RX---PA10    --��Ƭ����������

/* SPI TFT LCD�� */
//LCD_SDA        	//PB15      //����������
//LCD_SCL        	//PB13      //ʱ����
//LCD_CS        	//PB12      //Ƭѡ	
//LCD_SDO/RST     //PB14      //�������/��λ
//LCD_DC         	//PC5       //����/�����л�
//LCD_BLK         //PB1    	  //�������
/************************************************/

uint16_t t;  
uint16_t len;	
uint16_t times=0;
		
int main(void)
{	    	
	delay_init();	    //SysTick ��ʱ������ʼ��		 
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�	 
	KEY_Init();       //��ʼ������ --ģʽ��IO����ģʽ  
	USART1_init(115200);          //����1     ��ʼ������ 	 
	LCD_Init();           //��ʼ��LCD 1.44��Һ������ʼ��  SPI �ӿ� 	 
	GBK_Lib_Init();       //Ӳ��GBK�ֿ��ʼ��--(���ʹ�ò����ֿ��Һ�����汾���˴��������Σ������ֿ��ʼ����
	 
	while(1) {
		
        LCD_DemoMenu();//��ʾ����		
		
        if(KEY0_PRES == KEY_Scan(1)) {
//  	    LED0=0;
            LED_D0_toggle;//��˸LED,��ʾϵͳ��������
            delay_ms(500);
//			LED0=1;
        }
		
		if (USART_RX_STA&0x8000) {					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���			
			printf("\r\n�����͵���ϢΪ:\r\n\r\n");			
			for(t=0;t<len;t++) {				
				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�0  ��������				
				while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));//�ȴ����ͽ���				
			}
			printf("\r\n\r\n");//���뻻��			
			USART_RX_STA=0;			
		} else {
			times++;			
			if(times%500==0) {
				printf("\r\n ST32F103 ������ ����1 ����\r\n");
				printf("mcudev.taobao.com\r\n\r\n");
			}			
			if(times%200==0)
                printf("����������,�Իس�������\n");			
			if(times%30==0)
                LED_D1_toggle;//��˸LED,��ʾϵͳ��������			
			delay_ms(10);   
		}				
	}
 }



	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
/************************************************
	
//STM32F103����ģ��-�⺯���汾
//DevEBox  ��Խ����

//�Ա����̣�mcudev.taobao.com
//�Ա����̣�devebox.taobao.com	

************************************************/

