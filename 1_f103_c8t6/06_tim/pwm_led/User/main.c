#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "Delay.h"
#include "Key.h"
#include "LED.h"
#include "OLED.h"
#include "PWM.h"

#define PWM_DAC_LEVEL 256   // 2^8=256,8λ����
#define ADDA_VOL_MAX  3.3

void ShowAddaValue(u16 *t);

/*********************
 * Func.: ��ʱ������PWMʵ�ֺ�����
 * Author:���ƴ��Ի�Э
 * Date:2023.2.6
 *********************/ 
 uint8_t i;
int main1(void)
{
	OLED_Init();
	//PWM_Init();
    TIM2_PWM_Init(100-1, 720-1); //Ϊʲô��720?
	
    OLED_ShowString(1, 1, "TIM:PWM LED");
    
	while (1) {
		for (i = 0; i <= 100; i++) {
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
		for (i = 0; i <= 100; i++) {
			PWM_SetCompare1(100 - i);
			Delay_ms(10);
		}
	}
}

/*********************
 * Func.: ��ʱ��PWMʵ��DAC
 * Author:����ԭ��,xy
 * Date:2023.2.6
    DAC���PA0, ����PA2����ADC, ����ʾ����ֵ
 *********************/ 
int main(void)
{	 
 	u16 t=0;	 
	u16 pwmval = 100;         //��ʼֵΪ0	
	u8 key = KEY0_PRES;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    
    //��ʼ��
	Key_Init();				  //KEY��ʼ��
 	LED_Init();			     //LED�˿ڳ�ʼ��
	OLED_Init();			 	 //LCD��ʼ��
    AD_Init();		  		//ADC��ʼ��
	TIM2_PWM_Init(PWM_DAC_LEVEL-1,0);	//TIM1 PWM��ʼ��, 8λ,Fpwm=72M/256=281.25Khz.
    //TIM2_PWM_Init(100-1, 720-1);
    PWM_SetCompare1(pwmval);


    //��ʾ��ʾ��Ϣ	
    OLED_ShowString(1,1,"PWM DAC TEST");     
	OLED_ShowString(2,1,"PWM VAL:");	      
	OLED_ShowString(3,1,"DAC VOL:0.000V");	      
	OLED_ShowString(4,1,"ADC VOL:0.000V");
	    	      
	while(1) {
		t++;
		key = Key_GetNum();			  
		if (key == KEY2_PRES) { // WKUP_PRES->KEY2_PRES
			if(pwmval < 250)
                pwmval += 10;
			PWM_SetCompare1(pwmval); 		//���
		}else if (key == KEY1_PRES){
			if(pwmval>10)
                pwmval -= 10;
			else 
                pwmval = 0;                 // 0,�޸�Ϊ100
			PWM_SetCompare1(pwmval); 		//���
		}	 
		if(t==10||key==KEY1_PRES||key==KEY2_PRES) { 		//WKUP/KEY1������,���߶�ʱʱ�䵽��		
            ShowAddaValue(&t);
		}	           
		Delay_ms(10);	
    }
}

// ��ʾADC,DAC�ĵ�ѹֵ
void ShowAddaValue(u16 *t)
{
	u16 adcVal = 200;
    u16 dacVal = 255;
	float temp;
    
   // ��ʾDAC�����ѹֵ
    dacVal = TIM_GetCapture1(TIM2);           // ��ȡʧ��	
    OLED_ShowNum(2, 9, dacVal, 3);            // ��ʾDAC�Ĵ���ֵ
    temp = (float)dacVal*(ADDA_VOL_MAX/PWM_DAC_LEVEL);		    //�õ�DAC��ѹֵ
    dacVal = temp;
    OLED_ShowNum(3, 9, dacVal, 1);     	//��ʾ��ѹֵ��������
    temp -= dacVal;                       // ��ѹС������
    temp *= 1000;
    OLED_ShowNum(3, 11, temp, 3); 	    //��ʾ��ѹֵ��С������
    
    // ��ʾADC�����ѹֵ     
    adcVal = AD_GetAverageValue(ADC_Channel_2, 20);     //�õ�ADCת��ƽ��ֵ,�����˲ʱֵ,�����3.3V��0Vһֱ����
    temp = (float)adcVal*(ADDA_VOL_MAX/4096);			//�õ�ADC��ѹֵ
    adcVal = temp;
    OLED_ShowNum(4, 9, temp, 1);     	//��ʾ��ѹֵ��������
    temp -= adcVal;
    temp *= 1000;
    OLED_ShowNum(4, 11, temp, 3); 	//��ʾ��ѹֵ��С������
    *t = 0;
    LED1_Turn();	           
}
