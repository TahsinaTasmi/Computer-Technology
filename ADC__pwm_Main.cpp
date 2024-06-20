#include <Arduino.h>

#define _BV(bit) (1 << bit) // macro for bit shifting

#define LED_1_PIN PD2 // Uno #2
#define LED_2_PIN PD3
#define LED_3_PIN PD4
#define LED_4_PIN PD5
#define LED_5_PIN PD6
#define LED_6_PIN PD7
#define LED_7_PIN PB0
#define LED_8_PIN PB1
#define LED_9_PIN PB2
// add the rest of the definitions

#define LED_FLASH_T 250 // time constant
#define ADC_LIMITS 9    // number of voltage levels

// function prototypes
void flashLeds();
uint16_t readADC();
void ledsOn(uint8_t isOn);
void wait(uint16_t ms);

uint16_t uResult;  // ADC result 0...1023
uint8_t uLimitInd; // index of limits array

// initialize limits of ADC result values into array
// 5,0V/1024 = 4,8828125mV
// 4,9V = 4,9/0,0048828125 = 1003,5 => 1003
// 1003/9 = 111,4 => 111
uint16_t auLimits[ADC_LIMITS] = {
    1003, // [0] >1003 * 5V/1024 = 4,897V (1004=4,902V)	PWM 0
    888,  // [1] >4,336	PWM 100%
    777,  // [2] >3,794	PWM 80%
    666,  // [3] >3,252	PWM 60%
    555,  // [4] >2,710	PWM 50%
    444,  // [5] >2,168	PWM 40%
    333,  // [6] >1,626	PWM 30%
    222,  // [7] >1,084	PWM 20%
    111   // [8] >0,542	PWM 10%
};

// Initialize array of led PWM duty cycles for different voltages
uint8_t auLedPWMs[ADC_LIMITS + 1] = {0, 255, 204, 153, 128, 102, 77, 51, 26, 0}; /*add relevant values as a vector, separated by commas*/

// -------------------------------------------------------
void setup()
// -------------------------------------------------------
{

  // -------------------
  // setup GPIO
  // -------------------
  /* set Data Direction Registers */
  DDRD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
  DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);

  // -------------------
  // setup Timer0 (10ms)
  // -------------------
  /* TBD, optional  */

  // -------------------
  // setup Timer1 (PWM)
  // -------------------
  /* check for earlier lectures how to degine desired PWM on pin #10 */
  /* TBD */
  DDRB |= (1 << PB2);                    // output OC1B (Arduino #10)
  TCCR1A = (1 << COM1B1) | (1 << WGM10); // toggle OC1B on compare match, 8-bit PWM
  OCR1A = 0x00;                          // 0% duty cycle)
  TCCR1B = (1 << CS10);

  // -------------------
  // setup ADC
  // -------------------
  // AD input ADC1 (Uno#A1), result right aligned, Vref=5V, prescaler 128
  /* TBD, check from lecture */
  ADMUX |= (_BV(MUX0)) | (_BV(MUX1));                // Input on ADC1 (Uno #A1)
  ADMUX |= (_BV(REFS0));                             // Vref = 5V
  ADCSRA |= (_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0));  // prescaler 128: fadc=16Mhz/128=125kHz
  ADCSRA &= ~(_BV(ADATE));                           // disable auto trigger (default)
  ADCSRB &= ~(_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0)); // normal/free running mode (default)
  ADCSRA |= _BV(ADEN);
  // auto trigger disabled => no free run
  Serial.begin(9600);
}
void loop()
{
  // perform single AD conversion and read result value
  uResult = readADC();

  // find index of limit value from array
  // NOTE if value is not found => uLimitInd is ADC_LIMITS
  for (uLimitInd = 0; uLimitInd < ADC_LIMITS; uLimitInd++)
  {
    if (uResult > auLimits[uLimitInd])
      break; // compare ADC result
  }

  Serial.print(", uLimitInd: ");
  Serial.println(uLimitInd);

  ledsOn(0); // all leds off

  // turn led on one by one using index of auLimits[] starting from current index
  // NOTE: no break in cases, execution starts from first matching case and continues until break;
  switch (uLimitInd)
  {
  case 0:        // >=4,902V
      /* TBA */
    OCR1B =0; // green led off immediately (by setting duty cycle to 0%)!
    flashLeds(); // flash red leds for 2 seconds @2Hz
    break;

  case 1:
    PORTB |= (1 << LED_8_PIN);
  case 2:
    PORTB |= (1 << LED_7_PIN);
  case 3:
    PORTD |= (1 << LED_7_PIN);
  case 4:
    PORTD |= (1 << LED_6_PIN);
  case 5:
    PORTD |= (1 << LED_5_PIN);
  case 6:
    PORTD |= (1 << LED_4_PIN);
  case 7:
    PORTD |= (1 << LED_3_PIN);
  case 8:
    PORTD |= (1 << LED_2_PIN);
  case 9:
    PORTD |= (1 << LED_1_PIN);
    /* TBD, add the rest of the cases */

  default:
    // if value is not found in auLimits[] (adc result is less than first value) all leds remain off
    // uLimitInd equals to ADC_LIMITS
    break;

  } // end switch

  OCR1B = auLedPWMs[uLimitInd]; // adjust green led brightness to match ADC result

} // end loop

// -----------------------------------------
// ADC single conversion, return result
// -----------------------------------------
uint16_t readADC()
{
  uint16_t uADCval;
  /* TBD, optional */ // select ADC3 on AD MUX (could be done in ADC setup or passed as a parameter)
  /* TBD, optional */ // enable ADC

  /* TBD */ // start conversion by setting ADSC bit in ADCSRA
  ADCSRA |= _BV(ADSC);

  while (ADCSRA & _BV(ADSC))
    ; // wait until ADC ready ie. until ADSC bit is reset

  uADCval = ADC; // read result

  /* TBD, set the right bit to stop */ // stop ADC, no need to keep it up running
  ADCSRA &= ~(_BV(ADSC));
  return uADCval;
} // end readADC()

// -----------------------------------------
// turn yellow+red leds on or off
// -----------------------------------------
void ledsOn(uint8_t isOn)
{
  if (isOn)
  {
    // all leds on
    /* TBD, manipulate PORT bits */ // all leds on (PD2-PD7)
    /* TBD, manipulate PORT bits */ // all leds on (PB0-PB1)
    PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    PORTB |= (1 << PB0) | (1 << PB1) ;
  }
  else
  {
    /* TBD */ // all leds off (PD2-PD7)
    /* TBD */ // all leds off (PB0-PB1)
    PORTD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7));
    PORTB &= ~((1 << PB0) | (1 << PB1));
  }
} // end ledsOn()

// -----------------------------------------
// flash y+r leds 2 seconds 2Hz. T=0,5s => 4*T
// -----------------------------------------
void flashLeds()
{
  uint8_t t;

  // generate 4 pulses = 2 seconds
  for (t = 0; t < 4; t++)
  {
    // all leds off
    ledsOn(0);
    delay(LED_FLASH_T);

    // all leds on
    ledsOn(1);
    delay(LED_FLASH_T); // wait 250 ms
  }                     // end for t
} // end flashLeds

// -----------------------------------------
// wait milliseconds using timer0, optional replacement for delay()
// -----------------------------------------
void wait(uint16_t ms)
{
  uint16_t uCycles, i;

  // Timer0 OCF0A flag in TIFR0 is set every 10 ms
  uCycles = ms / 10; // count number of cycles needed

  TCNT0 = 0x00;        // reset counter
  TIFR0 |= _BV(OCF0A); // clear flag

  for (i = 0; i < uCycles; i++)
  {
    while (!(TIFR0 & _BV(OCF0A)))
      ; // wait until IRQ flag is set

    TIFR0 |= _BV(OCF0A); // Must write 1 to clear flag!
  }
} // end wait()