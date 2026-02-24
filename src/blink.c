/*
* [A] Arduino Uno WiFi Rev2 Pinout:
* https://docs.arduino.cc/resources/pinouts/ABX00021-full-pinout.pdf
*
* [B] ATmega4809 Datasheet:
* https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf
*
* See also the doc folder.
*/

/*
* From [B] P55, the CPU speed defaults to 20 MHz. This macro needs to be set for _delay_ms to function properly.
* The more proper way to set it would be as a compiler flag `-DF_CPU=20000000UL` in the avr-gcc call,
* but I have set it here for increased clarity. Note that it is set *before* the includes for headers 
* that may depend on it, like util/delay.h 
*/ 
#define F_CPU 20000000UL

#include <avr/io.h>
#include <util/delay.h>  // Required for _delay_ms 
#include <avr/cpufunc.h> // Required for _PROTECTED_WRITE

// From [A] P1, the built-in LED is connected to PD06 (port D, pin 6) on the MCU.
#define PORT PORTD
#define PIN 6

int main() {
    /*
    * From [B] P92, the divison ratio defaults to 6. This means that the CPU is running at (20/6) MHz. 
    * If we do not account for this, then the command _delay_ms(1000) will wait for 1 second based on 
    * a 20 MHz CPU speed, which is really 6 seconds for a (20/6) MHz clock speed.
    *
    * EXPECTED_DELAY_TIME_S = 1
    *
    * EXPECTED_CPU_SPEED = 20000000
    * EXPECTED_CYCLES = EXPECTED_DELAY_TIME_S / (1 / EXPECTED_CPU_SPEED)
    *                 = 1 / (1 / 20000000)
    *                 = 20000000
    *
    * DIVISON_RATIO = 6
    * ACTUAL_CPU_SPEED = EXPECTED_CPU_SPEED / DIVISON_RATIO
    *                  = 20000000 / 6
    *
    * ACTUAL_DELAY_TIME_S = EXPECTED_CYCLES * (1 / ACTUAL_CPU_SPEED)
    *                   = 20000000 * (1 / (20000000 / 6))
    *                   = 6 
    *
    * One way to fix this is to disable the prescaler, which is done below. See [B] P92.
    */
    
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);

    /*
    * Note an alternative way to accomplish the same thing:
    *
    * CPU_CCP = CCP_IOREG_gc;
    * CLKCTRL.MCLKCTRLB = 0;
    *
    * From [B] P33, the first line unlocks the necessary register to disable the prescaler for four CPU cycles.
    * However, _PROTECTED_WRITE uses inline assembly calls which is a lot more reliable in ensuring that
    * the register is set within the four-cycle window.
    */

    // From [B] P148, this will set the specified pin on the specified port to be an output.
    PORT.DIRSET = PORT.DIRSET | (1 << PIN);
    while(1) {
        // From [B] P154, this will toggle the specified pin on the specified port.
        PORT.OUTTGL = PORT.OUTTGL | (1 << PIN);
        _delay_ms(1000); 
    }
}
