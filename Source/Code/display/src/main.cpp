#include "display.h"
#include "debug.h"
#include "LedTimeout.h"
#include "SwitchesAndLeds.h"
#include "SwitchLedOutput.h"

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
TimeoutTask2<SwitchLedOutput, TaskScheduler, 100, 900> SwitchLedTask;

int main()
{
    sei();
    TimerCounter::Start();
    switchesAndLeds.Initialize();
    SwitchLedTask.SetReference(&switchesAndLeds);

    InitDisplay();
    // TestDisplay();

    switchesAndLeds.PrintDigits(1000);

    switchesAndLeds.SetBarLed(Bar2, Led0, true);
    switchesAndLeds.SetBarLed(Bar3, Led3, true);

    while (true)
    {
        TaskScheduler::Update();

        LedTask.Execute();
        SwitchLedTask.Execute();

        switchesAndLeds.Transfer();
        switchesAndLeds.SetSwitchedLeds();
    }

    return 0;
}

#include "avr/TimerCounter.cpp"