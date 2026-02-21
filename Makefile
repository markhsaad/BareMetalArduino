MCU     = atmega4809
TARGET  = blink
SRC_DIR = src

SRC_FILE = $(SRC_DIR)/$(TARGET).c
OBJ_FILE = $(SRC_DIR)/$(TARGET).o
BIN_FILE = $(SRC_DIR)/$(TARGET).bin
HEX_FILE = $(SRC_DIR)/$(TARGET).hex

default:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=$(MCU) -c -o $(OBJ_FILE) $(SRC_FILE)
	avr-gcc -mmcu=$(MCU) -o $(BIN_FILE) $(OBJ_FILE)
	avr-objcopy -O ihex -R .eeprom $(BIN_FILE) $(HEX_FILE)
	avrdude -v -c xplainedmini_updi -p m4809 -P usb -U flash:w:$(HEX_FILE):i
	
	rm $(OBJ_FILE) $(BIN_FILE) $(HEX_FILE)