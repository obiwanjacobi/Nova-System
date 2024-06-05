#ifndef __ROTARYDRIVER_H__
#define __ROTARYDRIVER_H__

#include <stdint.h>

enum RotarySwitchState
{
    None,
    Switch1,
    Switch2,
    Both
};

class Rotary_DriverManual
{
public:
    // call repeatedly to update the state of the switches
    // (just before calling Rotary_Controller.Execute)
    inline void setState(bool switch1, bool switch2)
    {
        _state = (RotarySwitchState)((switch2 << 1) | switch1);
    }

protected:
    inline RotarySwitchState getState() const
    {
        return _state;
    }

private:
    RotarySwitchState _state;
};

#endif //__ROTARYDRIVER_H__