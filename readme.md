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


## SPI

### MOSI

12 bytes

Foot Leds (IC21)
=> Leds Col (+bottom) 1 (IC4) => 2 (IC2) => 3 (IC1) => 4 (IC5) => 5 (IC3) => 6 (IC7) => 7 (IC6)
=> 7seg 1xx (IC11) => 7seg 1x (IC10) => 7seg (IC9)
=> Leds Top Right (IC8)

### MISO

1 byte
Rotary Encoders (IC24)

3 bytes
Foot Switches (IC27) => Switches Top row (IC26) => Switches Bottom row (IC25)
