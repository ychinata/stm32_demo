#include "headfile.h"

GPIO_TypeDef *gpio_index[3] = { GPIOA , GPIOB , GPIOC };

//-------------------------------------------------------------------------------------------------------------------
// @brief		GPIO初始化
// @param	  GPIOn	  选择GPIO端口	
// @param	  Pinx    选择引脚号
// @param	  mode    选择工作模式
// @return		void  
// Sample usage:		gpio_init(GPIO_A,Pin_0,OUT_PP);
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(GPIOn_enum GPIOn,Pinx_enum Pinx,GPIO_MODE_enum mode)
{
	uint32_t temp;
	
	RCC->APB2ENR |= 4<<GPIOn;         //使能时钟
  
	if(Pinx<8)
	{
			gpio_index[GPIOn]->CRL &= ~((0x0000000F)<<(Pinx*4));     //保持寄存器其他位不变 对选择位清零
			if(mode == OUT_PP || mode == AF_PP || mode == OUT_OD)
				gpio_index[GPIOn]->CRL |= ((0x00000003)|(1<<mode))<<(Pinx*4);        //推挽输出(默认输出低电平)
			else if(mode == IU || mode == ID)
			{
				gpio_index[GPIOn]->CRL |= 0x00000008<<(Pinx*4);        //上拉/下拉
				if(mode == IU)
				{
					temp = gpio_index[GPIOn]->ODR&(~(1<<Pinx));          //保持其他位不变 对选择位清零
					gpio_index[GPIOn]->ODR = temp|(1<<Pinx);             //保持其他位不变 对选择位置1
				}
				if(mode == ID)
					gpio_index[GPIOn]->ODR &= ~(1<<Pinx);                //保持其他位不变 对选择位清零
			}
			else
				gpio_index[GPIOn]->CRL |= (0x00000004<<mode)<<(Pinx*4);								
	}
	else
  {
			gpio_index[GPIOn]->CRH &= ~((0x0000000F)<<((Pinx-8)*4));     //保持寄存器其他位不变 对选择位清零
			if(mode == OUT_PP || mode == AF_PP || mode == OUT_OD)
				gpio_index[GPIOn]->CRH |= ((0x00000003)|(1<<mode))<<((Pinx-8)*4);        //推挽输出(默认输出低电平)
			else if(mode == IU || mode == ID)
			{
				gpio_index[GPIOn]->CRH |= 0x00000008<<((Pinx-8)*4);        //上拉/下拉
				if(mode == IU)
				{
					temp = GPIOA->ODR&(~(1<<Pinx));          //保持其他位不变 对选择位清零
					gpio_index[GPIOn]->ODR = temp|(1<<Pinx);             //保持其他位不变 对选择位置1
				}
				if(mode == ID)
					gpio_index[GPIOn]->ODR &= ~(1<<Pinx);                //保持其他位不变 对选择位清零
			}
			else
				gpio_index[GPIOn]->CRH |= (0x00000004<<mode)<<((Pinx-8)*4);	
			
		}

}

//-------------------------------------------------------------------------------------------------------------------
// @brief		设置GPIO输出电平
// @param	  GPIOn	  选择GPIO端口	
// @param	  Pinx    选择引脚号
// @param	  mode    置1为高电平 置0为低电平
// @return		void  
// Sample usage:			gpio_set(GPIO_A,Pin_0,1);
//-------------------------------------------------------------------------------------------------------------------
void gpio_set(GPIOn_enum GPIOn,Pinx_enum Pinx,uint8_t mode)
{
	uint32_t temp;
	
	if(mode)
	{
		temp = gpio_index[GPIOn]->ODR&(~(1<<Pinx));   //保持其他位不变 对选择位清零
		gpio_index[GPIOn]->ODR = temp|(1<<Pinx);      //保持其他位不变 对选择位置1
	}
	else
		gpio_index[GPIOn]->ODR &= ~(1<<Pinx);         //保持其他位不变 对选择位清零
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		读取GPIO输入
// @param	  GPIOn	  选择GPIO端口	
// @param	  Pinx    选择引脚号
// @return		uint8_t  
// Sample usage:			gpio_get(GPIO_A,Pin_4)
//-------------------------------------------------------------------------------------------------------------------
uint8_t gpio_get(GPIOn_enum GPIOn,Pinx_enum Pinx)
{
	uint32_t temp;
	
	temp = gpio_index[GPIOn]->IDR&(1<<Pinx);   //其他位全部置0 选择位为1则temp不为0 否则temp为0
	
	if(temp)                          //temp不为0 说明选择位为1 即输入高电平
		return 1;
	else                              //temp为0 说明选择位为0 即输入低电平
		return 0;
}
