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
* From [B] P55, the factory CPU speed is 20 MHz. However, running
* `avrdude -v -c xplainedmini_updi -p m4809 -P usb -U fuse2:r:-:h` in the terminal, we get the value
* of the OSCCFG register value to be 0x01, which means that the CPU speed is set to 16 MHz (the Arduino
* team probably reprogrammed the fuses for backwards compatibility). This macro needs to be set for 
* _delay_ms() to function properly. The more proper way to set it would be as a compiler flag 
* `-DF_CPU=16000000UL` in the avr-gcc call, but I have set it here for increased clarity. Note that it
* is set *before* the includes for headers that may depend on it, like util/delay.h 
*/ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>  // Required for _delay_ms() 
#include <avr/cpufunc.h> // Required for _PROTECTED_WRITE()

// From [A] P1, the built-in LED is connected to PD06 (port D, pin 6) on the MCU.
#define PORT PORTD
#define PIN 6

// See /usr/avr/include/avr/iom4809.h for additional useful macros, many of which are used below 
// e.g., USART_DREIF_bm, PIN4_bm, etc.

/***************************************************************************************************************************************/
/*
* ==================
* BLINKING THE LED *
* ==================
*/
void builtin_led_toggle(void) {
    // From [B] P154, this will toggle the specified pin on the specified port.
    PORT.OUTTGL = PORT.OUTTGL | (1 << PIN);
}

void builtin_led_init(void) {
    // From [B] P148, this will set the specified pin on the specified port to be an output.
    PORT.DIRSET = PORT.DIRSET | (1 << PIN);
}
/***************************************************************************************************************************************/
/***************************************************************************************************************************************/
/*
* ======================
* PRINTING TO TERMINAL *
* ======================
*/

/*
* - The Arduino Uno Wifi Rev2 has an ATmega4809 and an ATmega32U4.
* - The ATmega32U4 acts as a "USB bridge" between the ATmega4809 and the laptop's USB port.
* - From [A] P2, the ATmega4809 connects with the ATmega32U4 on PB04 and PB05 (a.k.a. PB4 and PB5).
* - From [B] P18, PB4 corresponds to TXD on USART3.
*/

void usart3_send_char(char c) {
    // See [B] 296.
    while (!(USART3.STATUS & USART_DREIF_bm));
    USART3.TXDATAL = c;
}

int usart3_print_char(char c, FILE *stream) {
    if (c == '\n') usart3_send_char('\r');
    usart3_send_char(c);
    return 0;
}

FILE uart_output = FDEV_SETUP_STREAM(usart3_print_char, NULL, _FDEV_SETUP_WRITE);

void usart3_init(void) {    
    /********************************************************************/
    // From [B] P278, initialize for Full Duplex Mode:

    // 1. Set the baud rate (USARTNn.BAUD).
    // See Table 23.1 on [B] P280 for the baud rate calculation.
    USART3.BAUD = (uint16_t)((64.0 * F_CPU) / (16.0 * 9600.0)); 
    
    // 2. Set the frame format and mode of operation (USARTn.CTRLC).
    // Use the default of 8 bits per frame.

    // 3. Configure the TXD pin as an output.
    PORTB.DIRSET = PIN4_bm;

    // 4. Enable the transmitter and the receiver (USARTn.CTRLB);
    // Only use the transmitter for now.
    USART3.CTRLB = USART_TXEN_bm;
    /********************************************************************/

    // Need to do this because of subnote (3) on [B] P18 for PB4; see [B] 137 for the register details.
    PORTMUX.USARTROUTEA = PORTMUX_USART3_ALT1_gc;

    // Redirect printf output to usart3_print_char().
    stdout = &uart_output;
}
/***************************************************************************************************************************************/

int main() {
    /*
    * From [B] P92, the prescaler defaults to 6. This means that the CPU is running at (F_CPU/6) MHz. 
    * If we do not account for this, then the command _delay_ms(1000) will wait for 1 second based on 
    * a F_CPU MHz CPU speed, which is really 6 seconds for a (F_CPU/6) MHz clock speed.
    *
    * EXPECTED_DELAY_TIME_S = 1
    *
    * EXPECTED_CPU_SPEED = F_CPU
    * EXPECTED_CYCLES = EXPECTED_DELAY_TIME_S / (1 / EXPECTED_CPU_SPEED)
    *                 = 1 / (1 / F_CPU)
    *                 = F_CPU
    *
    * PRESCALER = 6
    * ACTUAL_CPU_SPEED = EXPECTED_CPU_SPEED / PRESCALER
    *                  = F_CPU / 6
    *
    * ACTUAL_DELAY_TIME_S = EXPECTED_CYCLES * (1 / ACTUAL_CPU_SPEED)
    *                   = F_CPU * (1 / (F_CPU / 6))
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

    builtin_led_init();
    usart3_init();
    while(1) {
        printf("Hello, world!\n");
        builtin_led_toggle();
        _delay_ms(1000); 
    }
}
