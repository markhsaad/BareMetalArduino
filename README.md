# Bare Metal Arduino

Credit to [Low Level](https://youtu.be/j4xw8QomkXs?si=8F10g2sGsCeOV5s-) for helping me get started.

The goal of this repository is to provide a template to get started with bare metal programming with an Arduino Uno WiFi Rev2. The program blinks the built-in LED on the Arduino and prints "Hello, world!" to the terminal at 1 Hz. Detailed domments are provided in the C file to explain how the code works. Prior installation of Docker is assumed.

To get started:

1. Navigate to the root directory of this repository.
2. Build the Docker image: `docker compose build`
3. Start and enter the Docker container: `docker compose run --rm BareMetalArduino`
4. Inside the Docker container, navigate to `/app`.
5. Compile the code and flash it to the Arduino: `make`
    1. The built-in LED should start blinking at 1 Hz.
6. Monitor the output from the Arduino: `tio /dev/ttyACM0 -b 9600`
    1. "Hello, world!" should print in the terminal at 1 Hz and the TX LED should blink.
    
**NOTE: Replace all instances of `/dev/ttyACM0` in this repository with whatever your device's port name is.**

## Arduino USB Access for WSL2 and Docker (Windows)

In case you are running this on WSL2, below are instructions for communicating with the Arduino over USB from the Docker container.

1. Install [usbipd-win](https://github.com/dorssel/usbipd-win).
2. Connect the Arduino to WSL2.
    1. Open PowerShell and WSL2.
    2. Connect the Arduino to the USB port.
    3. (PowerShell) List the connected USB devices: `usbipd list`
    4. (PowerShell) Note the `BUSID` (e.g., 2-3) and bind it (one-time setup): `usbipd bind --busid <BUSID>`
    5. (PowerShell) Attach it to your running WSL2 instance (each time you reconnect): `usbipd attach --wsl --busid <BUSID>`
    6. (WSL2) Find the port name of the device (likely `/dev/ttyACM0` or `/dev/ttyUSB0`): `ls /dev/tty*`
