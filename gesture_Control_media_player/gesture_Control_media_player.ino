#include <Arduino.h>
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Media Controller");

int ir1 = 22; // ir sensor 1 pin
int ir2 = 23; // ir sensor 2 pin

int f1 = 0;
int f2 = 0;
int BuzzerPin = 19; // buzzer pin
void setup()
{
    Serial.begin(9600);
    pinMode(ir1, INPUT);
    pinMode(ir2, INPUT);
    pinMode(BuzzerPin, OUTPUT); // buzzer pin as output
    bleKeyboard.begin();
}

void loop()
{
    if (bleKeyboard.isConnected())
    {
        if (digitalRead(ir1) == LOW)
        {
            f1 = 1;
            delay(1000);
        }
        if (digitalRead(ir2) == LOW)
        {
            f2 = 1;
            delay(100);
        }

        if (f1 == 1 && f2 == 1)
        {
            Serial.println("pause/play");
            bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
            f1 = 0; // Reset f1
            f2 = 0; // Reset f2
            delay(2000);
        }
        else if (f1 == 1)
        {
            Serial.println("forward");
            bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
            f1 = 0; // Reset f1
            delay(1000);
        }
        else if (f2 == 1)
        {
            Serial.println("backward");
            bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
            f2 = 0; // Reset f2
            delay(1000);
        }

        delay(100);
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
