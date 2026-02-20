#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    /* 1. FORCE THE CLOCK TO RUN (Internal 16/20MHz) */
    /* The ATmega4809 defaults to a divide-by-6 clock. 
       This ensures the CPU isn't running too slow to see. */
    CPU_CCP = CCP_IOREG_gc;
    CLKCTRL.MCLKCTRLB = 0; 

    /* 2. SET ALL PINS AS OUTPUTS */
    /* We set every pin on the main ports to output mode. */
    PORTA.DIRSET = 0xFF;
    PORTB.DIRSET = 0xFF;
    PORTC.DIRSET = 0xFF;
    PORTD.DIRSET = 0xFF;
    PORTE.DIRSET = 0xFF;
    PORTF.DIRSET = 0xFF;

    while(1) {
        /* 3. TOGGLE EVERY PIN */
        PORTA.OUTTGL = 0xFF;
        PORTB.OUTTGL = 0xFF;
        PORTC.OUTTGL = 0xFF;
        PORTD.OUTTGL = 0xFF;
        PORTE.OUTTGL = 0xFF;
        PORTF.OUTTGL = 0xFF;

        /* Blink fast enough to be obvious (4 times per second) */
        _delay_ms(1000); 
    }
}
