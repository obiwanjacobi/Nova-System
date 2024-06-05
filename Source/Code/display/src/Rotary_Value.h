#ifndef __ROTARYVALUE_H__
#define __ROTARYVALUE_H__

#include <stdint.h>

template <class BaseT, typename ValueT, const uint32_t MaxValue>
class Rotary_Value : public BaseT
{
public:
    inline void setValue(ValueT value)
    {
        _value = value;
    }
    inline ValueT getValue() const
    {
        return _value;
    }

protected:
    inline void Increment()
    {
        if (_value < MaxValue)
            _value++;
    }
    inline void Decrement()
    {
        if (_value > 0)
            _value--;
    }

private:
    ValueT _value;
};

#endif //__ROTARYVALUE_H__