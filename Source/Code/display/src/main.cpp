#include "display.h"

#include "debug.h"
#include "avr/Port.h"
#include "util/delay.h"
#include <stdint.h>

using namespace ATL;
using namespace ATL::MCU;

LEDPIN led(false);

int main()
{
    //_delay_ms(200);

    TestDisplay();

    while (true)
    {
        // led.Write(true);
        // _delay_ms(200);
        // led.Write(false);
        // _delay_ms(800);
    }

    return 0;
}