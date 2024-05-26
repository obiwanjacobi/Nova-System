# MCU

ATMEGA168-20AU

ISP Header on UI board

- 1 pin 16 MISO - Bridge J4!
- 2 pin 6 VCC
- 3 pin 17 SCK
- 4 pin 15 MOSI
- 5 pin 29 RESET
- 6 pin 3,5,21 GND

> `avrdude -c arduino_as_isp -p m168 -U mem:op:file[:fmt]`
> `avrdude -c atmelice_isp -p m168 -U flash:r:flash.bin:r`
