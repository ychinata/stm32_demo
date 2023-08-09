	
/********************************************************************************

//���빦�ܣ� STM32F4x1 ��Ƭ��  ��׼��ʵ������

//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/	
	

/* Includes ------------------------------------------------------------------*/

#include "main.h"  //ϵͳͷ�ļ�


// STM32F411 �ⲿ����25Mhz�����ǵ�USBʹ�ã��ڲ�Ƶ������Ϊ96Mhz
// STM32F401 �ⲿ����25Mhz���ڲ�Ƶ������Ϊ84Mhz�z
// ��Ҫ����Ƶ��,�����޸�  system_stm32f4xx.c

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
	
  SystemCoreClockUpdate();        //  ����  RCC_Clocks ϵͳʱ�ӱ���      
  RCC_GetClocksFreq(&RCC_Clocks); //  ��ȡ  RCC_Clocks  ϵͳʱ��
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);  // ����  SysTick  ϵͳʱ���ж�Ϊ1mS

	
  /* Insert 50 ms delay */
	
  delay_ms(50);           // ��ʱ����--50mS
		
  GPIO_Config();          // I/O ���Ź��ܳ�ʼ������ �� ��ʼ�� LED ��Ӧ������
	
	USART1_init(115200);    // ��ʼ�����ڲ�����Ϊ115200
	
	LCD_Init();             //1.44��Һ���� ��ʼ��LCD SPI �ӿ�
	
	GBK_Lib_Init();         // Ӳ��GBK�ֿ��ʼ��--(���ʹ�ò����ֿ��Һ�����汾���˴��������Σ������ֿ��ʼ����
	
	LCD_Clear(WHITE);       // ����
	
		
	Draw_Font24B(12,2,RED," I/O ����");
	Draw_Font24B(4,30,BLUE," ��Խ����");
	Draw_Font16B(4,60,BLUE,"/mcudev.taobao.com");
	Draw_Font16B(4,80,BLUE,"/shop389957290.taobao.com");
	
	
	
/* Infinite loop */	
  while (1)
  {
		
			
		LED2_ON;    //D2��Ӧ����PC13  ���ͣ���;
		
		delay_ms(1000);  		   							//��ʱ500ms
		
		LED2_OFF;	    //D2��Ӧ����PC13 ���ߣ���;
		
		delay_ms(1000);                      //��ʱ500ms
			
			
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

//���빦�ܣ�  ��׼��ʵ������

//STM32F4����ģ��-�⺯���汾
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

*******************************************************************************/	



