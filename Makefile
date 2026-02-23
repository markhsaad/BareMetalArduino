MCU       = atmega4809
TARGET    = blink
SRC_DIR   = src
BUILD_DIR = build

SRC_FILE = $(SRC_DIR)/$(TARGET).c
OBJ_FILE = $(BUILD_DIR)/$(TARGET).o
BIN_FILE = $(BUILD_DIR)/$(TARGET).bin
HEX_FILE = $(BUILD_DIR)/$(TARGET).hex

default:
	mkdir -p $(BUILD_DIR)

	avr-gcc -Os -mmcu=$(MCU) -c -o $(OBJ_FILE) $(SRC_FILE)
	avr-gcc -mmcu=$(MCU) -o $(BIN_FILE) $(OBJ_FILE)
	avr-objcopy -O ihex -R .eeprom $(BIN_FILE) $(HEX_FILE)
	avrdude -v -c xplainedmini_updi -p m4809 -P usb -U flash:w:$(HEX_FILE):i