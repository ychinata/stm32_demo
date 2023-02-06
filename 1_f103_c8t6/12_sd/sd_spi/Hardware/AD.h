#ifndef __AD_H
#define __AD_H

extern uint16_t AD_Value[4];

void AD_Init(void);
void AD_DMA_Init(void);
uint16_t AD_GetValue(uint8_t adcChannelNo);

#endif
