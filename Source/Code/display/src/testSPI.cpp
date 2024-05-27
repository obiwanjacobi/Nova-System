#include "Spi.h"
#include "debug.h"
#include "avr/DigitalOutputPin.h"
#include "avr/DigitalInputPin.h"
#include "avr/Port.h"
#include "display.h"

using namespace ATL;
using namespace ATL::MCU;

Spi spi(Spi::Role::Master);

DigitalOutputPin<PortB, Pin5> SpiSck;
DigitalInputPin<PortB, Pin4> SpiMiso;
DigitalOutputPin<PortB, Pin3> SpiMosi;

void TestSPI()
{
    led.Write(false);

    spi.setClockPolarity(Spi::ClockPolarity::RisingEdge, Spi::ClockPolarity::RisingEdge);
    spi.setClockPrescaler(Spi::ClockPrescaler::ClockBy16);
    spi.Enable(true);

    spi.Write(0xAA);
    spi.WaitForCompletion();

    uint8_t data = spi.Read();
    PrintBinary(0, data);
    if (data != 0xAA)
        led.Write(true);

    spi.Write(0x55);
    spi.WaitForCompletion();

    data = spi.Read();
    PrintBinary(1, data);
    if (data != 0x55)
        led.Write(true);
}
