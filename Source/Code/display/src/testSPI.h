#ifndef __TESTSPI_H__
#define __TESTSPI_H__

#include "avr/DigitalOutputPin.h"

extern ATL::MCU::DigitalOutputPin<ATL::MCU::PortC, ATL::MCU::Pin5> LedOE;

void TestSPI();

#endif //__TESTSPI_H__