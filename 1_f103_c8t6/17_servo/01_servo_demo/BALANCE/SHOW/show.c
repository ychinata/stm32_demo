#include "show.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/

/**************************************************************************
函数功能：虚拟示波器往上位机发送数据 关闭显示屏
入口参数：无
返回  值：无
作    者：平衡小车之家
**************************************************************************/
void DataScope(void)
{   
	int Send_Count,i;//计数需要的变量
	
	DataScope_Get_Channel_Data( Position, 1 );      
	DataScope_Get_Channel_Data( Target_Position, 2 );      
//	DataScope_Get_Channel_Data( 0, 3 );              
//	DataScope_Get_Channel_Data( 0 , 4 );   
//	DataScope_Get_Channel_Data(0, 5 ); //用您要显示的数据替换0就行了
//	DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
//	DataScope_Get_Channel_Data(0, 7 );
//	DataScope_Get_Channel_Data( 0, 8 ); 
//	DataScope_Get_Channel_Data(0, 9 );  
//	DataScope_Get_Channel_Data( 0 , 10);//一共可以打印10个数据查看波形
	Send_Count = DataScope_Data_Generate(2);//打印几个数据就在这里改为几
	for( i = 0 ; i < Send_Count; i++) 
	{
	while((USART1->SR&0X40)==0);  
	USART1->DR = DataScope_OutPut_Buffer[i]; 
	}
}

