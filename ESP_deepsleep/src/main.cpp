#include <Arduino.h>
#define Threshold 40
#if CONFIG_IDF_TARGET_ESP32
#define THRESHOLD 40   /* Greater the value, more the sensitivity */
#else                  // ESP32-S2 and ESP32-S3 + default for other chips (to be adjusted) */
#define THRESHOLD 5000 /* Lower the value, more the sensitivity */
#endif
#define LED_PIN 19
#define BUTTON_PIN 26
// The ESP32 chip features 34 physical GPIO pins (GPIO0 ~ GPIO19, GPIO21 ~ GPIO23, GPIO25 ~ GPIO27, and GPIO32 ~ GPIO39).
// GPIO_NUM_0 ~ GPIO_NUM_39 are defined in the Arduino.h header file.
#define SECOND_TO_RUN 3
#define SECOND_TO_SLEEP 10
#define uS_TO_S_FACTOR 1000000
RTC_DATA_ATTR int count = 0;
RTC_DATA_ATTR uint8_t currentBrightness = 0;
touch_pad_t touchPin;

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}
void setup()
{
  Serial.begin(115200);
  count++;
  Serial.println(String("count:") + count);
  print_wakeup_reason();
  esp_sleep_enable_timer_wakeup(SECOND_TO_SLEEP * uS_TO_S_FACTOR);
  ledcSetup(0, 5000, 8);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  ledcAttachPin(LED_PIN, 0);
#if CONFIG_IDF_TARGET_ESP32
  touchSleepWakeUpEnable(T3, THRESHOLD);
  touchSleepWakeUpEnable(T7, THRESHOLD);

#else // ESP32-S2 + ESP32-S3
  // Setup sleep wakeup on Touch Pad 3 (GPIO3)
  touchSleepWakeUpEnable(T3, THRESHOLD);
#endif
}

void loop()
{
  if (millis() > SECOND_TO_RUN * 1000)
  {
    esp_deep_sleep_start();
  }
  Serial.println("loop");
  currentBrightness += 10;
  ledcWrite(0, currentBrightness);
  delay(200);
}