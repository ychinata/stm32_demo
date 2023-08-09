#include "led.h"

void LED_Init()
{
  /* LED on C13 pin(PC13) ***********************************/ 
  /* Enable the GPIOCperipheral */ 
  GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//����ʱ��

	
  /* Configure C13 pin(PC13) in output function */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//����ģʽ
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}
