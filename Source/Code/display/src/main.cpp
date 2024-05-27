#include "display.h"
#include "testSPI.h"
#include "debug.h"

#include "avr/Port.h"
#include "util/delay.h"
#include <stdint.h>

using namespace ATL;
using namespace ATL::MCU;

LEDPIN led(false);

int main()
{

    InitDisplay();

    TestSPI();

    // _delay_ms(2000);

    while (true)
    {
        // led.Write(true);
        // _delay_ms(200);
        // led.Write(false);
        // _delay_ms(800);
    }

    return 0;
}