/*************************************
ʹ�ô���3����STP-23Lģ�����ݣ�ʹ�ô���1������ճɹ��Ĵ�����oled��ʾ����
����˵����
STP-23Lģ��      ��Ƭ���ӿ�
-----------------------------------
   P5V----------------5V	
   GND----------------GND
   TX-----------------B11
   RX----------------����
***************************************/
#include "sys.h"
#include "oled.h"

void OLED_SHOW(void);

u16 receive_cnt;//����ɹ���������֡����
u8 confidence;
u16 distance,noise,reftof;
u32 peak,intg;
u8 dis;
extern LidarPointTypedef Pack_sum;     /* ���������� */

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����ж����ȼ����飬�����ȼ��ּ�����
	                                                //NVIC_PriorityGroup_2��������ռ���ȼ�λ��λ2�����Է�[0, 1, 2, 3]�ļ����ȼ�
	                                                //                          ��Ӧ���ȼ�λ��λ2�����Է�[0, 1, 2, 3]�ļ����ȼ�
	delay_init();//��ʱ��ʼ��
	
	uart_init(115200);//����1��ʼ�������������������֡�ɹ��Ĵ���
	/*�ر�JTAG�ӿ�*/
	JTAG_Set(JTAG_SWD_DISABLE);    

	/*��SWD�ӿ� �������������SWD�ӿڵ���*/
	JTAG_Set(SWD_ENABLE);           

	OLED_Init();
	uart3_init(230400);//�״ﴮ�ڽ���
	while(1)
	{
		printf("�ɹ�����%d��\r\n",receive_cnt);
		printf("����Ϊ %dmm\r\n",distance);
		printf("��������Ϊ %d\r\n",noise);
		printf("����ǿ����ϢΪ %d\r\n",peak);
		printf("���Ŷ�Ϊ %d\r\n",confidence);
		printf("���ִ��� %d\r\n",intg);
		printf("�¶ȱ���ֵΪ %d\r\n",reftof);
		delay_ms(10);
		//oled��ʾ
		OLED_SHOW();
		delay_ms(10);
		
	}
}

/*  oled��ʾ  */
void OLED_SHOW(void) 
{
		OLED_ShowString(0,0,"Distance:");			/* ��ʾ���� */
		OLED_ShowString(110,0,"mm"),
		OLED_ShowNumber(68,0,distance,5,12);
		OLED_ShowString(0,10,"noise:");				/* ��ʾ�������� */
		OLED_ShowNumber(68,10,noise,5,12);
		OLED_ShowString(0,20,"peak:");				/* ��ʾ����ǿ����Ϣ */
		OLED_ShowNumber(68,20,peak,5,12);
		OLED_ShowString(0,30,"con:");				  /* ���Ŷ� */
		OLED_ShowNumber(68,30,confidence,5,12);
		OLED_ShowString(0,40,"intg:");				/* ���ִ��� */
		OLED_ShowNumber(68,40,intg,5,12);
		OLED_ShowString(0,50,"reftof:");			/* �¶ȱ���ֵ */
		OLED_ShowNumber(68,50,reftof,5,12);
		OLED_Refresh_Gram();	
}