#include "display.h"
#include "debug.h"
#include "LedTimeout.h"
#include "SwitchesAndLeds.h"

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
SwitchesAndLeds switchesAndLeds;

typedef Scheduler<Time<Milliseconds>, 4> TaskScheduler;
TimeoutTask2<LedTimeout<Ports::PortB, Pins::Pin2>, TaskScheduler, 100, 900> LedTask;

int main()
{
    sei();
    TimerCounter::Start();
    switchesAndLeds.Initialize();
    InitDisplay();
    // TestDisplay();

    // switchesAndLeds.SetSwitchLed(SwitchLed::Bottom6, true);
    //  switchesAndLeds.TestPattern();

    while (true)
    {
        TaskScheduler::Update();

        LedTask.Execute();

        switchesAndLeds.Transfer();
        switchesAndLeds.SetSwitchedLeds();

        switchesAndLeds.EnableOutput(true);
        _delay_ms(100);
        switchesAndLeds.EnableOutput(false);
        _delay_ms(900);
    }

    return 0;
}

#include "avr/TimerCounter.cpp"