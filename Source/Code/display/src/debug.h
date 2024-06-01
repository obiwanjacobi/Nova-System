#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "avr/DigitalOutputPin.h"
#include "util/delay.h"

// use test hardware
// #define TEST_ON_ARDUINO

#ifdef TEST_ON_ARDUINO
typedef ATL::MCU::DigitalOutputPin<ATL::MCU::PortD, ATL::MCU::Pin2> LEDPIN;
#else
typedef ATL::MCU::DigitalOutputPin<ATL::MCU::PortB, ATL::MCU::Pin2> LEDPIN;
#endif

extern LEDPIN led;

#endif //__DEBUG_H__