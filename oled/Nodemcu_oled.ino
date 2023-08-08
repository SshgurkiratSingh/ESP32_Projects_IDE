#include <Wire.h>

// Include Adafruit Graphics and OLED libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

// reset pin not used on 4-pin OLED module
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);
int i = 0;
int dot = 0;

void setup()
{
    Wire.begin();
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    randomSeed(analogRead(0));
}
byte customChar[] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100
};
void displayRandomWithHeading()
{
    // genertaing random number

    int n1 = random(0, 40);
    int n2 = random(0, 100);
    display.clearDisplay();
    display.setTextSize(2.9);
    delay(1000);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Rand Num");
    // customChar1 here
 
    display.display();
    delay(1000);
    display.setTextSize(1);
    // Custom char 2 here
   

    display.setCursor(0, 30);
    display.print("Random 1: ");
    display.println(n1);
    // drawing a line
    display.drawLine(0, 40, 127, 40, WHITE);
    display.setCursor(0, 44);
    display.print("Random 2: ");
    display.println(n2);
    display.display();
    delay(1000);
}

void loadingForConnection()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(42, 20);
    display.println("Looking ");
    display.setCursor(49, 35);
    display.println("for ");
    display.setCursor(26, 50);
    display.print("Connection");

    for (int i = 0; i < dot; i++)
    {
        display.print(".");
    }
    dot = (dot + 1) % 4;

    display.display();
    delay(400);
}
void loop()
{

   display.clearDisplay();
    display.drawBitmap(3, 0, customChar, 8, 8, WHITE);
    display.display();
    delay(1000);
    // roate the display
    display.setRotation(2);
    display.display();
    delay(1000);
    display.setRotation(0);
    display.display();
    delay(1000);
    display.setRotation(1);
    display.display();
    delay(1000);
    display.setRotation(3);
    display.display();
    delay(1000);
    display.setRotation(0);
    display.display();
}