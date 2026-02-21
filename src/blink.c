/*
* [A] Arduino Uno WiFi Rev2 Pinout:
* https://docs.arduino.cc/resources/pinouts/ABX00021-full-pinout.pdf
*
* [B] ATmega4809 Datasheet:
* https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf
*
* See also the doc folder.
*/

#include <avr/io.h>
#include <util/delay.h>

// From [A] P1, the built-in LED is connected to PD06 (port D, pin 6) on the MCU.
#define PORT PORTD
#define PIN 6

int main() {
    /*
    * In the compile command, we have the flag -DF_CPU=16000000UL which sets the MCU CPU speed to 16 MHz.
    * However, reading online, it seems that the ATmega4809 has a default divison ratio of 6. This means 
    * that the CPU is running at (16/6) MHz. If we do not account for this, then the command _delay_ms(1000) 
    * will wait for 1 second based on a 16 MHz CPU speed which is really 6 seconds for a (16/6) MHz clock speed.
    *
    * EXPECTED_DELAY_TIME_S = 1
    *
    * EXPECTED_CPU_SPEED = 16000000
    * EXPECTED_CYCLES = EXPECTED_DELAY_TIME_S / (1 / EXPECTED_CPU_SPEED)
    *                 = 1 / (1 / 16000000)
    *                 = 16000000
    *
    * DIVISON_RATIO = 6
    * ACTUAL_CPU_SPEED = EXPECTED_CPU_SPEED / DIVISON_RATIO
    *                  = 16000000 / 6
    *
    * ACTUAL_DELAY_TIME_S = EXPECTED_CYCLES * (1 / ACTUAL_CPU_SPEED)
    *                   = 16000000 * (1 / (16000000 / 6))
    *                   = 6 
    *
    * To fix this, we either need to adjust the compile flag or disable the prescaler. Below, we do the latter.
    */

    // From [B] P33, this briefly unlocks the necessary register to disable the prescaler for modification.
    CPU_CCP = CCP_IOREG_gc;
    
    // From [B] P92, this disables the prescaler.
    CLKCTRL.MCLKCTRLB = 0;

    // From [B] P148, this will set the specified pin on the specified port to be an output.
    PORT.DIRSET = PORT.DIRSET | (1 << PIN);
    while(1) {
        // From [B] P154, this will toggle the specified pin on the specified port.
        PORT.OUTTGL = PORT.OUTTGL | (1 << PIN);
        _delay_ms(1000); 
    }
}
