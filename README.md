# Bare Metal Arduino

This is a simple project to demonstrate how to blink an LED on an Arduino Uno WiFi Rev2 using bare metal programming without the Arduino IDE.

## Arduino USB Access for WSL2 and Docker (Windows)

1. Install [usbipd-win](https://github.com/dorssel/usbipd-win).
2. Connect the Arduino to WSL2.
    1. Open PowerShell and WSL2.
    2. Connect the Arduino to the USB port.
    3. (PowerShell) List the connected USB devices: `usbipd list`
    4. (PowerShell) Note the `BUSID` (e.g., 2-3) and bind it (one-time setup): `usbipd bind --busid <BUSID>`
    5. (PowerShell) Attach it to your running WSL2 instance (each time you reconnect): `usbipd attach --wsl --busid <BUSID>`
    6. (WSL2) Find the port name of the device (likely /dev/ttyACM0 or /dev/ttyUSB0): `ls /dev/tty*`

## Starting/Stopping the Docker Container

- To start and enter the Docker container, run `docker compose run --rm BareMetalArduino`
- To exit the Docker container, press Ctrl + D.

## Flashing the Arduino

- To compile and flash the Arduino, run `make` from `/app`.
