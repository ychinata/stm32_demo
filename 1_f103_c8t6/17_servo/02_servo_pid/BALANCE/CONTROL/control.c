#include "control.h"	
#include "sys.h"
#include "led.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
函数功能：所有的控制代码都在这里面
          TIM1控制的定时中断 
**************************************************************************/
int TIM1_UP_IRQHandler(void)  
{    
    //10ms定时中断
    if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)==SET) {   
        TIM_ClearITPendingBit(TIM1,TIM_IT_Update);     //===清除定时器1中断标志位	  
        Key();                                          //===按键控制目标值
        Moto=Position_PID(Position,Target_Position);    //===位置PID控制器
        Moto=Xianfu(Moto,Target_Velocity);              //===PWM限幅
        Set_Pwm(Moto);
        Led_Flash(50);

    }       	
    return 0;	  
} 

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto)
{
	Position+=moto;
	
	if(Position>1250)Position=1250;
	if(Position<250)Position=250;
	TIM_SetCompare1(TIM2,Position);		
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
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
函数功能：按键修改运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)  
{	
	int tmp,tmp_x,tmp_m,Position_Amplitude=750,Velocity_Amplitude=5; 
	
	tmp=click_N_Double(30);//检测按键 
	tmp_x=click_X();
	tmp_m=click_M();
	
	
	if(tmp==1)Target_Position+=Position_Amplitude;  //单击增加位置
	if(tmp==2)Target_Position-=Position_Amplitude;  //单击增加位置
	
	if(Target_Position>1250)Target_Position=1250;
	if(Target_Position<250)Target_Position=250;
		
	if(tmp_x==1) Target_Velocity -=Velocity_Amplitude;
	if(tmp_m==1) Target_Velocity +=Velocity_Amplitude;
	
	if(Target_Velocity>50)Target_Velocity=50;
	if(Target_Velocity<0)Target_Velocity=0;
	
 }

/**************************************************************************
函数功能：取绝对值
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_PID (int position,int target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=target-position;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias/100+Position_KI*Integral_bias/100+Position_KD*(Bias-Last_Bias)/100;       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}

