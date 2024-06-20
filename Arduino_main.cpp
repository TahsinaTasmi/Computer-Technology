#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

// Pin definitions
#define neutralLed 2
#define gear1LED 3
#define gear2LED 4
#define reverseLED 5
#define gPlusButton 6
#define gMinusButton 7
#define gPlusMinusButton 8

int currentGear = 0;

// Function definitions
void pinMode(int pin, bool isOutput)
{
  if (isOutput)
  {
    DDRD |= (1 << pin);
  }
  else
  {
    DDRD &= ~(1 << pin);
    PORTD |= (1 << pin); // Enable internal pull-up resistor
  }
}

bool digitalRead(int pin)
{
  return (PIND & (1 << pin)) != 0;
}

void digitalWrite(int pin, bool value)
{
  if (value)
  {
    PORTD |= (1 << pin);
  }
  else
  {
    PORTD &= ~(1 << pin);
  }
}

void delay(unsigned long ms)
{
  while (ms > 0)
  {
    _delay_ms(1);
    ms--;
  }
}

void setup()
{
  pinMode(neutralLed, true);        // OUTPUT
  pinMode(gear1LED, true);          // OUTPUT
  pinMode(gear2LED, true);          // OUTPUT
  pinMode(reverseLED, true);        // OUTPUT
  pinMode(gPlusButton, false);      // INPUT
  pinMode(gMinusButton, false);     // INPUT
  pinMode(gPlusMinusButton, false); // INPUT
}

void loop()
{
  bool gPlusPressed = digitalRead(gPlusButton);
  bool gMinusPressed = digitalRead(gMinusButton);
  bool gPlusMinusPressed = digitalRead(gPlusMinusButton);

  if (gPlusPressed)
  {
    if (currentGear == 0)
    {
      currentGear = 1;
      delay(100);
    }
    else if (currentGear == 1)
    {
      currentGear = 2;
      delay(100);
    }
  }
  else if (gMinusPressed)
  {
    if (currentGear == 2)
    {
      currentGear = 1;
      delay(100);
    }
    else if (currentGear == 1)
    {
      currentGear = 0;
      delay(100);
    }
    else if (currentGear == 3)
    {
      currentGear = 0;
      delay(200);
    }
  }
  else if (gPlusMinusPressed)
  {
    currentGear = 0;
    delay(300);
  }

  digitalWrite(neutralLed, currentGear == 0);
  digitalWrite(gear1LED, currentGear == 1);
  digitalWrite(gear2LED, currentGear == 2);
  digitalWrite(reverseLED, currentGear == 3);
}

int main()
{
  setup();
  while (1)
  {
    loop();
  }
  return 0;
}



