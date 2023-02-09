#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "Delay.h"
#include "Key.h"
#include "LED.h"
#include "OLED.h"
#include "PWM.h"

#define PWM_DAC_LEVEL 256   // 2^8=256,8位精度
#define ADDA_VOL_MAX  3.3

void ShowAddaValue(void);
void SetPwmValue(u8 *keyOut, u16 *pwmValOut);


/*********************
 * Func.: main4: 秒表
 * Author:江科大自化协
 * Date:2023.2.9
 *********************/ 
int main(void)
{
	OLED_Init();
    // (72M/7200)/10000=1Hz,即1s
	TIM3_TimerInit(10000 - 1, 7200 - 1);    
	
    OLED_ShowString(1, 1, "TIM3");
	OLED_ShowString(2, 1, "Num:");
	
	while (1) {
		OLED_ShowNum(2, 5, g_Num, 5);
	}
}


/*********************
 * Func.: 定时器控制PWM实现呼吸灯
 * Author:江科大自化协
 * Date:2023.2.6
 *********************/ 
int main1(void)
{
    uint8_t i;
	OLED_Init();

    TIM2_PwmInit(100-1, 720-1); //为什么是720?
    
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
 * Func.: 定时器PWM实现DAC
 * Author:正点原子,xy
 * Date:2023.2.6
    DAC输出PA0, 环回PA2输入ADC, 并显示两个值
 *********************/ 
int main2(void)
{	 
 	u16 t=0;	 
	u8 key = KEY0_PRES;
    u16 pwmVal;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    
    //初始化
	Key_Init();				 //KEY初始化
 	LED_Init();			     //LED端口初始化
	OLED_Init();			 //LCD初始化
    AD_Init();		  		 //ADC初始化
	TIM2_PwmInit(PWM_DAC_LEVEL-1,0);	//TIM1 PWM初始化, 8位,Fpwm=72M/256=281.25Khz.
    //TIM2_PWM_Init(100-1, 720-1);
    PWM_SetCompare1(100);   //设定初始值

    //显示提示信息	
    OLED_ShowString(1,1,"PWM DAC TEST");     
	OLED_ShowString(2,1,"PWM VAL:");	      
	OLED_ShowString(3,1,"DAC VOL:0.000V");	      
	OLED_ShowString(4,1,"ADC VOL:0.000V");
	    	      
	while(1) {
		t++;
        SetPwmValue(&key, &pwmVal);
        //WKUP/KEY1按下了,或者定时时间到了		
		if(t==10||key==KEY1_PRES||key==KEY2_PRES) { 		
            ShowAddaValue();
            t = 0;
		}	           
		Delay_ms(10);	
    }
}

/*********************
 * Func.: main3.定时器PWM实现DAC,呼吸灯显示
 * Author:xy
 * Date:2023.2.6
    1.PWM值(即模拟DAC)由0上升到最大，再下降到最小.ADC的值跟随DAC值的变化
    2.DAC输出PA0, 环回PA2输入ADC, 并显示两个值
 *********************/ 
int main3(void)
{	 
 	u16 i =0;	 
    u16 pwmValueMax = 255;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    
    //初始化
 	LED_Init();			     //LED端口初始化
	OLED_Init();			 //LCD初始化
    AD_Init();		  		 //ADC初始化
	TIM2_PwmInit(PWM_DAC_LEVEL-1,0);	//TIM1 PWM初始化, 8位,Fpwm=72M/256=281.25Khz.
    //TIM2_PWM_Init(100-1, 720-1);
    PWM_SetCompare1(100);   //设定初始值

    //显示提示信息	
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
 * Func.: 设置pwm值
 * Author:正点原子,xy
 * Date:2023.2.6 待调试验证功能
 *********************/ 
void SetPwmValue(u8 *keyOut, u16 *pwmValOut)
{
	u16 pwmVal = *pwmValOut;         //初始值
    u8 key;
    
    key = Key_GetNum();			 
    *keyOut = key;
    if (key == KEY2_PRES) { // WKUP_PRES->KEY2_PRES
        if(pwmVal < 250)
            pwmVal += 10;
        PWM_SetCompare1(pwmVal); 		//输出
    }else if (key == KEY1_PRES){
        if(pwmVal>10)
            pwmVal -= 10;
        else 
            pwmVal = 0;                 // 0,修改为100
        PWM_SetCompare1(pwmVal); 		//输出
    }
    *pwmValOut = pwmVal;
}

// 显示ADC,DAC的电压值
void ShowAddaValue(void)
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
    LED1_Turn();	           
}
