#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>

void InitDisplay();
void TestDisplay();
void PrintBinary(uint8_t line, uint8_t value);
void PrintChar(uint8_t line, char value);

#endif //__DISPLAY_H__