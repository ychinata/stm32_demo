#ifndef __LED_H
#define __LED_H
#include "sys.h"
 /**************************************************************************
���ߣ�ƽ��С��֮�� 
�Ա����̣�http://shop114407458.taobao.com/
**************************************************************************/
//LED �˿ڶ���
#define LED PAout(4) // PA4

void Led_Flash(u16 time);
void LED_Init(void);
#endif