	
/********************************************************************************

//代码功能： STM32F4x1 单片机  标准库实例例程

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/	
	

/* Includes ------------------------------------------------------------------*/

#include "main.h"  //系统头文件


// STM32F411 外部晶振25Mhz，考虑到USB使用，内部频率设置为96Mhz
// STM32F401 外部晶振25Mhz，内部频率设置为84Mhz
// 需要其他频率,自行修改  system_stm32f4xx.c

/** @addtogroup Template_Project   
* @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

RCC_ClocksTypeDef   RCC_Clocks;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**************************************************************************

  * @brief  Main program
  * @param  None
  * @retval None
	
*************************************************************************/
	
	
int main(void)
{
	/* Enable Clock Security System(CSS): this will generate an NMI exception
     when HSE clock fails *****************************************************/
  RCC_ClockSecuritySystemCmd(ENABLE);
	
 /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files before to branch to application main.
       To reconfigure the default setting of SystemInit() function, 
       refer to system_stm32f4xx.c file */

  /* SysTick end of count event each 1ms */
	
  SystemCoreClockUpdate();        //  更新  RCC_Clocks 系统时钟变量      
  RCC_GetClocksFreq(&RCC_Clocks); //  获取  RCC_Clocks  系统时钟
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);  // 设置  SysTick  系统时钟中断为1mS

	
  /* Insert 50 ms delay */
	
  delay_ms(50);           // 延时函数--50mS
		
  GPIO_Config();          // I/O 引脚功能初始化函数 ： 初始化 LED 对应的引脚
	
	USART1_init(115200);    // 初始化串口波特率为115200
	
	LCD_Init();             //1.44寸液晶屏 初始化LCD SPI 接口
	
	GBK_Lib_Init();         // 硬件GBK字库初始化--(如果使用不带字库的液晶屏版本，此处可以屏蔽，不做字库初始化）
	
	LCD_Clear(WHITE);       // 清屏
	
		
	Draw_Font24B(12,2,RED," I/O 测试");
	Draw_Font24B(4,30,BLUE," 大越创新");
	Draw_Font16B(4,60,BLUE,"/mcudev.taobao.com");
	Draw_Font16B(4,80,BLUE,"/shop389957290.taobao.com");
	
	
	
/* Infinite loop */	
  while (1)
  {
		
			
		LED2_ON;    //D2对应引脚PC13  拉低，亮;
		
		delay_ms(1000);  		   							//延时500ms
		
		LED2_OFF;	    //D2对应引脚PC13 拉高，灭;
		
		delay_ms(1000);                      //延时500ms
			
			
	}
}










#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



	
/********************************************************************************

//代码功能：  标准库实例例程

//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

*******************************************************************************/	



