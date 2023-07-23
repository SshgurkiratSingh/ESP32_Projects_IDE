
// using BleKeyboard library to send media control keys to a PC using 3 buttons .
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Media Controler");
int BuzzerPin = 19; // buzzer pin
void setup()
{
    pinMode(23, INPUT_PULLUP);  // pause/play button as input
    pinMode(21, INPUT_PULLUP);  // next button as input
    pinMode(4, INPUT_PULLUP);   // previous button as input
    pinMode(BuzzerPin, OUTPUT); // buzzer pin as output
    Serial.begin(115200);       // begin serial
    bleKeyboard.begin();        // begin bleKeyboard
}
void loop()
{

    if (bleKeyboard.isConnected())
    {

        if (digitalRead(23) == LOW)
        {
            Serial.println("Play/Pause");
            digitalWrite(BuzzerPin, HIGH); // buzzer on
            bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
            delay(100);
            digitalWrite(BuzzerPin, LOW);
        }
        if (digitalRead(21) == LOW)
        {
            Serial.println("Next");
            digitalWrite(BuzzerPin, HIGH); // buzzer on
            bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
            delay(100);
            digitalWrite(BuzzerPin, LOW);
        }
        if (digitalRead(4) == LOW)
        {
            Serial.println("Previous");
            digitalWrite(BuzzerPin, HIGH); // buzzer on
            bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
            delay(100);
            digitalWrite(BuzzerPin, LOW);
        }
        delay(50);
    }
    else
    {
        Serial.println("BLE not connected");
        digitalWrite(BuzzerPin, HIGH); // buzzer on when card is detected
        delay(100);
        digitalWrite(BuzzerPin, LOW);
        delay(5000);
    }
}
