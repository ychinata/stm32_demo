

#include "VL53L0X.h"
#include "IIC_A.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"

// /**
//  * @brief  初始化 VL53LDX
//  * @param  无
//  * @retval 无
//  */

void VL53L0X_Test(void) 
{
	uchar i;
	
//	//查询输出版本号  
//	uchar val1 = IIC1_Single_Read(VL53L0X_REG_IDENTIFICATION_REVISION_ID);
//  Usart_SendByte(DEBUG_USARTx,0xf1); 
//	Usart_SendByte(DEBUG_USARTx,val1);
//	//查询输出设备号
//  val1 = IIC1_Single_Read(VL53L0X_REG_IDENTIFICATION_MODEL_ID);
//  Usart_SendByte(DEBUG_USARTx,0xf2); 
//	Usart_SendByte(DEBUG_USARTx,val1);
//	
//  val1 = IIC1_Single_Read(VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD);
//	Usart_SendByte(DEBUG_USARTx,0xf3); 
//	Usart_SendByte(DEBUG_USARTx,val1);
//	
//  val1 = IIC1_Single_Read(VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD);
//  Usart_SendByte(DEBUG_USARTx,0xf4); 
//	Usart_SendByte(DEBUG_USARTx,val1);

  IIC1_Single_Write(VL53L0X_REG_SYSRANGE_START, 0x01);

  u8 val = 0;
  u16 cnt = 0;
  while (cnt < 1000) { // 1 second waiting time max
    
    val = IIC1_Single_Read(VL53L0X_REG_RESULT_RANGE_STATUS);
    if (val & 0x01) break;
    cnt++;
		
		Delay_us(1000);
  }
	
	IIC1_Multiple_Read(IIC1_Rec_Data, 0x14, 12);
	
	Usart_SendByte(DEBUG_USARTx,0xf0);
	for(i=0;i<12;i++)
	{
		Usart_SendByte(DEBUG_USARTx,IIC1_Rec_Data[i]);
	}

}

/*********************************************END OF FILE**********************/
