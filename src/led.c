// ATmega4809 datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf
// Arduino Uno Wifi Rev 2 Pinout: https://docs.arduino.cc/resources/pinouts/ABX00021-full-pinout.pdf

#include <avr/io.h>
#include <util/delay.h>

int main() {
    /* read more later
    Breakdown of the Code
    CPU_CCP = CCP_IOREG_gc;
    This "unlocks" protected registers. Many critical system settings, like clock controls, are guarded by Configuration Change Protection (CCP) to prevent accidental changes.
    Writing the CCP_IOREG_gc key to this register provides a small window (typically 4 CPU cycles) during which the protected CLKCTRL registers can be modified.
    CLKCTRL.MCLKCTRLB = 0;
    This writes 0 to the Main Clock Control B register.
    Setting this to 0 clears the PEN (Prescaler Enable) bit, effectively bypassing the divider.
    By default, many of these chips start with a prescaler of 6 (running at ~3.33 MHz from a 20 MHz source); this command switches the CPU to the full 20 MHz. 
    */
    CPU_CCP = CCP_IOREG_gc;
    CLKCTRL.MCLKCTRLB = 0;
    
    // from pinout, led controlled by port D, pin 6

    // P148
    // set PORTD.DIR[6] as output
    PORTD.DIRSET = PORTD.DIRSET | (1 << 6);
    while(1) {
        // P154
        // toggle bit 6 of PORTD.OUT
        PORTD.OUTTGL = PORTD.OUTTGL | (1 << 6);
        
        _delay_ms(1000); 
    }
}
