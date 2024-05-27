#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "avr/DigitalOutputPin.h"
#include "util/delay.h"

// use test hardware
#define TEST_ON_ARDUINO

typedef ATL::MCU::DigitalOutputPin<ATL::MCU::PortD, ATL::MCU::Pin2> LEDPIN;

extern LEDPIN led;

#endif //__DEBUG_H__