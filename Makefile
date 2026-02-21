TARGET = blink
SRC_DIR = src

default:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega4809 -c -o $(SRC_DIR)/$(TARGET).o $(SRC_DIR)/$(TARGET).c
	avr-gcc -mmcu=atmega4809 -o $(SRC_DIR)/$(TARGET).bin $(SRC_DIR)/$(TARGET).o
	avr-objcopy -O ihex -R .eeprom $(SRC_DIR)/$(TARGET).bin $(SRC_DIR)/$(TARGET).hex
	avrdude -v -c xplainedmini_updi -p m4809 -P usb -U flash:w:$(SRC_DIR)/$(TARGET).hex:i