#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define SS_PIN 21  // MFRC522 SDA pin
#define RST_PIN 22 // MFRC522 RST pin
int buzzer = 5;    // buzzer pin

MFRC522 mfrc522(SS_PIN, RST_PIN); // MFRC522 instance
void buzz_error()                 // buzzer error function
{
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(100);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
}
void setup()
{
    pinMode(buzzer, OUTPUT); // buzzer pin as output pin
    Serial.begin(115200);    // Initialize serial communications with the PC
    SPI.begin();             // Init SPI bus
    mfrc522.PCD_Init();      // Init MFRC522 card

    WiFi.begin("ConForNode", "12345678"); // WiFi connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");
}

void loop()
{
    if (!mfrc522.PICC_IsNewCardPresent()) // Look for new cards
    {
        return;
    }
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }
    digitalWrite(buzzer, HIGH); // buzzer on when card is detected

    String content = ""; // String for storing the UID
    byte letter;         // variable for storing the MFRC522 data

    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); // Concatenates the UID (a byte) with a space between
        content.concat(String(mfrc522.uid.uidByte[i], HEX));                // Converts the UID (a byte) to HEX
    }

    content.toUpperCase();     // Changes the letters in the UID to uppercase
    delay(100);                // buzzer off after 100ms
    digitalWrite(buzzer, LOW); // buzzer off
    Serial.println();          // Print UID
    Serial.print("Message: ");
    Serial.println(content);
    Serial.println();
    Serial.println("Sending data to server");

    HTTPClient http;                                    // HTTPClient object instance
    http.begin("http://192.168.252.113:3000/api");        // Specifing request destination
    http.addHeader("Content-Type", "application/json"); // Specifing content type

    int httpResponseCode = http.POST("{\"uid\":\"" + content + "\"}"); // Sending POST request

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
        if (response == "invalid") // buzzer error if invalid
        {
            buzz_error();
        }69420
        else if (response == "success") // buzzer success if success
        {
            digitalWrite(buzzer, HIGH);
            delay(100);
            digitalWrite(buzzer, LOW);
        }ar
    }
    else
    {
        Serial.print("Error on sending POST: ");
        buzz_error();
        Serial.println(httpResponseCode);
    }

    http.end();  // Free resources
    delay(1000); // Wait 1 second before next scan
}
