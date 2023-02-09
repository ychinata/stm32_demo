#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "Delay.h"
#include "Key.h"
#include "LED.h"
#include "OLED.h"
#include "PWM.h"

#define PWM_DAC_LEVEL 256   // 2^8=256,8λ����
#define ADDA_VOL_MAX  3.3

void ShowAddaValue(void);
void SetPwmValue(u8 *keyOut, u16 *pwmValOut);


/*********************
 * Func.: main4: ���
 * Author:���ƴ��Ի�Э
 * Date:2023.2.9
 *********************/ 
int main(void)
{
	OLED_Init();
    // (72M/7200)/10000=1Hz,��1s
	TIM3_TimerInit(10000 - 1, 7200 - 1);    
	
    OLED_ShowString(1, 1, "TIM3");
	OLED_ShowString(2, 1, "Num:");
	
	while (1) {
		OLED_ShowNum(2, 5, g_Num, 5);
	}
}


/*********************
 * Func.: ��ʱ������PWMʵ�ֺ�����
 * Author:���ƴ��Ի�Э
 * Date:2023.2.6
 *********************/ 
int main1(void)
{
    uint8_t i;
	OLED_Init();

    TIM2_PwmInit(100-1, 720-1); //Ϊʲô��720?
    
    //TIM2_PWM_Init(PWM_DAC_LEVEL-1,0);
	
    OLED_ShowString(1, 1, "TIM:PWM LED");
    
	while (1) {
		for (i = 0; i <= 100; i++) {
			PWM_SetCompare1(i);
            //ShowAddaValue();
			Delay_ms(10);
		}
		for (i = 0; i <= 100; i++) {
			PWM_SetCompare1(100 - i);
            //ShowAddaValue();
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
int main2(void)
{	 
 	u16 t=0;	 
	u8 key = KEY0_PRES;
    u16 pwmVal;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    
    //��ʼ��
	Key_Init();				 //KEY��ʼ��
 	LED_Init();			     //LED�˿ڳ�ʼ��
	OLED_Init();			 //LCD��ʼ��
    AD_Init();		  		 //ADC��ʼ��
	TIM2_PwmInit(PWM_DAC_LEVEL-1,0);	//TIM1 PWM��ʼ��, 8λ,Fpwm=72M/256=281.25Khz.
    //TIM2_PWM_Init(100-1, 720-1);
    PWM_SetCompare1(100);   //�趨��ʼֵ

    //��ʾ��ʾ��Ϣ	
    OLED_ShowString(1,1,"PWM DAC TEST");     
	OLED_ShowString(2,1,"PWM VAL:");	      
	OLED_ShowString(3,1,"DAC VOL:0.000V");	      
	OLED_ShowString(4,1,"ADC VOL:0.000V");
	    	      
	while(1) {
		t++;
        SetPwmValue(&key, &pwmVal);
        //WKUP/KEY1������,���߶�ʱʱ�䵽��		
		if(t==10||key==KEY1_PRES||key==KEY2_PRES) { 		
            ShowAddaValue();
            t = 0;
		}	           
		Delay_ms(10);	
    }
}

/*********************
 * Func.: main3.��ʱ��PWMʵ��DAC,��������ʾ
 * Author:xy
 * Date:2023.2.6
    1.PWMֵ(��ģ��DAC)��0������������½�����С.ADC��ֵ����DACֵ�ı仯
    2.DAC���PA0, ����PA2����ADC, ����ʾ����ֵ
 *********************/ 
int main3(void)
{	 
 	u16 i =0;	 
    u16 pwmValueMax = 255;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    
    //��ʼ��
 	LED_Init();			     //LED�˿ڳ�ʼ��
	OLED_Init();			 //LCD��ʼ��
    AD_Init();		  		 //ADC��ʼ��
	TIM2_PwmInit(PWM_DAC_LEVEL-1,0);	//TIM1 PWM��ʼ��, 8λ,Fpwm=72M/256=281.25Khz.
    //TIM2_PWM_Init(100-1, 720-1);
    PWM_SetCompare1(100);   //�趨��ʼֵ

    //��ʾ��ʾ��Ϣ	
    OLED_ShowString(1,1,"PWM DAC TEST");     
	OLED_ShowString(2,1,"PWM VAL:");	      
	OLED_ShowString(3,1,"DAC VOL:0.000V");	      
	OLED_ShowString(4,1,"ADC VOL:0.000V");
	    	      
	while (1) {
		for (i = 0; i <= pwmValueMax; i++) {
			PWM_SetCompare1(i);
            ShowAddaValue();
			Delay_ms(10);
		}
		for (i = 0; i <= pwmValueMax; i++) {
			PWM_SetCompare1(pwmValueMax - i);
            ShowAddaValue();
			Delay_ms(10);
		}
	}    
}

/*********************
 * Func.: ����pwmֵ
 * Author:����ԭ��,xy
 * Date:2023.2.6 ��������֤����
 *********************/ 
void SetPwmValue(u8 *keyOut, u16 *pwmValOut)
{
	u16 pwmVal = *pwmValOut;         //��ʼֵ
    u8 key;
    
    key = Key_GetNum();			 
    *keyOut = key;
    if (key == KEY2_PRES) { // WKUP_PRES->KEY2_PRES
        if(pwmVal < 250)
            pwmVal += 10;
        PWM_SetCompare1(pwmVal); 		//���
    }else if (key == KEY1_PRES){
        if(pwmVal>10)
            pwmVal -= 10;
        else 
            pwmVal = 0;                 // 0,�޸�Ϊ100
        PWM_SetCompare1(pwmVal); 		//���
    }
    *pwmValOut = pwmVal;
}

// ��ʾADC,DAC�ĵ�ѹֵ
void ShowAddaValue(void)
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
    LED1_Turn();	           
}
