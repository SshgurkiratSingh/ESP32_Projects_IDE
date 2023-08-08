#include <Arduino.h>

#define ROOT_MENU_TOTAL 3
#define SUB_MENU1_TOTAL 2
#define SUB_MENU2_TOTAL 4
#define SUB_MENU3_TOTAL 5

enum page
{
    ROOT_MENU,
    SUB_MENU1,
    SUB_MENU2,
    SUB_MENU3
};

const int accept_btn = 2;
const int up_btn = 3;
const int down_btn = 4;
const int back_btn = 5;

uint8_t selected = 1;

void clearScreen()
{
    for (int i = 0; i < 20; i++)
    {
        Serial.println();
    }
}

void printPlainLine()
{
    Serial.println(F("--------------------------------------------------"));
}

void printSelected(uint8_t p1, uint8_t p2)
{
    if (p1 == p2)
    {
        Serial.print(F("->"));
    }
    else
    {
        Serial.print(F("  "));
    }
}

enum page currpage = ROOT_MENU;

void page_RootMenu()
{
    boolean updateDisplay = true;
    unsigned long loopStartMs;

    while (true)
    {
        loopStartMs = millis();

        if (updateDisplay)
        {
            updateDisplay = false;
            clearScreen();
            Serial.println(F("Root Menu"));
            printPlainLine();
            printSelected(selected, 1);
            Serial.println(F("First Menu"));
            printSelected(selected, 2);
            Serial.println(F("Second Menu"));
            printSelected(selected, 3);
            Serial.println(F("Third Menu"));
            Serial.println();
            Serial.println();
            Serial.println();
            printPlainLine();
        }

        if (digitalRead(up_btn) == LOW)
        {
            selected = selected == 1 ? ROOT_MENU_TOTAL : selected - 1;
            updateDisplay = true;
            delay(100);
        }

        if (digitalRead(down_btn) == LOW)
        {
            selected = selected == ROOT_MENU_TOTAL ? 1 : selected + 1;
            updateDisplay = true;
            delay(100);
        }

        if (digitalRead(accept_btn) == LOW)
        {
            delay(100);
            switch (selected)
            {
            case 1:
                currpage = SUB_MENU1;
                return;
            case 2:
                currpage = SUB_MENU2;
                return;
            case 3:
                currpage = SUB_MENU3;
                return;
            }
        }

        while (millis() - loopStartMs < 25)
        {
            delay(2);
        }
    }
}

void page_SubMenu1()
{
    boolean updateDisplay = true;
    unsigned long loopStartMs;

    while (true)
    {
        loopStartMs = millis();

        if (updateDisplay)
        {
            updateDisplay = false;
            clearScreen();
            Serial.println(F("Sub Menu 1"));
            printPlainLine();
            printSelected(selected, 1);
            Serial.println(F("First Menu"));
            printSelected(selected, 2);
            Serial.println(F("Second Menu"));
            Serial.println();
            Serial.println();
            Serial.println();
            Serial.println();
            Serial.println();
            Serial.println();
            printPlainLine();
        }

        if (digitalRead(up_btn) == LOW)
        {
            selected = selected == 1 ? SUB_MENU1_TOTAL : selected - 1;
            updateDisplay = true;
            delay(100);
        }

        if (digitalRead(down_btn) == LOW)
        {
            selected = selected == SUB_MENU1_TOTAL ? 1 : selected + 1;
            updateDisplay = true;
            delay(100);
        }

        if (digitalRead(back_btn) == LOW)
        {
            currpage = ROOT_MENU;
            return;
        }

        while (millis() - loopStartMs < 25)
        {
            delay(2);
        }
    }
}

void page_SubMenu2()
{
   
}

void page_SubMenu3()
{
   
}

void setup()
{
    Serial.begin(9600);
    pinMode(accept_btn, INPUT_PULLUP);
    pinMode(up_btn, INPUT_PULLUP);
    pinMode(down_btn, INPUT_PULLUP);
    pinMode(back_btn, INPUT_PULLUP);
}

void loop()
{
    switch (currpage)
    {
    case ROOT_MENU:
        page_RootMenu();
        break;
    case SUB_MENU1:
        page_SubMenu1();
        break;
    case SUB_MENU2:
        page_SubMenu2();
        break;
    case SUB_MENU3:
        page_SubMenu3();
        break;
    }
}
