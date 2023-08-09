#ifndef __LCD_H
#define __LCD_H		

///////////////////////////加入项目的关联头文件////////////////////////////////////

#include "main.h"	 
#include "stdlib.h"
#include "delay.h"




extern uint16_t D_Color; //点阵颜色
extern uint16_t B_Color; //背景颜色

//////////////////////////////////////////////////////////////////////////////////	 

//LCD液晶屏驱动程序

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	


//对应驱动接口引脚

//	#define LCD_SDA        	//PB15      //数据输入线
//	#define LCD_SCL        	//PB13      //时钟线
//	#define LCD_CS        	//PB12      //片选	
//	#define LCD_SDO/RST     //PB14      //数据输出/复位
//	#define LCD_D/C         //PB2       //（对应 D/C ）命令/数据切换
//	#define LCD_BLK         //PB10    	 //背光控制

//********************************************************************************//

//数据类型定义

/* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       long long int64_t;


/* exact-width unsigned integer types */
typedef unsigned          char   uint8_t;
typedef unsigned short     int   uint16_t;
typedef unsigned           int   uint32_t;
typedef unsigned       long long uint64_t;


typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//对应驱动接口引脚

//	#define LCD_SDA        	//PB15      //数据输入线
//	#define LCD_SCL        	//PB13      //时钟线
//	#define LCD_CS        	//PB12      //片选	
//	#define LCD_SDO/RST     //PB14      //数据输出/复位
//	#define LCD_RS          //PB2       //（对应 D/C ）命令/数据切换
//	#define LCD_BLK         //PB10    	 //背光控制


//液晶控制口置1操作语句宏定义

#define	LCD_SDA_SET  	GPIO_SetBits(GPIOB,GPIO_Pin_15)     //PB15      //数据输入线
#define	LCD_SCL_SET  	GPIO_SetBits(GPIOB,GPIO_Pin_13)     //PB13      //时钟线 

#define	LCD_CS_SET  	GPIO_SetBits(GPIOB,GPIO_Pin_12)     //PB12      //片选	   
#define LCD_RST_SET   GPIO_SetBits(GPIOB,GPIO_Pin_14)     //PB14      //数据输出/复位

#define	LCD_RS_SET  	GPIO_SetBits(GPIOB,GPIO_Pin_2)      //PB2       //（对应 D/C ）命令/数据切换  
#define	LCD_BLK_SET  	GPIO_SetBits(GPIOB,GPIO_Pin_10)     //PB10    	 //背光控制

//液晶控制口置0操作语句宏定义

#define	LCD_SDA_CLR  	GPIO_ResetBits(GPIOB,GPIO_Pin_15)    //PB15      //数据输入线   
#define	LCD_SCL_CLR  	GPIO_ResetBits(GPIOB,GPIO_Pin_13)    //PB13      //时钟线

#define	LCD_CS_CLR  	GPIO_ResetBits(GPIOB,GPIO_Pin_12)    //PB12      //片选	 
#define LCD_RST_CLR   GPIO_ResetBits(GPIOB,GPIO_Pin_14)    //PB14      //数据输出/复位

#define	LCD_RS_CLR  	GPIO_ResetBits(GPIOB,GPIO_Pin_2)     //PB2       //（对应 D/C ）命令/数据切换
#define	LCD_BLK_CLR  	GPIO_ResetBits(GPIOB,GPIO_Pin_10)    //PB10    	 //背光控制


#define	LCD_BLK_On          LCD_BLK_SET   		 //开背光  LCD背光--PB10 	    
#define	LCD_BLK_Off         LCD_BLK_CLR    		 //关背光  LCD背光--PB10 	 


//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				  //LCD ID
	u8  dir;			  //横屏还是竖屏控制：竖屏和横屏。	
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令 
}_lcd_dev; 	  



/////////////////////////////////////用户配置区///////////////////////////////////	

//支持横竖屏快速定义切换

#define LCD_X  	128     //横向点阵数量
#define LCD_H  	128     //纵向


//竖屏和横屏的选择，设置这里：
#define LCD_DIR_Mode  	  0	    //4种工作模式，0和1是竖屏模式，2和3是横屏模式
#define LCD_Picture  	    0	    //演示图片选择设置： 		0,竖屏图片   1,横屏图片



//屏幕显示方向变量（无需设置）
extern uint8_t USE_HORIZONTAL;    //方向设置： 		0,竖屏模式   1,横屏模式.



//////////////////////////////////////////////////////////////////////////////////	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数

//LCD的画笔颜色和背景色	   

extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色


//////////////////////////////////////////////////////////////////////////////////	



//////////////////////////////////////////////////////////////////////////////////
	 
//扫描方向定义--扫描方式有不同规格，可能定义不左右和上下的参照方向不同，总结方式，只有一下八种

#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 


///////////////////////////  颜色值  ///////////////////////////////////////////////////////

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define BRED             0xF81F
#define GRED 			       0xFFE0
#define GBLUE			       0x07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0xBC40 //棕色
#define BRRED 			     0xFC07 //棕红色
#define GRAY  			     0x8430 //灰色


//GUI颜色

#define DARKBLUE      	 0x01CF	//深蓝色
#define LIGHTBLUE      	 0x7D7C	//浅蓝色  
#define GRAYBLUE       	 0x5458 //灰蓝色
//以上三色为PANEL的颜色 
 
 
#define LIGHTGREEN     	 0x841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			     0xC618 //浅灰色(PANNEL),窗体背景色

#define GRAY0   0xEF7D   	    //灰色0 
#define GRAY1   0x8410      	//灰色1   
#define GRAY2   0x4208      	//灰色2  

#define LGRAYBLUE        0xA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0x2B12 //浅棕蓝色(选择条目的反色)



////////////////////////延时函数--宏定义/////////////////////////////////////////////////



#define	LCD_Delay_us  	delay_us   //延时函数
#define	LCD_Delay_ms  	delay_ms   //延时函数



////////////////////////硬件驱动-功能函数//////DevEBox  大越创新//////shop389957290.taobao.com///////////////////////////////////////////////////


void SPI2_Init(void);                //这里针是对SPI2的初始化
void LCD_GPIO_Init(void);            //液晶IO初始化配置
u8 SPI2_ReadWriteByte(u8 TxData);    //SPI接口数据传输函数

void LCD_WR_REG(vu16 regval);        //液晶屏--写寄存器函数
void LCD_WR_DATA8(u8 data);          //写8位数据
void LCD_WR_DATA16(vu16 data);       //写16位数据


void LCD_Init(void);													   	              //初始化
void LCD_HardwareRest(void);                                    //硬复位--如果IO连接，硬件复位可控有效
void LCD_SoftRest(void);                                        //软复位
void LCD_DisplayOn(void);													              //开显示
void LCD_DisplayOff(void);													            //关显示


//////////////////以下函数是屏幕显示图形的简单驱动函数////////////DevEBox  大越创新//////shop389957290.taobao.com//////////////////////////////////////

void LCD_Clear(u16 Color);	 												            //清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos);										      //设置光标
void LCD_DrawPoint(u16 x,u16 y);											          //画点--使用设置的笔尖颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								  //快速画点--使用当前输入颜色参数

void LCD_Draw_Circle(u16 x0,u16 y0,u8 r, u16 Color);							 //画圆
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 Color);		   //画线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 Color); //画矩形

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   			             //填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);		             //填充指定颜色
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode);					   //显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color);  					    //显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color,u8 mode);				  //显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u16 color,u8 *p);		//显示一个字符串,12/16字体

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);               //写寄存器

void LCD_WriteRAM_Prepare(void);                                //开始写GRAM  命令
void LCD_WriteRAM(u16 RGB_Code);		                            //LCD写GRAM

void LCD_Display_Dir(u8 dir);						                        //设置屏幕显示方向
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);         //设置窗口					

void Draw_Test(void);//绘图工具函数测试
void Color_Test(void);//颜色填充显示测试
void Font_Test(void);// 字体字形显示测试
void GBK_LibFont_Test(void);//GBK 字体字形显示测试
void Demo_Menu(void);//演示程序菜单



#endif  
	 
	 
/*******************************************************************************/

//DevEBox  大越创新

//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com
/*******************************************************************************/


