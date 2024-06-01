#include "display.h"
#include "testSPI.h"
#include "debug.h"

#include "avr/Port.h"
#include "util/delay.h"
#include <stdint.h>

using namespace ATL;
using namespace ATL::MCU;

LEDPIN led;

int main()
{
    InitDisplay();
    // TestDisplay();

    _delay_ms(1000);
    TestSPI();

    while (true)
    {
        led.Write(true);
        LedOE.Write(true);
        _delay_ms(100);
        led.Write(false);
        LedOE.Write(false);
        _delay_ms(900);
    }

    return 0;
}