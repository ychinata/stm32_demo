#include "Dis_Picture.h" 
										    

//////////////////////////////////////////////////////////////////////////////////	 
/******************************************************************************/	 
//ͼƬ��ʾ ��������	

//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	
/******************************************************************************/							  

#ifdef LCD_TFT_TYPE_144
#define gImage_LCD gImage_LCD144
#endif
#ifdef LCD_TFT_TYPE_180
#define gImage_LCD gImage_LCD180
#endif

//16λ ��ֱɨ��  �ҵ���  ��λ��ǰ
// todo:1.8inch��ͼƬ��ʾ������2023.8.20
void Show_Picture(void)
{
	u32 i,j,k=0;
	
	u16 picH,picL;
	
	LCD_Clear(WHITE);//����
	
	Draw_Font16B(4,16,RED,"3:ͼƬ��ʾ����");	
	
	delay_ms(1000);    //��ʱ��ʾ
	
	LCD_Clear(WHITE);  //����
		
    LCD_Set_Window(0,0,lcddev.width,lcddev.height);//����һ���Զ�������ʾ���򴰿�

    LCD_WriteRAM_Prepare();     	//��ʼд��GRAM	
                    
    for (i=0;i<lcddev.height;i++) {
        for(j=0;j<lcddev.width;j++) {            
            picH=gImage_LCD[k++];
            picL=gImage_LCD[k++];
            
            LCD_WR_DATA8(picH);  //д8λ��ʾ����
            LCD_WR_DATA8(picL);            
        }		
    }        
	
	delay_ms(1000);    //��ʱ��ʾ
	
} 

