#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "malloc.h"  
#include "mmc_sd.h"   
#include "OLED.h"
#include "sys.h"

/********************��������*****************
 * OLED(4��)-oled.c
    SCL-PB8
    SDA-PB9

 * SD����    
    PA3 -- SD_CS    (mmc_sd.h)���޸�
    PA5 -- SPI1_SCK (SPI1_Init)
    PA6 -- SPI1_MISO(SPI1_Init)
    PA7 -- SPI1_MOSI(SPI1_Init)

**********************************************/

//��ȡSD����ָ�����������ݣ���ͨ������1���
//sec�����������ַ���
void SD_Read_Sectorx(u32 sec)
{
	u8 *buf;
	u16 i;
	buf = mymalloc(512);				//�����ڴ�
	if(SD_ReadDisk(buf,sec,1)==0)	//��ȡ0����������
	{	
		//LCD_ShowString(60,190,200,16,16,"USART1 Sending Data...");
		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)
            printf("%x ",buf[i]);//��ӡsec��������    	   
		printf("\r\nDATA ENDED\r\n");
		//LCD_ShowString(60,190,200,16,16,"USART1 Send Data Over!");
	}
	myfree(buf);//�ͷ��ڴ�	
}
	

/*********************
 * Func.: Mini STM32�����巶������28 SD��ʵ��
 * Author:����ԭ��,xy
 * Date:2023.2.7
    -
 *********************/ 
int main(void)
{ 
	u8 key;		 
	u32 sd_size;
	u8 t=0;	 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2	  
	//uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600,���ڵ��Դ�ӡ
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	OLED_Init();			   	//��ʼ��LCD	
 	KEY_Init();				//������ʼ��  
 	mem_init();				//��ʼ���ڴ��	 
    

	//LCD_ShowString(60,50,200,16,16,"Mini STM32");	
	//LCD_ShowString(60,70,200,16,16,"SD CARD TEST");	
	//LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	//LCD_ShowString(60,110,200,16,16,"2014/3/13");  
	//LCD_ShowString(60,130,200,16,16,"KEY0:Read Sector 0");	  
    
    //��ⲻ��SD��
 	while(SD_Initialize()) {
		//LCD_ShowString(60,150,200,16,16,"SD Card Error!");
		Delay_ms(500);					
		//LCD_ShowString(60,150,200,16,16,"Please Check! ");
		Delay_ms(500);
		LED1_Turn();//DS0��˸
	}

	//���SD���ɹ� 											    
	//LCD_ShowString(60,150,200,16,16,"SD Card OK    ");
	//LCD_ShowString(60,170,200,16,16,"SD Card Size:     MB");
	sd_size=SD_GetSectorCount();//�õ�������
	//LCD_ShowNum(164,170,sd_size>>11,5,16);//��ʾSD������
	while(1) {
		key = KEY_GetNum();
		if(key == KEY0_PRES)
            SD_Read_Sectorx(0);//KEY0��,��ȡSD������0������
		t++;
		Delay_ms(10);
		if (t==20) {
			LED1_Turn();
			t=0;
		}
	}    	   
}



// ADC��ͨ���ɼ�
// 2023.1.30
int main1(void)
{
    uint16_t ADValue;
    float voltage;
    
	OLED_Init();
	AD_Init();
	OLED_ShowString(1, 1, "ADC Single Ch:");	
	OLED_ShowString(2, 1, "ADValue:");
	OLED_ShowString(3, 1, "Volatge:0.00V");
	
	while (1) {
		ADValue = AD_GetValue(ADC_Channel_0);
		voltage = (float)ADValue / 4095 * 3.3;
		
		OLED_ShowNum(2, 9, ADValue, 4);
		OLED_ShowNum(3, 9, voltage, 1);     // ��ʾ��ѹ��������
		OLED_ShowNum(3, 11, (uint16_t)(voltage * 100) % 100, 2);    // ��ʾ��ѹС������
		
		Delay_ms(100);
	}
}

