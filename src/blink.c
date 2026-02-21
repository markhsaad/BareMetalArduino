/*
* [A] ATmega4809 Datasheet:
* https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf
*
* [B] Arduino Uno WiFi Rev2 Pinout:
* https://docs.arduino.cc/resources/pinouts/ABX00021-full-pinout.pdf
*/

#define PIN 6 // [B] P1

#include <avr/io.h>
#include <util/delay.h>

int main() {
    // [A] P33 ?
    CPU_CCP = CCP_IOREG_gc;
    
    // [A] P92
    CLKCTRL.MCLKCTRLB = 0;

    // [A] P148
    PORTD.DIRSET = PORTD.DIRSET | (1 << PIN);
    while(1) {
        // [A] P154
        PORTD.OUTTGL = PORTD.OUTTGL | (1 << PIN);
        _delay_ms(1000); 
    }
}
