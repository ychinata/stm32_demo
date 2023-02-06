#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "Delay.h"
#include "Key.h"
#include "LED.h"
#include "OLED.h"
#include "PWM.h"

#define PWM_DAC_LEVEL 256   // 2^8=256,8位精度
#define ADDA_VOL_MAX  3.3

void ShowAddaValue(u16 *t);

/*********************
 * Func.: 定时器控制PWM实现呼吸灯
 * Author:江科大自化协
 * Date:2023.2.6
 *********************/ 
 uint8_t i;
int main1(void)
{
	OLED_Init();
	//PWM_Init();
    TIM2_PWM_Init(100-1, 720-1); //为什么是720?
	
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
 * Func.: 定时器PWM实现DAC
 * Author:正点原子,xy
 * Date:2023.2.6
    DAC输出PA0, 环回PA2输入ADC, 并显示两个值
 *********************/ 
int main(void)
{	 
 	u16 t=0;	 
	u16 pwmval = 100;         //初始值为0	
	u8 key = KEY0_PRES;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    
    //初始化
	Key_Init();				  //KEY初始化
 	LED_Init();			     //LED端口初始化
	OLED_Init();			 	 //LCD初始化
    AD_Init();		  		//ADC初始化
	TIM2_PWM_Init(PWM_DAC_LEVEL-1,0);	//TIM1 PWM初始化, 8位,Fpwm=72M/256=281.25Khz.
    //TIM2_PWM_Init(100-1, 720-1);
    PWM_SetCompare1(pwmval);


    //显示提示信息	
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
			PWM_SetCompare1(pwmval); 		//输出
		}else if (key == KEY1_PRES){
			if(pwmval>10)
                pwmval -= 10;
			else 
                pwmval = 0;                 // 0,修改为100
			PWM_SetCompare1(pwmval); 		//输出
		}	 
		if(t==10||key==KEY1_PRES||key==KEY2_PRES) { 		//WKUP/KEY1按下了,或者定时时间到了		
            ShowAddaValue(&t);
		}	           
		Delay_ms(10);	
    }
}

// 显示ADC,DAC的电压值
void ShowAddaValue(u16 *t)
{
	u16 adcVal = 200;
    u16 dacVal = 255;
	float temp;
    
   // 显示DAC输出电压值
    dacVal = TIM_GetCapture1(TIM2);           // 获取失败	
    OLED_ShowNum(2, 9, dacVal, 3);            // 显示DAC寄存器值
    temp = (float)dacVal*(ADDA_VOL_MAX/PWM_DAC_LEVEL);		    //得到DAC电压值
    dacVal = temp;
    OLED_ShowNum(3, 9, dacVal, 1);     	//显示电压值整数部分
    temp -= dacVal;                       // 电压小数部分
    temp *= 1000;
    OLED_ShowNum(3, 11, temp, 3); 	    //显示电压值的小数部分
    
    // 显示ADC输入电压值     
    adcVal = AD_GetAverageValue(ADC_Channel_2, 20);     //得到ADC转换平均值,如果是瞬时值,会出现3.3V和0V一直跳动
    temp = (float)adcVal*(ADDA_VOL_MAX/4096);			//得到ADC电压值
    adcVal = temp;
    OLED_ShowNum(4, 9, temp, 1);     	//显示电压值整数部分
    temp -= adcVal;
    temp *= 1000;
    OLED_ShowNum(4, 11, temp, 3); 	//显示电压值的小数部分
    *t = 0;
    LED1_Turn();	           
}
