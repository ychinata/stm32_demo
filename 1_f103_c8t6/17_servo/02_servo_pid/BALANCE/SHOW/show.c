#include "show.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/

/**************************************************************************
�������ܣ�����ʾ��������λ���������� �ر���ʾ��
��ڲ�������
����  ֵ����
��    �ߣ�ƽ��С��֮��
**************************************************************************/
void DataScope(void)
{   
	int Send_Count,i;//������Ҫ�ı���
	
	DataScope_Get_Channel_Data( Position, 1 );      
	DataScope_Get_Channel_Data( Target_Position, 2 );      
//	DataScope_Get_Channel_Data( 0, 3 );              
//	DataScope_Get_Channel_Data( 0 , 4 );   
//	DataScope_Get_Channel_Data(0, 5 ); //����Ҫ��ʾ�������滻0������
//	DataScope_Get_Channel_Data(0 , 6 );//����Ҫ��ʾ�������滻0������
//	DataScope_Get_Channel_Data(0, 7 );
//	DataScope_Get_Channel_Data( 0, 8 ); 
//	DataScope_Get_Channel_Data(0, 9 );  
//	DataScope_Get_Channel_Data( 0 , 10);//һ�����Դ�ӡ10�����ݲ鿴����
	Send_Count = DataScope_Data_Generate(2);//��ӡ�������ݾ��������Ϊ��
	for( i = 0 ; i < Send_Count; i++) 
	{
	while((USART1->SR&0X40)==0);  
	USART1->DR = DataScope_OutPut_Buffer[i]; 
	}
}

