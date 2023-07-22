# ESP32 Media Controler using BLE Keyboard

This Arduino project demonstrates how to use the ESP32 board along with the `BleKeyboard` library to create a wireless media controller. The program enables you to control media playback on a PC or device by simulating media control keys (play/pause, next, and previous) through Bluetooth Low Energy (BLE).

## Hardware Requirements

- ESP32 board (e.g., NodeMCU ESP-32)
- 3 push buttons (for play/pause, next, and previous)
- Buzzer
- Bluetooth-enabled device (e.g., PC, smartphone) with media playback support

## Installation

1. Connect the push buttons (for play/pause, next, and previous) to the ESP32 board following the specified pin connections.
2. Connect the buzzer to the designated pin on the ESP32 board.
3. Install the `BleKeyboard` library in your Arduino IDE to enable Bluetooth keyboard functionality.
4. Upload the provided Arduino sketch to your ESP32 board.

## Usage

1. Turn on the Bluetooth on your PC or device and pair it with the ESP32 (the device name will be "Media Controler").
2. Once connected, you can control media playback using the push buttons:
   - Press the play/pause button to toggle between play and pause.
   - Press the next button to skip to the next track.
   - Press the previous button to go back to the previous track.

The program will also provide audio feedback using the buzzer when a button is pressed.

## Troubleshooting

If the Bluetooth connection is not established, the buzzer will sound to indicate that the BLE is not connected. In this case, please ensure that Bluetooth is enabled on your device and try pairing again.

## Contact

For any questions or inquiries related to this project, you can reach out to the project owner:

- Name: Gurkirat Singh
- Email: gurkirat7092@yahoo.com
