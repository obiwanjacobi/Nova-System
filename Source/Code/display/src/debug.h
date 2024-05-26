#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "avr/DigitalOutputPin.h"
#include "util/delay.h"

typedef ATL::MCU::DigitalOutputPin<ATL::MCU::PortB, ATL::MCU::Pin5> LEDPIN;

extern LEDPIN led;

#endif //__DEBUG_H__