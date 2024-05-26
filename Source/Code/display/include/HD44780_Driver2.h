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

#ifndef __HD44780_DRIVER2_H__
#define __HD44780_DRIVER2_H__

#include "avr/Port.h"
#include "HD44780_Profile2.h"
#include <stdint.h>

namespace ATL
{

    /** This class implements driver logic for the HD44780 LCD display.
     *  It assumes a write-only (5-R/W connected to GND) 4-bit mode (soft-mode).
     *  Note that in 4-bit mode only the high data lines (4-7) of the display are used.
     *  See also: http://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller
     *  \tparam RegSelPinT is a DigitalOutputPin that is connected to RS of the display.
     *  \tparam ReadWritePinT is a DigitalOutputPin that is connected to R/W of the display.
     *  \tparam EnablePinT is a DigitalOutputPin that is connected to Enable of the display.
     *  \tparam Data04PinT is a DigitalIOPin that is connected to D4 of the display.
     *  \tparam Data15PinT is a DigitalIOPin that is connected to D5 of the display.
     *  \tparam Data26PinT is a DigitalIOPin that is connected to D6 of the display.
     *  \tparam Data37PinT is a DigitalIOPin that is connected to D7 of the display.
     *  It can be used to fine-tune the synchronous delays. See also HD44780_Profile.
     */
    template <typename RegSelPinT, typename ReadWritePinT, typename EnablePinT,
              typename Data04PinT, typename Data15PinT, typename Data26PinT, typename Data37PinT,
              typename TimingProfileT = HD44780_Profile2>
    class HD44780_Driver2
    {
    public:
        /** Initializes the function set.
         */
        inline void Initialize()
        {
            _rs.Write(false);
            _rw.Write(false);
            _enable.Write(false);

            // Follow the initialization sequence on page 46 (figure 24) - HD44780 data sheet
            TimingProfileT::WaitPowerUp();
            Write4(0x03);
            TimingProfileT::WaitInitializeLong();
            Write4(0x03);
            TimingProfileT::WaitInitialize();
            Write4(0x03);
            TimingProfileT::WaitInitializeShort();
            Write4(0x02);
            TimingProfileT::WaitInitializeShort();
            WriteFunctionSet();
        }

        /** Writes a command to the LCD.
         *  Pulls the RS LOW to indicate a command.
         *  \param cmd is the command to write.
         */
        inline void WriteCommand(uint8_t cmd)
        {
            WaitForNotBusy();
            _rs.Write(false);
            Write8(cmd);
        }

        /** Writes data to the LCD.
         *  Pulls the RS HIGH to indicate data.
         *  \param data is the data to write.
         */
        inline void WriteData(uint8_t data)
        {
            WaitForNotBusy();
            _rs.Write(true);
            Write8(data);
        }

    protected:
        /** Writes 8-bits to the display - MSB first.
         *  \param data is all 8 bits.
         */
        inline void Write8(uint8_t data)
        {
            _rw.Write(false);
            _data04.SetDirection(MCU::Output);
            _data15.SetDirection(MCU::Output);
            _data26.SetDirection(MCU::Output);
            _data37.SetDirection(MCU::Output);

            Write4(data >> 4);
            Write4(data);
        }

        /** Writes 4-bits to the display.
         *  Pulses Enable to let the display read the data.
         *  \param data is all 4 bits in the lower nibble.
         */
        inline void Write4(uint8_t data)
        {
            _enable.Write(false);
            _data04.Write(data & 0x01);
            _data15.Write(data & 0x02);
            _data26.Write(data & 0x04);
            _data37.Write(data & 0x08);

            _enable.Write(true);
            TimingProfileT::WaitPulseEnable();
            _enable.Write(false);
        }

        inline uint8_t Read8()
        {
            _rw.Write(true);
            _data04.SetDirection(MCU::Input);
            _data15.SetDirection(MCU::Input);
            _data26.SetDirection(MCU::Input);
            _data37.SetDirection(MCU::Input);

            uint8_t msBits = Read4();
            uint8_t lsBits = Read4();
            return msBits << 4 | lsBits;
        }

        /** Reads 4-bits from the display.
         *  Pulls Enable high to let the display sent the data.
         *  \return all 4 bits in the lower nibble.
         */
        inline uint8_t Read4()
        {
            _enable.Write(true);
            TimingProfileT::WaitReadEnable();

            uint8_t value = _data04.Read() | _data15.Read() << 1 | _data26.Read() << 2 | _data37.Read() << 3;

            _enable.Write(false);
            return value;
        }

        void WaitForNotBusy()
        {
            _rs.Write(false);
            uint8_t value = Read8();
            while (value & 0x80)
            {
                value = Read8();
            }
        }

    private:
        RegSelPinT _rs;
        ReadWritePinT _rw;
        EnablePinT _enable;
        Data04PinT _data04;
        Data15PinT _data15;
        Data26PinT _data26;
        Data37PinT _data37;

        inline void WriteFunctionSet()
        {
            // D7 D6 D5 D4 D3 D2 D1 D0
            //  0  0  1  0  1  0  0  0   => 0x28
            //      cmd 4b 2l  F  *  *
            // cmd = FunctionSet command bit
            // 4b = 4-bit data transfer (DL)
            // 2l = 2 line display (not single line) (N)
            // F = Font not available for multi-line.
            // * = don't care
            // WriteCommand(0x28);   // we don't want to wait for busy here
            //_rs.Write(false);     // already pulled low
            Write8(0x28);
        }
    };

} // ATL

#endif //__HD44780_DRIVER2_H__
