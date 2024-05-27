#ifndef __SPI_H__
#define __SPI_H__

#include "avr/PowerReduction.h"
#include "avr/io.h"
#include <stdint.h>

namespace ATL
{

    /**
     * This class implements a SPI interface.
     * It assumes only one SPI peripheral is available on the AVR MCU.
     */
    // When Pin SS is set as an output then it is not part of SPI
    // template <typename SckPinT, typename MosiPinT, typename MisoPinT>
    class Spi
    {
    public:
        enum Role
        {
            Master,
            Slave
        };

        enum ClockPrescaler
        {
            ClockBy2,
            ClockBy4,
            ClockBy8,
            ClockBy16,
            ClockBy32,
            ClockBy64,
            ClockBy128
        };

        enum ClockPolarity
        {
            RisingEdge,
            FallingEdge
        };

        enum DataOrder
        {
            MsbFirst,
            LsbFirst
        };

        Spi(Role role)
        {
            SetRole(role);
        }

        inline void SetRole(Role role)
        {
            setControlBit(role == Role::Master, (1 << MSTR));
        }

        inline Role getRole() const
        {
            // read MSTR from SPCR
            return ControlReg() & (1 << MSTR)
                       ? Role::Master
                       : Role::Slave;
        }

        inline void Enable(bool enable = true)
        {
            // set PRSPI bit in PRR (power reduction register) to enable SPI
            ATL::MCU::PowerReduction::setEnableSpi(enable);
            // set SPE in SPSR (SPI enable)
            setControlBit(enable, (1 << SPE));
        }

        /**
         * Set clock polarity and phase.
         * \param clock RisingEdge for a clock that is low in idle and FallingEdge for an inverted clock.
         * \param sample is the clock edge when the data is sampled.
         */
        inline void setClockPolarity(ClockPolarity clock, ClockPolarity sample)
        {
            setControlBit(clock == ClockPolarity::FallingEdge, (1 << CPOL));
            if (clock == ClockPolarity::RisingEdge)
                setControlBit(sample == ClockPolarity::FallingEdge, (1 << CPHA));
            else
                setControlBit(sample == ClockPolarity::RisingEdge, (1 << CPHA));
        }

        inline void setClockPrescaler(ClockPrescaler prescaler)
        {
            switch (prescaler)
            {
            case ClockPrescaler::ClockBy2:
                if (getRole() == Role::Master)
                    setPrescaler(false, false, true);
                else // slave only work with clk/4 and lower
                    setPrescaler(false, false, false);
                break;
            case ClockPrescaler::ClockBy4:
                setPrescaler(false, false, false);
                break;
            case ClockPrescaler::ClockBy8:
                setPrescaler(true, false, true);
                break;
            case ClockPrescaler::ClockBy16:
                setPrescaler(true, false, false);
                break;
            case ClockPrescaler::ClockBy32:
                setPrescaler(false, true, true);
                break;
            default:
            case ClockPrescaler::ClockBy64:
                setPrescaler(false, true, false);
                // setPrscaler(true, true, true);
                break;
            case ClockPrescaler::ClockBy128:
                setPrescaler(true, true, false);
                break;
            }
        }

        inline ClockPrescaler getClockPrescaler()
        {
            // TODO
            return ClockPrescaler::ClockBy64;
        }

        inline void setDataOrder(DataOrder order)
        {
            // set DORD in SPCR (data order)
            setControlBit(order == DataOrder::LsbFirst, (1 << DORD));
        }

        inline DataOrder getDataOrder() const
        {
            // read DORD from SPCR (data order)
            return ControlReg() & (1 << DORD)
                       ? DataOrder::LsbFirst
                       : DataOrder::MsbFirst;
        }

        inline bool hasDataCollision() const
        {
            // read from SPSR (SPI status register)
            return (StatusReg() & (1 << WCOL)) != 0;
        }

        inline void Write(uint8_t data)
        {
            // write data to SPDR (SPI data register)
            DataReg() = data;
        }

        inline uint8_t Read()
        {
            // read data from SPDR (SPI data register)
            return DataReg();
        }

        inline void setEnableInterrupt(bool enable = true)
        {
            setControlBit(enable, (1 << SPIE));
        }

        /**
         * Only use this function when you do not use the SPI interrupt (ISR(SPI_STC_vect)).
         * Blocking function that waits for the data transfer to complete.
         */
        inline void WaitForCompletion()
        {
            // wait for completion
            while ((StatusReg() & (1 << SPIF)) == 0)
            {
            }
        }

    private:
        inline static void setPrescaler(bool spr0, bool spr1, bool spi2x)
        {
            setControlBit(spr0, (1 << SPR0));
            setControlBit(spr1, (1 << SPR1));
            uint8_t bit = (1 << SPI2X);
            if (spi2x)
                StatusReg() |= bit;
            else
                StatusReg() &= ~bit;
        }

        inline static void setControlBit(bool set, uint8_t bit)
        {
            if (set)
                ControlReg() |= bit;
            else
                ControlReg() &= ~bit;
        }

        // SPCR (SPI control register)
        inline static volatile uint8_t &ControlReg()
        {
            return SPCR;
        }
        // SPSR (SPI status register)
        inline static volatile uint8_t &StatusReg()
        {
            return SPSR;
        }
        // SPDR (SPI data register)
        inline static volatile uint8_t &DataReg()
        {
            return SPDR;
        }
    };

}
#endif //__SPI_H__
