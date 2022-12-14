#ifndef _ONEWIRE_H
#define _ONEWIRE_H

uint8_t OneWire_Init(void);
void OneWire_SendByte(uint8_t Byte);
uint8_t OneWire_ReceiveByte(void);

#endif

