# RFID Card Reader with IoT Integration

This Arduino project allows you to read RFID cards using the MFRC522 RFID module and an ESP32 board. When a valid RFID card is detected, the card UID is sent to a server over WiFi for further processing. Additionally, a buzzer provides feedback for successful card reads.

## Hardware Requirements

- ESP32 board (e.g., NodeMCU ESP-32)
- MFRC522 RFID module
- Buzzer
- RFID cards (compatible with MFRC522)

## Installation

1. Connect the MFRC522 RFID module and the buzzer to the ESP32 board as per the provided pin connections.
2. Make sure you have the necessary libraries installed in your Arduino IDE:
   - MFRC522
   - SPI
   - WiFi
   - HTTPClient
   - ArduinoJson

## Usage

1. Upload the provided Arduino sketch to your ESP32 board.
2. Modify the WiFi SSID and password in the sketch to match your WiFi network credentials.
3. Upload the sketch to your ESP32 board.
4. Open the Serial Monitor to view the output.

When a new RFID card is detected, the card UID will be displayed on the Serial Monitor. The sketch will then attempt to send the UID to the server for further processing. The buzzer will provide feedback indicating whether the UID was successfully processed or not.

## Contributing

Contributions to this project are welcome. If you find any issues or have suggestions for improvement, please feel free to open an issue or submit a pull request.

## Contact

For any questions or inquiries, you can reach out to the project owner:

- Name: Gurkirat Singh
- Email: gurkirat7092@yahoo.com
