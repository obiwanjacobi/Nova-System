#include "avr/Delay_Avr.h"
#include "avr/DigitalPin.h"
#include "avr/DigitalOutputPin.h"
#include "avr/Port.h"

#include "HD44780_Driver2.h"
#include "HD44780_Controller2.h"
#include "HD44780_DisplayWriter.h"
#include "HD44780_View.h"
#include "TextWriter.h"

using namespace ATL;
using namespace ATL::MCU;

#define LcdColumns 24
#define LcdLines 2

typedef DigitalOutputPin<PortB, Pin1> LCD_CONTRAST;

typedef HD44780_DisplayWriter<
    TextWriter<
        HD44780_View<
            HD44780_Controller2<
                HD44780_Driver2<
                    DigitalOutputPin<PortD, Pin5>, // RS
                    DigitalOutputPin<PortD, Pin6>, // R/W
                    DigitalOutputPin<PortD, Pin7>, // E
                    DigitalPin<PortC, Pin0>,       // D0/4
                    DigitalPin<PortC, Pin1>,       // D1/5
                    DigitalPin<PortC, Pin2>,       // D2/6
                    DigitalPin<PortC, Pin3>        // D3/7
                    >>,
            LcdLines,
            LcdColumns>>>
    LCD2;

LCD2 lcd;

void InitDisplay()
{
    lcd.Initialize();
    lcd.SetDisplayControl(true, false, false);
}

void PrintDecimal(uint8_t line, uint8_t value)
{
    lcd.GoTo(line, 0);
    lcd.getBaseRef().Write(value);
}

void PrintBinary(uint8_t line, uint8_t value)
{
    lcd.GoTo(line, 0);

    uint8_t mask = 0x80;
    char buffer[2];
    buffer[1] = 0;

    for (int n = 0; n < 8; n++)
    {
        buffer[0] = value & mask ? '1' : '0';
        lcd.Write(buffer);
        mask >>= 1;
    }
}

void PrintChar(uint8_t line, const char value)
{
    lcd.GoTo(line, DisplayWriter::DontCare);
    lcd.getBaseRef().Write(value);
}

void TestDisplay()
{
    InitDisplay();
    lcd.WriteLine("Hello World!");
}

#include "TextFormatInfo.cpp"