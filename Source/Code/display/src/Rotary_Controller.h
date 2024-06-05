#ifndef __ROTARYCONTROLLER_H__
#define __ROTARYCONTROLLER_H__

#include "Rotary_Driver.h"

template <class BaseT>
class Rotary_Controller : public BaseT
{
public:
    // Driver override to also Execute.
    inline void setState(bool switch1, bool switch2)
    {
        BaseT::setState(switch1, switch2);
        Execute();
    }

protected:
    // call this repeatedly to detect the rotary endcoder switches
    inline void Execute()
    {
        RotarySwitchState state = BaseT::getState();
        // if (state != _lastState)
        {
            //bool lastSw1 = _lastState & RotarySwitchState::Switch1;
            bool lastSw2 = _lastState & RotarySwitchState::Switch2;
            bool newSw1 = state & RotarySwitchState::Switch1;
            bool newSw2 = state & RotarySwitchState::Switch2;

            // clockwise (increment)
            // SW1: _|--|__|--|__|--|__|--|_
            // SW2: __|--|__|--|__|--|__|--|_

            if (lastSw2 != newSw2)
            {
                if (newSw1 == newSw2)
                    BaseT::Increment();
                else
                    BaseT::Decrement();

                _lastState = state;
            }
        }
    }

private:
    RotarySwitchState _lastState;
};

#endif //__ROTARYCONTROLLER_H__