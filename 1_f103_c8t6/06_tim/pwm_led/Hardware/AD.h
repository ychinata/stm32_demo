#ifndef __AD_H
#define __AD_H

extern uint16_t AD_Value[4];

void AD_Init(void);
void AD_DMA_Init(void);
uint16_t AD_GetValue(uint8_t adcChannelNo);
u16 AD_GetAverageValue(u8 adcChannelNo, u8 times);

#endif
