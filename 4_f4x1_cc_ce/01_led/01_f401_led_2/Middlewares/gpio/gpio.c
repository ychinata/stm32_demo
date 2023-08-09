
/********************************************************************************

//代码功能：  标准库 GPIO 控制

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/
  
#include "gpio.h"
 
//初始化LED2--PC13   并使能这个口的时钟		IO初始化    

void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
////   /* Output HSE clock on MCO1 pin(PA8) ****************************************/ 
////  /* Enable the GPIOA peripheral */ 
////  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
////  
////  /* Configure MCO1 pin(PA8) in alternate function */
////  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
////  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
////  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
////  GPIO_Init(GPIOA, &GPIO_InitStructure);
////    
////  /* HSE clock selected to output on MCO1 pin(PA8)*/
////  RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
  
  /* 初始化对应的IO接口 PC13   LED on C13 pin(PC13) ***********************************/ 
  /* Enable the GPIOCperipheral */ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* Configure C13 pin(PC13) in output function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
}





























/********************************************************************************

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/






