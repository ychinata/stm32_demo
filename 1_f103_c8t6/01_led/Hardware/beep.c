#define BEEP_GPIO GPIOA
#define BEEP_PIN  GPIO_Pin_12

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);
	
	while (1)
	{

		GPIO_ResetBits(BEEP_GPIO, BEEP_PIN);
		Delay_ms(100);
		GPIO_SetBits(BEEP_GPIO, BEEP_PIN);
		Delay_ms(100);
		GPIO_ResetBits(BEEP_GPIO, BEEP_PIN);
		Delay_ms(100);
		GPIO_SetBits(BEEP_GPIO, BEEP_PIN);
		Delay_ms(700);

        
        //GPIO_ResetBits(BEEP_GPIO, BEEP_PIN);
	}
}