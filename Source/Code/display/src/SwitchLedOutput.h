#ifndef __SWITCHLEDOUTPUT_H__
#define __SWITCHLEDOUTPUT_H__

#include "SwitchesAndLeds.h"

class SwitchLedOutput
{
public:
    inline void SetReference(SwitchesAndLeds *switchesAndLeds)
    {
        _switchesAndLeds = switchesAndLeds;
    }

protected:
    inline uint16_t getId() const
    {
        return (uint16_t)this;
    }

    void OnTimeout1()
    {
        if (_switchesAndLeds)
            _switchesAndLeds->EnableOutput(false);
    }
    void OnTimeout2()
    {
        if (_switchesAndLeds)
            _switchesAndLeds->EnableOutput(true);
    }

private:
    SwitchesAndLeds *_switchesAndLeds = nullptr;
};

#endif //__SWITCHLEDOUTPUT_H__