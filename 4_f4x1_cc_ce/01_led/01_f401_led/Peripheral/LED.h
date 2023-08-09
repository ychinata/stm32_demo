#ifndef __LED_H
#define __LED_H
#include "stm32f4xx.h"
#include "sys.h"

//LED¶Ë¿Ú¶¨Òå
#define LED0 PCout(13)	// LED0
 

void LED_Init(void);

#endif
