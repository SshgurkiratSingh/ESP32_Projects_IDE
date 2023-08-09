#include <FS.h>                                     // File System
#include <SPIFFS.h>                                 // File System
char output[40] = "http://192.168.1.100:3000/api/"; // default output url
#include <Arduino.h>
#include <SPI.h>                                                    // SPI library
#include <MFRC522.h>                                                // RFID library
#include <WiFiManager.h>                                            // wifi manager
#include <HTTPClient.h>                                             // http client
#include <ArduinoJson.h>                                            // json
#define SS_PIN 21                                                   // MFRC522 SDA pin
#define RST_PIN 22                                                  // MFRC522 RST pin
int buzzer = 5;                                                     // buzzer pin
int summonPage = 13;                                                // summon page button pin
bool shouldSaveConfig = false;                                      // should save config
WiFiManagerParameter custom_output("output", "output", output, 40); // custom output for url
WiFiManager wifiManager;                                            // wifi manager instance

void saveConfigCallback() // save config callback function
{
  Serial.println("Should save config");
  shouldSaveConfig = true; // set should save config to true
}
// miso at 23
// mosi at 19
// sck at 18
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

  if (SPIFFS.begin()) // start file system
  {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) // check if file exists
    {

      Serial.println("reading config file");              // print message
      File configFile = SPIFFS.open("/config.json", "r"); // open file in read mode
      if (configFile)
      {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]); // allocate memory

        configFile.readBytes(buf.get(), size); // read file into buffer
        StaticJsonDocument<512> jsonBuffer;    // You can adjust the size as needed

        DeserializationError error = deserializeJson(jsonBuffer, buf.get(), size); // Deserialize the JSON from file

        if (error)
        {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
        }
        else
        {
          serializeJson(jsonBuffer, Serial);
        }
      }
    }
  }
  else
  {
    Serial.println("failed to mount FS");
  }

  WiFi.mode(WIFI_STA);               // set wifi mode to station
  pinMode(buzzer, OUTPUT);           // buzzer pin as output pin
  pinMode(summonPage, INPUT_PULLUP); // summon page button as input pin
  Serial.begin(115200);              // Initialize serial communications with the PC
  SPI.begin();                       // Init SPI bus
  mfrc522.PCD_Init();                // Init MFRC522 card

  // reset the setting
  // wifiManager.resetSettings();

  bool res;
  res = wifiManager.autoConnect("add credentials"); // make A ssid to get credentials
  if (!res)
  {
    Serial.println(F("Failed to connect"));
  }
  else
  {
    Serial.println("Connected");
    Serial.println(custom_output.getValue()); // print the output url
  }
  wifiManager.setSaveConfigCallback(saveConfigCallback); // set save config callback

  wifiManager.addParameter(&custom_output); //  add paramter

  Serial.println("Connected.");

  strcpy(output, custom_output.getValue()); 

  // save the custom parameters to FS
  if (shouldSaveConfig)
  {
    Serial.println("saving config");

    DynamicJsonDocument jsonBuffer(512); // You can adjust the size as needed
    JsonObject json = jsonBuffer.to<JsonObject>();

    json["output"] = output;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
      Serial.println("failed to open config file for writing");
      return;
    }

    serializeJson(json, Serial);
    serializeJson(json, configFile);
    configFile.close();
  }
}

void loop()
{
  if (digitalRead(summonPage) == LOW)
  {
    Serial.println(F("btn pressed!"));

    wifiManager.setConfigPortalTimeout(60);
    saveConfigCallback();
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    if (!wifiManager.startConfigPortal("Update Credentials"))
    {
      Serial.println(F("failed to connect and hit timeout"));
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
  http.begin(output);
  Serial.println(output);
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
