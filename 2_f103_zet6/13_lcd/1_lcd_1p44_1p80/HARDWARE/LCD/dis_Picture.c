#include "Dis_Picture.h" 
										    

//////////////////////////////////////////////////////////////////////////////////	 
/******************************************************************************/	 
//图片显示 驱动代码	

//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	
/******************************************************************************/							  

#ifdef LCD_TFT_TYPE_144
#define gImage_LCD gImage_LCD144
#endif
#ifdef LCD_TFT_TYPE_180
#define gImage_LCD gImage_LCD180
#endif

//16位 垂直扫描  右到左  高位在前
// todo:1.8inch的图片显示不正常2023.8.20
void Show_Picture(void)
{
	u32 i,j,k=0;
	
	u16 picH,picL;
	
	LCD_Clear(WHITE);//清屏
	
	Draw_Font16B(4,16,RED,"3:图片显示测试");	
	
	delay_ms(1000);    //延时显示
	
	LCD_Clear(WHITE);  //清屏
		
    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//设置一个自动换行显示区域窗口

    LCD_WriteRAM_Prepare();     	//开始写入GRAM	
                    
    for (i=0;i<lcddev.height;i++) {
        for(j=0;j<lcddev.width;j++) {            
            picH=gImage_LCD[k++];
            picL=gImage_LCD[k++];
            
            LCD_WR_DATA8(picH);  //写8位显示数据
            LCD_WR_DATA8(picL);            
        }		
    }        
	
	delay_ms(1000);    //延时显示
	
} 

