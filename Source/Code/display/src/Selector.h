#ifndef __SELECTOR_H__
#define __SELECTOR_H__

#include "avr/DigitalOutputPin.h"
#include "avr/Port.h"

using namespace ATL;
using namespace ATL::MCU;

/** Selects what peripherals to access using the Address enum.
 *  IO is enabled/disabled with Selector::Enable(bool).
 */
class Selector
{
public:
    enum class Address
    {
        Address0 = 0x00, // TBD
        Address1 = 0x01, // TBD
        RotaryEncoders = 0x02,
        SwitchesAndLeds = 0x03,
    };

    inline static void Enable(bool enable = true)
    {
        _enable.Write(!enable);
        if (!enable)
        {
            _address0.Write(false);
            _address1.Write(false);
        }
    }

    inline static void Select(Address address)
    {
        _address0.Write((uint8_t)address & 0x01);
        _address1.Write((uint8_t)address & 0x02);
    }

private:
    Selector(){};
    static DigitalOutputPin<PortB, Pin0> _address0;
    static DigitalOutputPin<PortD, Pin2> _address1;
    static DigitalOutputPin<PortD, Pin4> _enable;
};

DigitalOutputPin<PortB, Pin0> Selector::_address0;
DigitalOutputPin<PortD, Pin2> Selector::_address1;
DigitalOutputPin<PortD, Pin4> Selector::_enable;

#endif // __SELECTOR_H__