#include "display.h"
#include "testSPI.h"
#include "debug.h"
#include "LedTimeout.h"

#include "Time.h"
#include "TimeoutTask2.h"
#include "Scheduler.h"
#include "avr/Port.h"
#include "avr/Time_Avr.h"

#include "util/delay.h"
#include <avr/interrupt.h>
#include <stdint.h>

using namespace ATL;
using namespace ATL::MCU;

LEDPIN led;

typedef Scheduler<Time<Milliseconds>, 4> TaskScheduler;

TimeoutTask2<LedTimeout<Ports::PortB, Pins::Pin2>, TaskScheduler, 100, 900> LedTask;

int main()
{
    sei();
    TimerCounter::Start();

    InitDisplay();
    // TestDisplay();

    _delay_ms(1000);
    // TestSPI();

    while (true)
    {
        uint32_t delta = TaskScheduler::Update();

        LedTask.Execute();

        // led.Write(true);
        // LedOE.Write(true);
        //_delay_ms(100);
        // led.Write(false);
        // LedOE.Write(false);
        //_delay_ms(900);
    }

    return 0;
}

#include "avr/TimerCounter.cpp"