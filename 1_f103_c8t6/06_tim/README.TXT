实验器材:
	无
	
实验目的:
	定时器TIM
	PWM
	

连接管脚：
    LED/PWM <-> PA0
	 <-> PA1
	 <-> PA2
	 <-> PA3
	
	
寄存器配置：
	TIM2
	TIM_Period = 100 - 1;		//ARR
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR,/* 设置比较值,此值用来确定占空比 */
	
实验现象:

	
注意事项:














