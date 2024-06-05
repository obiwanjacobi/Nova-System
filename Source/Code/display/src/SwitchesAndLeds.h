#ifndef __SWITCHES_H__
#define __SWITCHES_H__

#include "display.h"

#include "Selector.h"
#include "Rotary_Controller.h"
#include "Rotary_Driver.h"
#include "Rotary_Value.h"

#include "Array.h"
#include "Delay.h"
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
enum Encoder
{
    Encoder0,
    Encoder1,
    Encoder2,
    Encoder3
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
// digit segments:    A
// 0 - DP             -
// 1 - C          F |  | B
// 2 - B             -   G
// 3 - A         E |  | C
// 4 - G            -   . DP
// 5 - F            D
// 6 - E
// 7 - D
static const uint8_t DigitSegTable[10] = {
    0b11101110, 0b00000110, 0b11011100, 0b10011110, 0b00110110,
    0b10111010, 0b11111000, 0b00001110, 0b11111110, 0b10111110};

typedef Rotary_Controller<Rotary_Value<Rotary_DriverManual, uint16_t, 999>> RotaryEncoder;

// TODO: Foot switches and leds

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
        _spi.setClockPrescaler(Spi::ClockPrescaler::ClockBy8);
        _spi.setDataOrder(Spi::DataOrder::MsbFirst);
        _spi.Enable(true);
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

    inline void PrintDigit(Digit digit, uint8_t value)
    {
        if (value > 9)
            SetDigit(digit, 0x10);
        SetDigit(digit, DigitSegTable[value]);
    }

    inline void PrintDigits(uint16_t value)
    {
        if (value >= 1000)
        {
            SetDigit(Digit1, 0x10);
            SetDigit(Digit10, 0x10);
            SetDigit(Digit100, 0x10);
            return;
        }

        PrintDigit(Digit100, value / 100);
        PrintDigit(Digit10, value / 10 % 10);
        PrintDigit(Digit1, value % 10);
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

            Delay<TimeResolution::Microseconds>::Wait(10);
        }

        _ledStrobe.Write(true);
        _ledStrobe.Write(false);
        Selector::Enable(false);
    }

    inline void EnableOutput(bool enable)
    {
        _ledOE.Write(enable);
    }

    inline void ReadRotaryEncoders()
    {
        Selector::Select(Selector::Address::RotaryEncoders);
        Selector::Enable(true);

        _spi.Write(0);
        _spi.WaitTransferComplete();
        Delay<TimeResolution::Microseconds>::Wait(10);
        uint8_t data = _spi.Read();

        Selector::Enable(false);

        _rotary0.setState(data & 0x10, data & 0x20);
        _rotary1.setState(data & 0x01, data & 0x02);
        _rotary2.setState(data & 0x04, data & 0x08);
        _rotary3.setState(data & 0x40, data & 0x80);
    }

    inline uint16_t getRotaryValue0() const
    {
        return _rotary0.getValue();
    }
    inline uint16_t getRotaryValue1() const
    {
        return _rotary1.getValue();
    }
    inline uint16_t getRotaryValue2() const
    {
        return _rotary2.getValue();
    }
    inline uint16_t getRotaryValue3() const
    {
        return _rotary3.getValue();
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

    RotaryEncoder _rotary0;
    RotaryEncoder _rotary1;
    RotaryEncoder _rotary2;
    RotaryEncoder _rotary3;
};

#endif // __SWITCHES_H__