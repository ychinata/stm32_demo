/*************************************
使用串口3接收STP-23L模块数据，使用串口1输出接收成功的次数，oled显示距离
接线说明：
STP-23L模块      单片机接口
-----------------------------------
   P5V----------------5V	
   GND----------------GND
   TX-----------------B11
   RX----------------不接
***************************************/
#include "sys.h"
#include "oled.h"

void OLED_SHOW(void);

u16 receive_cnt;//计算成功接收数据帧次数
u8 confidence;
u16 distance,noise,reftof;
u32 peak,intg;
u8 dis;
extern LidarPointTypedef Pack_sum;     /* 输出结果储存 */

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组，即优先级分级个数
	                                                //NVIC_PriorityGroup_2，代表抢占优先级位数位2，可以分[0, 1, 2, 3]四级优先级
	                                                //                          响应优先级位数位2，可以分[0, 1, 2, 3]四级优先级
	delay_init();//延时初始化
	
	uart_init(115200);//串口1初始化，用于输出接收数据帧成功的次数
	/*关闭JTAG接口*/
	JTAG_Set(JTAG_SWD_DISABLE);    

	/*打开SWD接口 可以利用主板的SWD接口调试*/
	JTAG_Set(SWD_ENABLE);           

	OLED_Init();
	uart3_init(230400);//雷达串口接收
	while(1)
	{
		printf("成功接收%d次\r\n",receive_cnt);
		printf("距离为 %dmm\r\n",distance);
		printf("环境噪音为 %d\r\n",noise);
		printf("接收强度信息为 %d\r\n",peak);
		printf("置信度为 %d\r\n",confidence);
		printf("积分次数 %d\r\n",intg);
		printf("温度表征值为 %d\r\n",reftof);
		delay_ms(10);
		//oled显示
		OLED_SHOW();
		delay_ms(10);
		
	}
}

/*  oled显示  */
void OLED_SHOW(void) 
{
		OLED_ShowString(0,0,"Distance:");			/* 显示距离 */
		OLED_ShowString(110,0,"mm"),
		OLED_ShowNumber(68,0,distance,5,12);
		OLED_ShowString(0,10,"noise:");				/* 显示环境噪音 */
		OLED_ShowNumber(68,10,noise,5,12);
		OLED_ShowString(0,20,"peak:");				/* 显示接收强度信息 */
		OLED_ShowNumber(68,20,peak,5,12);
		OLED_ShowString(0,30,"con:");				  /* 置信度 */
		OLED_ShowNumber(68,30,confidence,5,12);
		OLED_ShowString(0,40,"intg:");				/* 积分次数 */
		OLED_ShowNumber(68,40,intg,5,12);
		OLED_ShowString(0,50,"reftof:");			/* 温度表征值 */
		OLED_ShowNumber(68,50,reftof,5,12);
		OLED_Refresh_Gram();	
}