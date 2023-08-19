#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "Text.h"
#include "Dis_Picture.h" 

//////////////////////////////////////////////////////////////////////////////////	 

//1.44寸  LCD 液晶驱动	  
  
//STM32F4工程模板-库函数版本
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

//********************************************************************************

//////////////////////////////////////////////////////////////////////////////////	 
				 
//LCD的画笔颜色和背景色	  

u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 

uint8_t USE_HORIZONTAL=0;    //方向设置： 		0,竖屏模式   1,横屏模式.


//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;



/******************************************************************************

  接口定义，请根据接线修改并修改相应IO初始化--驱动板子和IO要对应

//	#define LCD_SDA        	//PB15      //数据输入线
//	#define LCD_SCL        	//PB13      //时钟线
//	#define LCD_CS        	//PB12      //片选	
//	#define LCD_SDO/RST     //PB14      //数据输出/复位
//	#define LCD_DC         	//PC5       //命令/数据切换
//	#define LCD_BLK         //PB1    	  //背光控制



*******************************************************************************/


/******************************************************************************/

//函数： void SPI2_Init(void)

//函数功能：以下是 硬件SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI2的初始化


//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

/******************************************************************************/


void SPI2_Init(void)
{	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB ,ENABLE);
	
	//配置SPI2管脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
			
	//SPI2配置选项
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
 
	//这里只针对SPI口初始化
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);    //复位SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);   //停止复位SPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:全速双工模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//定义波特率预分频的值:波特率预分频值
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                          //CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设

	SPI2_ReadWriteByte(0xff);//启动传输		  
	
}



/******************************************************************************/

//函数： void LCD_GPIO_Init(void)

//函数功能：驱动液晶IO初始化配置

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	


//	#define LCD_SDA        	//PB15      //数据输入线
//	#define LCD_SCL        	//PB13      //时钟线
//	#define LCD_CS        	//PB12      //片选	
//	#define LCD_SDO/RST     //PB14      //数据输出/复位
//	#define LCD_DC         	//PC5       //命令/数据切换
//	#define LCD_BLK         //PB1    	  //背光控制

/******************************************************************************/
void LCD_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	      
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC ,ENABLE);
	
  #if    Used_HW_SPI   //初始化硬件SPI2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_12 ;   //  片选	  //背光控制
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;               //PC5    C/D  命令/数据切换
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);                 
  		
  SPI2_Init();           //初始化硬件SPI2
	
	#else   //使用IO模拟SPI通信
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;   //| GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;               //PC5    C/D  命令/数据切换
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;               //PB14--MISO    数据接收
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            //上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	#endif 
	

// LCD_HardwareRest();   //硬复位--如果IO连接，硬件复位可控有效
	
	LCD_BLK_On;           //开启背光
	
//	LCD_BLK_Off;   //关闭背光
//	LCD_BLK_On;    //开启背光
      
}




/********************************************************************/
//函数：u8 LCD_SPI_ReadWriteByte(u8 TxData)
//函数功能： SPI读写函数 ： 使用操作寄存器方式，速度更快

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节


//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/

u8 SPI2_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI2, TxData);                                  //通过外设SPIx发送一个byte  数据
	
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI2);                                //返回通过SPIx最近接收的数据	
   
}


/********************************************************************/
//函数：u8 LCD_SPI_WR(u8 byte)
//函数功能： SPI读写函数 ： 使用IO引脚模拟方式


//TxData:要写入的字节
//返回值:读取到的字节


//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/

u8 IO_SPI_ReadWriteByte(u8 TxData)
{
	unsigned char bit_ctr;
	
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)  // 输出8位
	{
		
		
		if((TxData&0x80)==0x80)LCD_SDA_SET; 			// MSB TO MOSI
			else LCD_SDA_CLR; 

		LCD_SCL_CLR;
		
		TxData=(TxData<<1);					  // shift next bit to MSB
				
		TxData|=LCD_SDO;	        		// capture current MISO bit
		
		LCD_SCL_SET;
		
	}
	
	LCD_SCL_SET;//空闲状态为高电平
	
	return TxData;
	
}


//********************************************************************/

//函数：void LCD_WR_REG(u16 regval)
//函数功能：
//写寄存器函数
//regval:寄存器值

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/

void LCD_WR_REG(u16 regval)
{   
	 LCD_CS_CLR;  //LCD_CS=0  //片选
   LCD_DC_CLR;  //LCD_DC=0  //设置命令状态
	 LCD_SPI_ReadWriteByte(regval&0x00FF);
	 LCD_CS_SET;  //LCD_CS=1	 
}



//*******************************************************************/

//函数：void LCD_WR_DATA8(u8 data)   //写8位数据
//函数功能：
//液晶屏--写8位数据函数

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/


void LCD_WR_DATA8(u8 data)   //写8位数据
{
	LCD_CS_CLR;  //LCD_CS=0  //片选
	LCD_DC_SET;	 //LCD_DC=1   //设置数据状态			    	   
	LCD_SPI_ReadWriteByte(data);	
	LCD_CS_SET;  //LCD_CS=1   			 
}


//*******************************************************************/
//函数：void LCD_WR_DATA16(u16 data)   //写16位数据
//函数功能：写LCD数据
//输入参数：
//data:要写入的值

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/


void LCD_WR_DATA16(u16 data)
{	
	
 	LCD_CS_CLR;  //LCD_CS=0  //片选
	LCD_DC_SET;	 //LCD_DC=1   //设置数据状态
	LCD_SPI_ReadWriteByte(data>>8);
	LCD_SPI_ReadWriteByte(data);
	LCD_CS_SET;    //LCD_CS=1
	
}


//*******************************************************************/

//函数：void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)  //写液晶屏寄存器

//函数功能：写寄存器

//输入参数: 
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);		   //写入要写的寄存器序号	 
	LCD_WR_DATA8(LCD_RegValue);//写入数据	    		 
}


//*******************************************************************/
//函数：void LCD_WriteRAM_Prepare(void)   
//函数功能：开始写GRAM  命令

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/

void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}	



//*******************************************************************/
//函数：LCD_WriteRAM(u16 RGB_Code)   
//函数功能：写入点阵颜色值
//输入参数:
//RGB_Code:颜色值

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/

void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA16( RGB_Code );//写十六位GRAM
}

//********************************************************************************/
//函数：void LCD_DisplayOn(void)
//函数功能：
//LCD开启显示

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//开启显示
}	


//*******************************************************************/
//函数：void LCD_DisplayOff(void)
//函数功能：
//LCD关闭显示

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/

void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//关闭显示

} 

//********************************************************************************/
//函数：void LCD_SoftRest(void)
//函数功能：给屏幕发命令，执行软复位命令
//LCD开启显示

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/
void LCD_SoftRest(void)
{					   
	LCD_WR_REG(0x01);	//发送软复位命令
	LCD_Delay_ms(100);      // delay 50 ms 
}	


//********************************************************************************/
//函数：void LCD_SoftRest(void)
//函数功能：给屏幕发命令，执行软复位命令
//LCD开启显示

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/
void LCD_HardwareRest(void)
{					   
	LCD_RST_CLR;     //液晶屏复位 --硬复位--使能       //PB14作为液晶屏复位控制引脚
	LCD_Delay_ms(50);      // delay 50 ms 
	LCD_DC_SET;      //液晶屏复位 --硬复位--失能       //PB14作为液晶屏复位控制引脚
	LCD_Delay_ms(30);      // delay 30 ms 
}	



//*******************************************************************/
//函数：void LCD_SetCursor(u16 Xpos, u16 Ypos)
//函数功能：设置光标位置
//输入参数：
//Xpos:横坐标
//Ypos:纵坐标

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
    if(USE_HORIZONTAL==0) {
            
        LCD_WR_REG(lcddev.setxcmd); 
        LCD_WR_DATA16(Xpos+2);

        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA16(Ypos+3);	
            
    } else {
        LCD_WR_REG(lcddev.setxcmd); 
        LCD_WR_DATA16(Xpos+3);

        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA16(Ypos+2);	
    }
	
}





//*******************************************************************/
//函数：void LCD_DrawPoint(u16 x,u16 y)
//函数功能：画点
//输入参数：
//x,y:坐标
//POINT_COLOR:此点的颜色

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/

void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA16(POINT_COLOR); 
}


//*******************************************************************/
//函数：void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
//函数功能：快速画点
//输入参数：
//x,y:坐标
//color:颜色

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*******************************************************************/

void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	
     
    	LCD_SetCursor(x,y);		//设置光标位置 
			LCD_WR_REG(lcddev.wramcmd); 
			LCD_WR_DATA16(color); //写入16位颜色

}	 





/**************************************************************************/
//函数：void LCD_Display_Dir(u8 dir)
//函数功能：设置LCD的显示方向及像素参数

//输入参数：

//设置LCD显示方向

////dir:   0,竖屏  正
//         1,竖屏  反
//         2,横屏  左
//         3,横屏  右

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*************************************************************************/
void LCD_Display_Dir(u8 dir)
{
	
		
	if(dir==0)			     //竖屏  正
	{
		lcddev.dir=0;	     //竖屏
		lcddev.width=LCD_X;
		lcddev.height=LCD_H;
    
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;
		
    LCD_WriteReg(0x36,0xC8);//选择扫描方向		
		
		USE_HORIZONTAL=0;   //方向设置： 		0,竖屏模式   1,横屏模式.
	

	}
	
else if (dir==1)			 //竖屏
	{	  				
		lcddev.dir=0;	     //竖屏
		lcddev.width=LCD_X;
		lcddev.height=LCD_H;
    
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;
		
		LCD_WriteReg(0x36,0x48);//选择扫描方向		
		
		USE_HORIZONTAL=0;   //方向设置： 		0,竖屏模式   1,横屏模式.
		

	} 	
	
	
	else if (dir==2)			//横屏
	{	  				
		lcddev.dir=1;	     //横屏
		lcddev.width=LCD_H;
		lcddev.height=LCD_X;

		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  
		
		LCD_WriteReg(0x36,0xA8);//选择扫描方向		
		
		USE_HORIZONTAL=1;   //方向设置： 		0,竖屏模式   1,横屏模式.
		

		
	} 
 else if (dir==3)				  //横屏
	{	  				
		lcddev.dir=1;	        //横屏
		lcddev.width=LCD_H;
		lcddev.height=LCD_X;

		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B; 
    LCD_WriteReg(0x36,0x68);//选择扫描方向		
		
		USE_HORIZONTAL=1;   //方向设置： 		0,竖屏模式   1,横屏模式.
		


	} 	
 else //设置默认为竖屏--正
 {
	  lcddev.dir=0;	     //竖屏
		lcddev.width=LCD_X;
		lcddev.height=LCD_H;
    
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;

	  LCD_WriteReg(0x36,0xC8);//选择扫描方向		
	 
	  USE_HORIZONTAL=0;   //方向设置： 		0,竖屏模式   1,横屏模式.
	 
	 
 }	 


//////以下设置，为窗口参数设置，设置了全屏的显示范围			

   LCD_Set_Window(0,0,lcddev.width,lcddev.height);//设置全屏窗口
 
 
 

 	
}	


/**************************************************************************/
//函数：void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
//函数功能：设置LCD的显示窗口

//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height.

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
//*************************************************************************/

void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   
	
	
	  width=sx+width-1;
	  height=sy+height-1;
   if(USE_HORIZONTAL==0)
			 {
				 
				LCD_WR_REG(lcddev.setxcmd); 
				LCD_WR_DATA16(sx+2);      //设置 X方向起点
				LCD_WR_DATA16(width+2);   //设置 X方向终点	
					
				LCD_WR_REG(lcddev.setycmd);
				LCD_WR_DATA16(sy+3);      //设置 Y方向起点
				LCD_WR_DATA16(height+3);  //设置 Y方向终点
				 
			 }
		 else
			 {
				  LCD_WR_REG(lcddev.setxcmd); 
					LCD_WR_DATA16(sx+3);      //设置 X方向起点
					LCD_WR_DATA16(width+3);   //设置 X方向终点	
						
					LCD_WR_REG(lcddev.setycmd);
					LCD_WR_DATA16(sy+2);      //设置 Y方向起点
					LCD_WR_DATA16(height+2);  //设置 Y方向终点
				 
		  }
} 


/*******************************************************************************/
//函数：void LCD_Init(void)
//函数功能：初始化lcd


//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_Init(void)
{ 	
	 
  LCD_GPIO_Init();        //初始化驱动 I/O接口	
	
	LCD_SoftRest();        //软复位 
	
	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	LCD_WR_REG(0x11);//Sleep exit 
	delay_ms (120);
		
	//ST7735R Frame Rate
	LCD_WR_REG(0xB1); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 

	LCD_WR_REG(0xB2); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 

	LCD_WR_REG(0xB3); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	
	LCD_WR_REG(0xB4); //Column inversion 
	LCD_WR_DATA8(0x07); 
	
	//ST7735R Power Sequence
	LCD_WR_REG(0xC0); 
	LCD_WR_DATA8(0xA2); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x84); 
	LCD_WR_REG(0xC1); 
	LCD_WR_DATA8(0xC5); 

	LCD_WR_REG(0xC2); 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0x00); 

	LCD_WR_REG(0xC3); 
	LCD_WR_DATA8(0x8A); 
	LCD_WR_DATA8(0x2A); 
	LCD_WR_REG(0xC4); 
	LCD_WR_DATA8(0x8A); 
	LCD_WR_DATA8(0xEE); 
	
	LCD_WR_REG(0xC5); //VCOM 
	LCD_WR_DATA8(0x0E); 
	
	LCD_WR_REG(0x36); //MX, MY, RGB mode 
	LCD_WR_DATA8(0xC8); 
	
	//ST7735R Gamma Sequence
	LCD_WR_REG(0xe0); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x1a); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x18); 
	LCD_WR_DATA8(0x2f); 
	LCD_WR_DATA8(0x28); 
	LCD_WR_DATA8(0x20); 
	LCD_WR_DATA8(0x22); 
	LCD_WR_DATA8(0x1f); 
	LCD_WR_DATA8(0x1b); 
	LCD_WR_DATA8(0x23); 
	LCD_WR_DATA8(0x37); 
	LCD_WR_DATA8(0x00); 	
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x10); 

	LCD_WR_REG(0xe1); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x1b); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x17); 
	LCD_WR_DATA8(0x33); 
	LCD_WR_DATA8(0x2c); 
	LCD_WR_DATA8(0x29); 
	LCD_WR_DATA8(0x2e); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x39); 
	LCD_WR_DATA8(0x3f); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0x10);  
	
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x7f);

	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x9f);
	
	LCD_WR_REG(0xF0); //Enable test command  
	LCD_WR_DATA8(0x01); 
	LCD_WR_REG(0xF6); //Disable ram power save mode 
	LCD_WR_DATA8(0x00); 
	
	LCD_WR_REG(0x3A); //65k mode 
	LCD_WR_DATA8(0x05); 
	
	
	LCD_WR_REG(0x29);//Display on	 

	  LCD_Display_Dir(LCD_DIR_Mode);	//选择--屏幕显示方式
		
	  LCD_BLK_On;					//点亮背光
		
	  LCD_Clear(WHITE);
	
	
	
}


//////////////////以下函数是屏幕显示图形的简单驱动函数////////////DevEBox  大越创新//////shop389957290.taobao.com//////////////////////////////////////



/*******************************************************************************/
//函数：void LCD_Clear(u16 color)
//函数功能：全屏清屏填充函数
//输入参数：
//color:要清屏的填充色

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_Clear(u16 color)
{
	u32 index=0;
	u32 totalpoint;
	
	LCD_Set_Window(0,0,lcddev.width,lcddev.height);//设置全屏窗口
	
	totalpoint=lcddev.width * lcddev.height; 			//得到总点数
 
	LCD_SetCursor(0,0);	//设置光标位置 
	
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	
	
	for(index=0;index<totalpoint;index++)
	{
		LCD_WR_DATA16(color);	
	}
} 


/*******************************************************************************/
//函数：void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)

//函数功能：区域填充函数--填充单个颜色
//输入参数：
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
		u32 i;
		u32 xlen=0;

    //设置窗口
	
	  LCD_Set_Window(sx,sy,ex-sx,ey-sy);//设置窗口
 	
	  LCD_WR_REG(lcddev.wramcmd);	  
	
 		xlen=(ex-sx)*(ey-sy);//计算出总共需要写入的点数
		
		LCD_WriteRAM_Prepare();     			//开始写入GRAM
		
		for(i=0;i<=xlen;i++)
		{
		 	LCD_WR_DATA16(color);	//显示颜色 	    
		}


		//恢复全屏窗口
				
		//以下设置，为窗口参数设置，设置了全屏的显示范围			
			
    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//设置全屏窗口
		 
}


/*******************************************************************************/
//函数：void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)

//函数功能：区域填充函数
//输入参数：
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  

		u32 i;
		u32 xlen=0;

 //设置窗口

    LCD_Set_Window(sx,sy,ex-sx,ey-sy);//设置窗口	
		  	
	  LCD_WR_REG(lcddev.wramcmd);	  
	
 		xlen=(ex-sx)*(ey-sy);//计算出总共需要写入的点数
		
		LCD_WriteRAM_Prepare();     			//开始写入GRAM
		
		for(i=0;i<=xlen;i++)
		{
		 	LCD_WR_DATA16(*color);	//显示颜色 	    
		}


	//以下设置，为窗口参数设置，设置了全屏的显示范围			
			
    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//设置全屏窗口
		

}



/*******************************************************************************/
//函数：void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
//函数功能：画线
//输入参数：
//x1,y1:起点坐标
//x2,y2:终点坐标
//Color;线条颜色

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 Color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	
	
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else
     {
		    incx=-1;
		    delta_x=-delta_x;
		 }

	
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else
     { 
		   incy=-1;
		   delta_y=-delta_y;
		 }

	
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_Fast_DrawPoint(uRow,uCol,Color);//画点 --使用输入颜色参数 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

/*******************************************************************************/
//函数：void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
//函数功能：画矩形	  
//输入参数：
//(x1,y1),(x2,y2):矩形的对角坐标
//Color;线条颜色

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 Color)
{
	LCD_DrawLine(x1,y1,x2,y1,Color);
	LCD_DrawLine(x1,y1,x1,y2,Color);
	LCD_DrawLine(x1,y2,x2,y2,Color);
	LCD_DrawLine(x2,y1,x2,y2,Color);
}

/*******************************************************************************/
//函数：void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
//函数功能：在指定位置画一个指定大小的圆
//输入参数：
//(x,y):中心点
//r    :半径
//Color;线条颜色

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_Draw_Circle(u16 x0,u16 y0,u8 r, u16 Color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_Fast_DrawPoint(x0+a,y0-b,Color);             //5
 		LCD_Fast_DrawPoint(x0+b,y0-a,Color);             //0           
		LCD_Fast_DrawPoint(x0+b,y0+a,Color);             //4               
		LCD_Fast_DrawPoint(x0+a,y0+b,Color);             //6 
		LCD_Fast_DrawPoint(x0-a,y0+b,Color);             //1       
 		LCD_Fast_DrawPoint(x0-b,y0+a,Color);             
		LCD_Fast_DrawPoint(x0-a,y0-b,Color);             //2             
  	LCD_Fast_DrawPoint(x0-b,y0-a,Color);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}


/*******************************************************************************/
//函数：LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode)
//函数功能：在指定位置显示一个字符
//输入参数：
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//color,字符颜色
//mode:叠加方式(1)还是非叠加方式(0)

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode)
{  							  
  u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,color);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}



/*******************************************************************************/
//函数：u32 LCD_Pow(u8 m,u8 n)
//函数功能：m^n函数
//输入参数：两个8位数据
//返回值:m^n次方.

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	


/*******************************************************************************/
//函数：void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color)
//函数功能：显示数字,高位为0,则不显示
//输入参数：

//x,y :起点坐标	 
//num:数值(0~4294967295);	
//len :数字的位数
//size:字体大小
//color:颜色 


//返回值:无

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,color,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,color,0); 
	}
} 



/*******************************************************************************/
//函数：void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color,u8 mode)
//函数功能：显示数字,高位为0,还是显示
//输入参数：
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//color:颜色 
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/

void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,color,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,color,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,color,mode&0X01); 
	}
} 

/*******************************************************************************/
//函数：void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u16 color,u8 *p)
//函数功能：显示字符串
//输入参数：
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//color:颜色 
//*p:字符串起始地址		  

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/


void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u16 color,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,color,0);
        x+=size/2;
        p++;
    }  
}







//****************************************************************************************/
//函数：void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
//功能描述: 在屏幕显示一凸起的按钮框
//输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
//输    出: 无


//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/****************************************************************************************/

void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
{
	
	LCD_DrawLine(x1,y1, x2,y1,GRAY2);  //H
		
	LCD_DrawLine(x1+1,y1+1,x2,y1+1,GRAY1);  //H
		
	LCD_DrawLine(x1,  y1,  x1,y2,GRAY2);    //V
		
	LCD_DrawLine(x1+1,y1+1,x1+1,y2,GRAY1);  //V
	
	LCD_DrawLine(x1, y2, x2, y2,WHITE);     //H
	LCD_DrawLine(x2, y1, x2, y2,WHITE);     //V
}

//****************************************************************************************/
//函数：void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
//功能描述: 在屏幕显示一凹下的按钮框
//输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
//输    出: 无
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/****************************************************************************************/
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
	
	LCD_DrawLine(x1,  y1,  x2,y1,WHITE);    //H
	LCD_DrawLine(x1,  y1,  x1,y2,WHITE);    //V
	
	LCD_DrawLine(x1+1,y2-1,x2,y2-1,GRAY1);  //H
		
	LCD_DrawLine(x1,  y2,  x2,y2,GRAY2);    //H
		
	LCD_DrawLine(x2-1,y1+1,x2-1,y2,GRAY1);  //V
		
  LCD_DrawLine(x2  ,y1  ,x2,y2,GRAY2);    //V
	

}


//****************************************************************************************/
//函数：void Draw_Test(void)
//功能描述: 绘制图形函数测试


//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/****************************************************************************************/

void Draw_Test(void)
{
	
		LCD_Clear(WHITE); //清屏
		
	  LCD_DrawLine(20,20, lcddev.width-20,lcddev.height-20,RED);//划线函数
	  LCD_DrawLine(20,lcddev.height-20, lcddev.width-20, 20,RED);//划线函数
	
	  LCD_DrawRectangle(20,20,lcddev.width-20,lcddev.height-20, BLUE);//绘制方形状
	  
	  LCD_Draw_Circle(lcddev.width/2,lcddev.height/2,lcddev.width/3-1, BRED);//绘制圆形
	  LCD_Draw_Circle(lcddev.width/2,lcddev.height/2,lcddev.width/3, BRED);//绘制圆形
	  LCD_Draw_Circle(lcddev.width/2,lcddev.height/2,lcddev.width/3+1, BRED);//绘制圆形
			
		delay_ms(2000);//延时

}


//****************************************************************************************/
//函数：void Color_Test(void)
//功能描述: 颜色填充显示测试


//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/****************************************************************************************/

void Color_Test(void)
{
	
		LCD_Clear(WHITE); //清屏
		
		Draw_Font16B(8,16,BLUE,"1:颜色填充测试");
	
	  delay_ms(1000);
	
	  LCD_Fill(5,5,lcddev.width-5,lcddev.height-5,RED);//设置一个窗口，写入指定区域颜色
	
	  LCD_Fill(20,20,lcddev.width-20,lcddev.height-20,YELLOW);//设置一个窗口，写入指定区域颜色
	
	  LCD_Fill(50,50,lcddev.width-50,lcddev.height-50,BLUE);//设置一个窗口，写入指定区域颜色
	
	  LCD_Fill(60,60,lcddev.width-60,lcddev.height-60,MAGENTA);//设置一个窗口，写入指定区域颜色
	
		delay_ms(1000);

		LCD_Clear(WHITE);
	
	  Draw_Test();//绘图函数测试
	
		delay_ms(300);//延时
		LCD_Clear(BLACK);
		delay_ms(300);//延时
		LCD_Clear(RED);
		delay_ms(300);//延时
		LCD_Clear(YELLOW);
		delay_ms(300);//延时
		LCD_Clear(GREEN);
		delay_ms(300);//延时
		LCD_Clear(BLUE);
		delay_ms(300);//延时

}


uint16_t D_Color=BLUE; //点阵颜色
uint16_t B_Color=WHITE; //背景颜色

//****************************************************************************************/
//函数：void Font_Test(void)
//功能描述: 字体字形显示测试


//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/****************************************************************************************/

void Font_Test(void)
{
	LCD_Clear(WHITE);
	Draw_Font16B(8,16,BLUE,"2:文字显示测试");

	delay_ms(1000);
	LCD_Clear(WHITE);
	
	Draw_Font24B(16,4,RED, "大越创新");
	Draw_Font24B(4,32,BLACK,"液晶屏测试");
	Draw_Font16B(4,60, BLACK,"嵌入式开发网");

	Draw_Font16B(0,100,BLUE,"F:mcudev.taobao.com");
	
	delay_ms(1000);	
	
	LCD_Clear(WHITE);

	Draw_Font16B(4,20,RED,"0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz");

	delay_ms(1000);	
	
	LCD_Clear(WHITE);


}


//****************************************************************************************/
//函数：void Demo_Menu(void)
//功能描述: 演示程序菜单


//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/****************************************************************************************/
void LCD_DemoMenu(void)
{

	LCD_Clear(WHITE); //清屏
	
	Draw_Font24B(12,2,RED,"大越创新");
	Draw_Font24B(4,30,RED,"液晶屏测试");

	Draw_Font16B(4,60,BLUE,"1: 颜色填充");
	Draw_Font16B(4,80,BLUE,"2: 文字显示");
	Draw_Font16B(4,100,BLUE,"3: 图片显示");
//  
	Draw_Font16B(4,120,RED,"F:mcudev.taobao.com");
//	Draw_Font16B(4,140,RED,"F:devebox.taobao.com");
//	
    delay_ms(2000);//延时

    Color_Test();       //颜色填充测试
    Font_Test();        //字体字形显示测试
    GBK_LibFont_Test(); //GBK字库测试 -(如果使用不带字库的液晶屏版本，此处可以屏蔽，不做字库测试）
    //Show_Picture();     //显示一张图片	// 注释掉可以减少100k的bin大小
}

