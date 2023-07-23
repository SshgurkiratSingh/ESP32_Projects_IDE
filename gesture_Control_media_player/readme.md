# Media Controller with IR Sensors using BLE Keyboard

This Arduino project allows you to control media playback on a Bluetooth-enabled device using IR sensors and the ESP32 board acting as a Bluetooth Low Energy (BLE) keyboard. The program simulates media control keys (play/pause, next track, previous track) using the `BleKeyboard` library.

## Hardware Requirements

- ESP32 board (e.g., NodeMCU ESP-32)
- 2 IR sensors
- Buzzer
- Bluetooth-enabled device (e.g., PC, smartphone) with media playback support

## Installation

1. Connect the two IR sensors and the buzzer to the ESP32 board according to the specified pin connections.
2. Install the `BleKeyboard` library in your Arduino IDE to enable BLE keyboard functionality.
3. Upload the provided Arduino sketch to your ESP32 board.

## Usage

1. Turn on the Bluetooth on your PC or device and pair it with the ESP32 (the device name will be "Media Controller").
2. Once connected, the IR sensors can be used to control media playback:
   - When both IR sensors are triggered simultaneously, it will play/pause media.
   - If only one IR sensor is triggered, it will either skip to the next track (forward) or go back to the previous track (backward).

The program provides audio feedback using the buzzer when a media control action is executed.

## IR Sensor Behavior

- `ir1` (IR Sensor 1): Triggers the forward (next track) action.
- `ir2` (IR Sensor 2): Triggers the backward (previous track) action.

## Troubleshooting

If the Bluetooth connection is not established, the buzzer will sound to indicate that BLE is not connected. In this case, please ensure that Bluetooth is enabled on your device and try pairing again.

## Contact

For any questions or inquiries related to this project, you can reach out to the project owner:

- Name: Gurkirat Singh
- Email: gurkirat7092@yahoo.com
