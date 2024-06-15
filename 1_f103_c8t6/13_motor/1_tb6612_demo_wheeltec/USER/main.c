#include "stm32f10x.h"
#include "delay.h"
#include "gpio.h"
#include "moto.h"
#include "pwm.h"

/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：5.7
修改时间：2021-04-29

 
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update：2021-04-29

All rights reserved
***********************************************/



//------------接线说明---------------
//TB6612丝印标识--------STM32F1主板引脚

//    PWMA    -----------    B6
//    AIN1    -----------    B8
//    AIN2    -----------    B7
//    STBY    -----------    5V
//    VM      -----------    5-12V（外接电源）
//    GND     -----------    GND  （外接电源）
//    VCC     -----------    5V   （逻辑电源）
//    GND     -----------    GND   （逻辑共地）
// 

//------------接线说明---------------

//TB6612丝印标识--------电机
//    AO1   ------------  电机线电源+
//    AO2   ------------  电机线电源-
//------------接线说明---------------


 int main(void)
 {	
   SystemInit(); //配置系统时钟为72M   
	 delay_init();    //延时函数初始化
   Gpio_Init();    //初始化gpio口B pin_7/pin_8

   pwm_int(7199,0);      //初始化pwm输出 72000 000 /7199+1=10000 
	
  while(1)
	{
	 moto(0);                //moto=1时正转
	 TIM_SetCompare1(TIM4,3000);   //设置TIM4通道1的占空比  3000/7200
//	moto(1);                //moto=0时反转
//	TIM_SetCompare1(TIM4,4000);   //设置TIM4通道1的占空比  4000/7200
	}
 }

