#ifndef __SWITCHES_H__
#define __SWITCHES_H__

#include "Array.h"
#include "Delay.h"
#include "Selector.h"
#include "Spi.h"
#include "Time.h"
#include "avr/Delay_Avr.h"
#include "avr/Time_Avr.h"
#include "avr/DigitalOutputPin.h"
#include "avr/DigitalInputPin.h"
#include "avr/Port.h"
#include <stdint.h>

using namespace ATL;
using namespace ATL::MCU;

enum SwitchLed
{
    Top0,
    Top1,
    Top2,
    Top3,
    Top4,
    Top5,
    Top6,
    Top7,
    Bottom0,
    Bottom1,
    Bottom2,
    Bottom3,
    Bottom4,
    Bottom5,
    Bottom6,
    SwitchLedCount
};
enum Bar
{
    Bar0,
    Bar1,
    Bar2,
    Bar3,
    Bar5,
    Bar6,
    Bar7,
};
enum Led
{
    Led0,
    Led1,
    Led2,
    Led3,
    Led4,
    Led5,
    // Led6,
    // Led7,
};
enum Digit
{
    Digit1,
    Digit10,
    Digit100
};

// Top: LD4, 58, 6, 9, 10, 56, 55, 57
// bottom: LD1, 2, 5, 3, 7, 8, 11

static const uint8_t SwitchIndexTable[SwitchLed::SwitchLedCount] = {
    1, 2, 1, 2, 2, 2, 2, 2,
    1, 2, 1, 1, 1, 1, 2};

static const uint8_t SwitchBitTable[SwitchLed::SwitchLedCount] = {
    2, 7, 4, 6, 5, 2, 3, 1,
    0, 0, 3, 1, 5, 6, 4};

static const uint8_t SwitchLedIndexTable[SwitchLed::SwitchLedCount] = {
    8, 7, 6, 5, 4, 0, 0, 0, 10, 9, 8, 7, 6, 5, 4};

static const uint8_t SwitchLedBitTable[SwitchLed::SwitchLedCount] = {
    0, 0, 0, 0, 0, 1, 2, 0, 7, 7, 7, 7, 7, 7, 7};

// digit index: 3, 2, 1

/** Manages the switches and LEDs.
 * \BaseT implements the swicth handlers.
 */
class SwitchesAndLeds
{
public:
    SwitchesAndLeds()
        : _spi(Spi::Role::Master), _spiSck(false), _spiMosi(false), _ledOE(false), _ledStrobe(false)
    {
        Selector::Enable(false);
    }

    inline void Initialize()
    {
        _spi.setClockPolarity(Spi::ClockPolarity::RisingEdge, Spi::ClockPolarity::RisingEdge);
        _spi.setClockPrescaler(Spi::ClockPrescaler::ClockBy4);
        _spi.setDataOrder(Spi::DataOrder::MsbFirst);
        _spi.Enable(true);
    }

    inline void TestPattern()
    {
        _leds.SetAt(0, 0);
        _leds.SetAt(1, 1);
        _leds.SetAt(2, 1);
        _leds.SetAt(3, 1);
        _leds.SetAt(4, 0);
        _leds.SetAt(5, 0);
        _leds.SetAt(6, 0);
        _leds.SetAt(7, 0);
        _leds.SetAt(8, 0);
        _leds.SetAt(9, 0x70);
        _leds.SetAt(10, 0b01100000);
        _leds.SetAt(11, 0);
    }

    inline void SetSwitchedLeds()
    {
        for (uint8_t i = 0; i < _switches.getCapacity(); i++)
        {
            uint8_t sw = _switches.GetAt(i);
            for (uint8_t j = 0; j < 8; j++)
            {
                SwitchLed led = GetSwitchLed(i, j);
                bool value = sw & (1 << j);
                SetSwitchLed(led, value);
            }
        }
    }

    inline SwitchLed GetSwitchLed(uint8_t index, uint8_t bit)
    {
        for (uint8_t i = 0; i < SwitchLed::SwitchLedCount; i++)
        {
            if (SwitchIndexTable[i] == index && SwitchBitTable[i] == bit)
            {
                return (SwitchLed)i;
            }
        }

        return SwitchLed::SwitchLedCount;
    }

    inline void SetSwitchLed(SwitchLed led, bool value)
    {
        SetSwitchLed(SwitchLedIndexTable[(int16_t)led], SwitchLedBitTable[(int16_t)led], value);
    }

    inline void SetSwitchLed(uint8_t index, uint8_t bit, bool value)
    {
        uint8_t byte = _leds.GetAt(index);
        if (value)
            _leds.SetAt(index, byte | (1 << bit));
        else
            _leds.SetAt(index, byte & ~(1 << bit));
    }

    inline bool GetSwitch(SwitchLed sw)
    {
        uint8_t value = _switches.GetAt(SwitchIndexTable[(int16_t)sw]);
        return value & (1 << SwitchBitTable[(int16_t)sw]);
    }

    inline void SetBarLed(Bar bar, Led led, bool value)
    {
        uint8_t index = 10 - (uint8_t)bar;
        SetSwitchLed(index, 6 - led, value);
    }

    inline void SetDigit(Digit digit, uint8_t value)
    {
        uint8_t index = (uint8_t)digit + 1;
        _leds.SetAt(index, value);
    }

    inline void Transfer()
    {
        Selector::Select(Selector::Address::SwitchesAndLeds);
        Selector::Enable(true);

        for (uint8_t i = 0; i < _leds.getCapacity(); i++)
        {
            uint8_t val = _leds.GetAt(i);
            _spi.Write(~val);
            _spi.WaitTransferComplete();

            if (i < _switches.getCapacity())
            {
                val = _spi.Read();
                _switches.SetAt(i, ~val);
            }

            Delay<Microseconds>::Wait(10);
        }

        _ledStrobe.Write(true);
        _ledStrobe.Write(false);
        Selector::Enable(false);
    }

    inline void EnableOutput(bool enable)
    {
        _ledOE.Write(enable);
    }

private:
    Spi _spi;
    DigitalOutputPin<PortB, Pin5> _spiSck;
    DigitalInputPin<PortB, Pin4> _spiMiso;
    DigitalOutputPin<PortB, Pin3> _spiMosi;

    Array<uint8_t, 12> _leds;
    Array<uint8_t, 3> _switches;

    DigitalOutputPin<PortC, Pin5> _ledOE;
    DigitalOutputPin<PortC, Pin4> _ledStrobe;
};

#endif // __SWITCHES_H__