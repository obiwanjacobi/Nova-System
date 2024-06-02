#include "Spi.h"
#include "debug.h"
#include "avr/DigitalOutputPin.h"
#include "avr/DigitalInputPin.h"
#include "avr/Port.h"
#include "Array.h"
#include "display.h"

using namespace ATL;
using namespace ATL::MCU;

// Spi spi(Spi::Role::Master);

// DigitalOutputPin<PortB, Pin5> SpiSck;
// DigitalInputPin<PortB, Pin4> SpiMiso;
// DigitalOutputPin<PortB, Pin3> SpiMosi;

#ifndef TEST_ON_ARDUINO
// DigitalOutputPin<PortB, Pin0> Enable0;
// DigitalOutputPin<PortD, Pin2> Enable1;
// DigitalOutputPin<PortD, Pin4> EnableG;

// DigitalOutputPin<PortC, Pin5> LedOE;
// DigitalOutputPin<PortC, Pin4> LedStrobe;
#endif

#define BufferSize 12
Array<uint8_t, BufferSize> buffer;

void Transfer()
{
    for (uint8_t i = 0; i < BufferSize; i++)
    {
        uint8_t val = buffer.GetAt(i);
        // spi.Write(~val);
        // spi.WaitTransferComplete();
        // val = spi.Read();
        buffer.SetAt(i, ~val);

        _delay_us(10);
    }
}

void TestSPI()
{
    // SpiSck.Write(false);
    // SpiMosi.Write(false);

    led.Write(true);
    _delay_ms(200);

    // LedOE.Write(false);
    // LedStrobe.Write(false);

    // spi.setClockPolarity(Spi::ClockPolarity::RisingEdge, Spi::ClockPolarity::RisingEdge);
    // spi.setClockPrescaler(Spi::ClockPrescaler::ClockBy4);
    // spi.Enable(true);

    // Enable0.Write(true);
    // Enable1.Write(true);
    // EnableG.Write(false);

    // buffer.Clear();
    buffer.SetAt(0, 0);
    buffer.SetAt(1, 1);
    buffer.SetAt(2, 1);
    buffer.SetAt(3, 1);
    buffer.SetAt(4, 0);
    buffer.SetAt(5, 0);
    buffer.SetAt(6, 0);
    buffer.SetAt(7, 0);
    buffer.SetAt(8, 0);
    buffer.SetAt(9, 0x70);
    buffer.SetAt(10, 0b01100000);
    buffer.SetAt(11, 0);
    Transfer();

    uint8_t val = buffer.GetAt(1);
    PrintBinary(0, val);
    val = buffer.GetAt(2);
    PrintBinary(1, val);

    // LedStrobe.Write(true);
    // spi.Enable(false);
    // LedStrobe.Write(false);

    // LedOE.Write(true);
    // LedOE.Write(false);

    led.Write(false);
}
