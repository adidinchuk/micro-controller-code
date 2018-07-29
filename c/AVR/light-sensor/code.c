/* LDR - Sensor Demo */

// ------- Preamble -------- //
#define F_CPU 1000000L                      /* atTiny85 Bautrate */
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include "pinDefines.h"

/*-----
Function	ADMUX Value	AREF Pin
Vcc (Power Supply)	(1<<REFS0)	Leave unconnected
Internal 2.56V	(1<<REFS0)|(1<<REFS1)	Leave unconnected
External Reference	0	Connect Voltage Reference
-----*/

// ADMUX - This register is used to select reference voltage source, how the result should be stored (either left adjusted or right adjusted), analog port channel to be used for conversion.
//ADCSRA - This register is responsible for enabling ADC, start ADC converting, prescaler selection and interrupt control.

//
static inline void initADC0(void){
  ADMUX |= (1<<REFS0) | (1<<REFS1);               /* reference AVCC voltage */
  ADCSRA |= (1<<ADPS1) | (1<<ADPS0);  /*ADC clock prescaler /8 */
  ADCSRA |= (1<<ADEN);                           /* enable ADC */
}
int main(void){

  //uint8_t ledValue;
  uint16_t adcValue;
  //uint8_t i;

  DDRB |= 0b000001;

  PORTB = 0b00000001;
  _delay_ms(5000);
  PORTB = 0b00000000;

  initADC0();
  DDRB = 0xff;

  while(1){
    //ADC conversion
    ADCSRA |= (1<<ADSC);    
    loop_until_bit_is_clear(ADCSRA, ADSC);
    // read ADC
    adcValue = ADC;
    if(adcValue > 50){
      PORTB = 0b00000001;
    }else{
      PORTB = 0b00000000;
    }
    //ledValue = (adcValue>>7);
    _delay_ms(50);
  }
  return 0;
}