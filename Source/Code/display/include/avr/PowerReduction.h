/*
Arduino Template Library https://github.com/obiwanjacobi/atl
Written by Marc Jacobi
Copyright 2012-2015 All Rights Reserved

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __POWERREDUCTION_H__
#define __POWERREDUCTION_H__

#include <avr\io.h>

namespace ATL
{
    namespace MCU
    {

        class PowerReduction
        {
        public:
            template <const uint8_t UsartId>
            inline static void setEnableUsart(bool enable = true);

            template <const uint8_t UsartId>
            inline static bool getEnableUsart();

            inline static void setEnableSpi(bool enable = true)
            {
                if (enable)
                    regPRR0() &= ~(1 << PRSPI);
                else
                    regPRR0() |= (1 << PRSPI);
            }
            inline static bool getEnableSpi()
            {
                return regPRR0() & (1 << PRSPI);
            }

            inline static void setEnableTimer0(bool enable = true)
            {
                if (enable)
                    regPRR0() &= ~(1 << PRTIM0);
                else
                    regPRR0() |= (1 << PRTIM0);
            }
            inline static bool getEnableTimer0()
            {
                return regPRR0() & (1 << PRTIM0);
            }

            inline static void setEnableTimer1(bool enable = true)
            {
                if (enable)
                    regPRR0() &= ~(1 << PRTIM1);
                else
                    regPRR0() |= (1 << PRTIM1);
            }
            inline static bool getEnableTimer1()
            {
                return regPRR0() & (1 << PRTIM1);
            }

            inline static void setEnableTimer2(bool enable = true)
            {
                if (enable)
                    regPRR0() &= ~(1 << PRTIM2);
                else
                    regPRR0() |= (1 << PRTIM2);
            }
            inline static bool getEnableTimer2()
            {
                return regPRR0() & (1 << PRTIM2);
            }

            inline static void setEnableAdc(bool enable = true)
            {
                if (enable)
                    regPRR0() &= ~(1 << PRADC);
                else
                    regPRR0() |= (1 << PRADC);
            }
            inline static bool getEnableAdc()
            {
                return regPRR0() & (1 << PRADC);
            }

            inline static void setEnableTwi(bool enable = true)
            {
                if (enable)
                    regPRR0() &= ~(1 << PRTWI);
                else
                    regPRR0() |= (1 << PRTWI);
            }
            inline static bool getEnableTwi()
            {
                return regPRR0() & (1 << PRTWI);
            }

        private:
            PowerReduction()
            {
            }

#ifdef PRR0
            inline static volatile uint8_t &regPRR0()
            {
                return PRR0;
            }
#endif
#ifdef PRR
            inline static volatile uint8_t &regPRR0()
            {
                return PRR;
            }
#endif
        };

#ifdef PRUSART0
        template <>
        inline void PowerReduction::setEnableUsart<0>(bool enable)
        {
            if (enable)
            {
                regPRR0() &= ~(1 << PRUSART0);
            }
            else
            {
                regPRR0() |= (1 << PRUSART0);
            }
        }

        template <>
        inline bool PowerReduction::getEnableUsart<0>()
        {
            return (regPRR0() & PRUSART0) == 0;
        }
#endif

#ifdef PRUSART1
        template <>
        inline void PowerReduction::setEnableUsart<1>(bool enable)
        {
            if (enable)
            {
                regPRR0() &= ~(1 << PRUSART1);
            }
            else
            {
                regPRR0() |= (1 << PRUSART1);
            }
        }

        template <>
        inline bool PowerReduction::getEnableUsart<1>()
        {
            return (regPRR0() & PRUSART1) == 0;
        }
#endif

    }
} // ATL::MCU

#endif /* __POWERREDUCTION_H__ */