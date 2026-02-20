default:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega4809 -c -o src/led.o src/led.c
	avr-gcc -mmcu=atmega4809 -o src/led.bin src/led.o
	avr-objcopy -O ihex -R .eeprom src/led.bin src/led.hex
	avrdude -v -c xplainedmini_updi -p m4809 -P usb -U flash:w:src/led.hex:i