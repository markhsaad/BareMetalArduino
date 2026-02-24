# Bare Metal Arduino

Credit to [Low Level](https://youtu.be/j4xw8QomkXs?si=8F10g2sGsCeOV5s-) for helping me get started.

This is a simple project to demonstrate how to blink an LED on an Arduino Uno WiFi Rev2 using bare metal programming without the Arduino IDE. There are four main steps:

1. Create a clean, easily reproducible environment, hence the decision to use Docker within WSL2.
    1. See `Dockerfile`, `docker-compose.yml`, and section **Starting/Stopping the Docker Container**.
2. Establish communication between the Arduino and the Docker container.
    1. See section **Arduino USB Access for WSL2 and Docker (Windows)**
3. Implement the basic commands needed to compile and flash a simple C file to the Arduino.
    1. See `Makefile` and section **Flashing the Arduino**.
4. Implement the logic needed to control the built-in LED on the Arduino.
    1. See `src/blink.c`.

## Arduino USB Access for WSL2 and Docker (Windows)

1. Install [usbipd-win](https://github.com/dorssel/usbipd-win).
2. Connect the Arduino to WSL2.
    1. Open PowerShell and WSL2.
    2. Connect the Arduino to the USB port.
    3. (PowerShell) List the connected USB devices: `usbipd list`
    4. (PowerShell) Note the `BUSID` (e.g., 2-3) and bind it (one-time setup): `usbipd bind --busid <BUSID>`
    5. (PowerShell) Attach it to your running WSL2 instance (each time you reconnect): `usbipd attach --wsl --busid <BUSID>`
    6. (WSL2) Find the port name of the device (likely `/dev/ttyACM0` or `/dev/ttyUSB0`): `ls /dev/tty*`

## Starting/Stopping the Docker Container

- To start and enter the Docker container, run `docker compose run --rm BareMetalArduino`
- To exit the Docker container, press Ctrl + D.

## Flashing the Arduino

- To compile and flash the Arduino, run `make` from `/app`.
