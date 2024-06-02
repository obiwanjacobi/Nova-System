#ifndef __LEDTIMEOUT_H__
#define __LEDTIMEOUT_H__

#include "TimeoutTask2.h"
#include "IdentifiableObject.h"
#include "avr/DigitalOutputPin.h"

using namespace ATL;
using namespace ATL::MCU;

template <const Ports port, const Pins pin>
class LedTimeout : public IdentifiableObject<DigitalOutputPin<port, pin>>
{
    typedef IdentifiableObject<DigitalOutputPin<port, pin>> BaseT;

public:
protected:
    void OnTimeout()
    {
        BaseT::Write(!BaseT::getValue());
    }
    void OnTimeout1()
    {
        BaseT::Write(false);
    }
    void OnTimeout2()
    {
        BaseT::Write(true);
    }
};

#endif //__LEDTIMEOUT_H__