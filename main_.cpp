#include <Arduino.h>
int T1 = 2000;
int T2 = 4000;
int T3 = 1000;
int T4 = 4000;

void setup()
{
  // put your setup code here, to run once:
  // LED on Port B pin PB3 (Uno connector #11)
  DDRB |= (1 << PB5);
  DDRB |= (1 << PB3);
  DDRB |= (1 << PB1);
  // PORTB |={1<<PB3}; // switch LED on default state;
}

void loop()
{
  PORTB |= (1 << PB5);  // red off
  PORTB &= ~(1 << PB3); // yellow on
  PORTB |= (1 << PB1);  // green off
  delay(T1);

  PORTB &= ~(1 << PB5); // red off
  PORTB |= (1 << PB3);  // yellow off
  PORTB |= (1 << PB1);  // green on
  delay(T4);

  PORTB &= ~(1 << PB5); // red on
  PORTB &= ~(1 << PB3); // yellow on
  PORTB |= (1 << PB1);
  delay(T3);

  // PORTB |= (1<<PB5); //red off
  PORTB |= (1 << PB5);  // red of
  PORTB |= (1 << PB3);  // yellow of
  PORTB &= ~(1 << PB1); // green on
  delay(T2);

  // delay(T2);*/
}

