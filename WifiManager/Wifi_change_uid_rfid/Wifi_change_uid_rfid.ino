#include <FS.h>
#include <SPIFFS.h>
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define SS_PIN 21    // MFRC522 SDA pin
#define RST_PIN 22   // MFRC522 RST pin
int buzzer = 5;      // buzzer pin
int summonPage = 13; // summon page button pin
// miso at 23
// mosi at 19
// sck at 18
bool shouldSaveConfig = false;
char url[100] = "http://192.168.1.150:3000/api";

void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

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
  if (SPIFFS.begin())
  {
    Serial.println("SPIFFS mounted successfully");
    if (SPIFFS.exists("/config.json"))
    {
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile)
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, buf.get());
        if (!error)
        {
          Serial.println("deserialized");
          JsonObject root = doc.as<JsonObject>();
          String server = root["server"];
          Serial.println(server);
          WiFiManagerParameter custom_text("Server URL", "Server URL", server, 50);
          WiFiManager wifiManager;
          wifiManager.addParameter(&custom_text);
          bool res;
          res = wifiManager.autoConnect("add credentials");
        }
      }
    }
  }
  pinMode(buzzer, OUTPUT);           // buzzer pin as output pin
  pinMode(summonPage, INPUT_PULLUP); // summon page button as input pin
  Serial.begin(115200);              // Initialize serial communications with the PC
  SPI.begin();                       // Init SPI bus
  mfrc522.PCD_Init();                // Init MFRC522 card
  WiFiManagerParameter custom_text("Server URL", "Server URL", "http://192.168.1.150:3000/api", 50);

  WiFi.mode(WIFI_STA);
  WiFiManager wifiManager;
  // reset the setting
  // wifiManager.resetSettings();

  wifiManager.addParameter(&custom_text);

  bool res;
  res = wifiManager.autoConnect("add credentials");
  if (!res)
  {
    Serial.println("Failed to connect");
  }
  else
  {
    Serial.println("Connected");
    Serial.println(custom_text.getValue());
  }
}

void loop()
{
  if (digitalRead(summonPage) == LOW)
  {
    Serial.println("btn pressed!");
    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(60);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    if (!wifiManager.startConfigPortal("Update Credentials"))
    {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      buzz_error();
      // reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }
  }
  if (!mfrc522.PICC_IsNewCardPresent()) // Look for new cards
  {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  digitalWrite(buzzer, HIGH); // buzzer on when card is detected
  String content = "";        // String for storing the UID
  byte letter;                // variable for storing the MFRC522 data
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
  HTTPClient http;
  http.begin("http://192.168.1.100:3000/api");
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST("{\"uid\":\"" + content + "\"}");
  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
    if (response == "invalid")
    {
      buzz_error();
    }
    else if (response == "success")
    {
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
    }
  }
  else
  {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  delay(1000);
}
