#include "avr/Delay_Avr.h"
#include "avr/DigitalIOPin.h"
#include "avr/DigitalOutputPin.h"
#include "avr/Port.h"

#include "HD44780_Driver2.h"
#include "HD44780_Controller2.h"
#include "HD44780_Driver.h"
#include "HD44780_Controller.h"
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
                    DigitalIOPin<PortC, Pin0>,     // D0/4
                    DigitalIOPin<PortC, Pin1>,     // D1/5
                    DigitalIOPin<PortC, Pin2>,     // D2/6
                    DigitalIOPin<PortC, Pin3>      // D3/7
                    >>,
            LcdLines,
            LcdColumns>>>
    LCD2;

// typedef HD44780_DisplayWriter<
//     TextWriter<
//         HD44780_View<
//             HD44780_Controller<
//                 HD44780_Driver<
//                     DigitalOutputPin<PortD, Pin5>, // RS
//                     DigitalOutputPin<PortD, Pin7>, // E
//                     DigitalOutputPin<PortC, Pin0>, // D0/4
//                     DigitalOutputPin<PortC, Pin1>, // D1/5
//                     DigitalOutputPin<PortC, Pin2>, // D2/6
//                     DigitalOutputPin<PortC, Pin3>  // D3/7
//                     >>,
//             LcdLines,
//             LcdColumns>>>
//     LCD;

// typedef HD44780_DisplayWriter<
//     TextWriter<
//         HD44780_View<
//             HD44780_Controller2<
//                 HD44780_Driver2<
//                     DigitalOutputPin<PortB, Pin4>, // RS
//                     DigitalOutputPin<PortB, Pin3>, // R/W
//                     DigitalOutputPin<PortB, Pin2>, // E
//                     DigitalIOPin<PortD, Pin4>,     // D0/4
//                     DigitalIOPin<PortD, Pin5>,     // D1/5
//                     DigitalIOPin<PortD, Pin6>,     // D2/6
//                     DigitalIOPin<PortD, Pin7>      // D3/7
//                     >>,
//             2,
//             16>>>
//     LCD2;

// typedef HD44780_DisplayWriter<
//     TextWriter<
//         HD44780_View<
//             HD44780_Controller<
//                 HD44780_Driver<
//                     DigitalOutputPin<PortB, Pin4>, // RS
//                     DigitalOutputPin<PortB, Pin2>, // E
//                     DigitalIOPin<PortD, Pin4>,     // D0/4
//                     DigitalIOPin<PortD, Pin5>,     // D1/5
//                     DigitalIOPin<PortD, Pin6>,     // D2/6
//                     DigitalIOPin<PortD, Pin7>      // D3/7
//                     >>,
//             2,
//             16>>>
//     LCD;

LCD2 lcd;

void TestDisplay()
{
    lcd.Initialize();
    lcd.SetDisplayControl(true, false, false);
    lcd.WriteLine("Hello World!");
}

#include "TextFormatInfo.cpp"