#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "rtc.h" 
#include "wkup.h"
#include "adc.h"	 
#include "dac.h"
#include "timer.h"
#include "usmart.h"

 
uint8_t i; 
 
/************************************************
 ALIENTEK战舰STM32开发板实验21
 PWM DAC 实验  
 作者：正点原子 @ALIENTEK
************************************************/

//设置输出电压
//vol:0~330,代表0~3.3V
void PWM_DAC_Set(u16 vol)
{
	float temp=vol;
	temp/=100;
	temp=temp*256/3.3;
	TIM_SetCompare1(TIM1,temp);
}

 int main1(void)
 {	 
	u16 adcx;
	float temp;
 	u8 t=0;	 
	u16 pwmval=0;
	u8 key;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	KEY_Init();				  //KEY初始化
 	LED_Init();			     //LED端口初始化
	LCD_Init();			 	 //LCD初始化
    Adc_Init();		  		//ADC初始化
	TIM1_PWM_Init(255,0);	//TIM1 PWM初始化, 8位,Fpwm=72M/256=281.25Khz.
    TIM_SetCompare1(TIM1,100);//初始值为0	
	     

    //显示提示信息	
	LCD_ShowString(60,70,200,16,16,"PWM DAC TEST");											     
	LCD_ShowString(60,150,200,16,16,"PWM VAL:");	      
	LCD_ShowString(60,170,200,16,16,"DAC VOL:0.000V");	      
	LCD_ShowString(60,190,200,16,16,"ADC VOL:0.000V");
	
	TIM_SetCompare1(TIM1,pwmval);//初始值	    	      
	while(1) {
		t++;
		key=KEY_Scan(0);			  
		if(key==WKUP_PRES) {		 
			if(pwmval<250)pwmval+=10;
			TIM_SetCompare1(TIM1,pwmval); 		//输出
		}else if(key==KEY1_PRES){
			if(pwmval>10)pwmval-=10;
			else pwmval=0;
			TIM_SetCompare1(TIM1,pwmval); 		//输出
		}	 
		if(t==10||key==KEY1_PRES||key==WKUP_PRES) { 		//WKUP/KEY1按下了,或者定时时间到了		
			adcx=TIM_GetCapture1(TIM1);
			LCD_ShowxNum(124,150,adcx,4,16,0);     	//显示DAC寄存器值
			temp=(float)adcx*(3.3/256);				//得到DAC电压值
			adcx=temp;
 			LCD_ShowxNum(124,170,temp,1,16,0);     	//显示电压值整数部分
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(140,170,temp,3,16,0x80); 	//显示电压值的小数部分
 			adcx=Get_Adc_Average(ADC_Channel_1,20);  		//得到ADC转换值	  
			temp=(float)adcx*(3.3/4096);			//得到ADC电压值
			adcx=temp;
 			LCD_ShowxNum(124,190,temp,1,16,0);     	//显示电压值整数部分
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(140,190,temp,3,16,0x80); 	//显示电压值的小数部分
			t=0;
			LED0=!LED0;	   
		}	    
		delay_ms(10);	

	}
 }

