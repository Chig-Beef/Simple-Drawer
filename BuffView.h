#ifndef _BUFF_VIEW_H_
#define _BUFF_VIEW_H_

#include "def.h"

void writeUInt16ToBuff(uint16 i, int index, byte *buff);
void writeUInt32ToBuff(uint32 i, int index, byte *buff);

uint16 readUInt16FromBuff(const int index, const byte *buff);
uint32 readUInt32FromBuff(const int index, const byte *buff);

#endif
