#include "led.h"

void LED_Init()
{
  /* LED on C13 pin(PC13) ***********************************/ 
  /* Enable the GPIOCperipheral */ 
  GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//开启时钟

	
  /* Configure C13 pin(PC13) in output function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//低速
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽模式
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //无上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}
