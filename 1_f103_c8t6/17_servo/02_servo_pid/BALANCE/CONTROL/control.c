#include "control.h"	
#include "sys.h"
#include "led.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
          TIM1���ƵĶ�ʱ�ж� 
**************************************************************************/
int TIM1_UP_IRQHandler(void)  
{    
    //10ms��ʱ�ж�
    if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)==SET) {   
        TIM_ClearITPendingBit(TIM1,TIM_IT_Update);     //===�����ʱ��1�жϱ�־λ	  
        Key();                                          //===��������Ŀ��ֵ
        Moto=Position_PID(Position,Target_Position);    //===λ��PID������
        Moto=Xianfu(Moto,Target_Velocity);              //===PWM�޷�
        Set_Pwm(Moto);
        Led_Flash(50);

    }       	
    return 0;	  
} 

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int moto)
{
	Position+=moto;
	
	if(Position>1250)Position=1250;
	if(Position<250)Position=250;
	TIM_SetCompare1(TIM2,Position);		
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
int Xianfu(int value,int Amplitude)
{	
	int temp;
	if(value>Amplitude) temp = Amplitude;
	else if(value<-Amplitude) temp = -Amplitude;
	else temp = value;
	return temp;
			
}

/**************************************************************************
�������ܣ������޸�����״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)  
{	
	int tmp,tmp_x,tmp_m,Position_Amplitude=750,Velocity_Amplitude=5; 
	
	tmp=click_N_Double(30);//��ⰴ�� 
	tmp_x=click_X();
	tmp_m=click_M();
	
	
	if(tmp==1)Target_Position+=Position_Amplitude;  //��������λ��
	if(tmp==2)Target_Position-=Position_Amplitude;  //��������λ��
	
	if(Target_Position>1250)Target_Position=1250;
	if(Target_Position<250)Target_Position=250;
		
	if(tmp_x==1) Target_Velocity -=Velocity_Amplitude;
	if(tmp_m==1) Target_Velocity +=Velocity_Amplitude;
	
	if(Target_Velocity>50)Target_Velocity=50;
	if(Target_Velocity<0)Target_Velocity=0;
	
 }

/**************************************************************************
�������ܣ�ȡ����ֵ
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PID (int position,int target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=target-position;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}

