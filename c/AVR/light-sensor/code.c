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

volatile int analogResult = 0;

int main(void){

  //pin 4 read, pin 3 write
  DDRB &= ~(1<<DDB4);
  DDRB |= ((1<<DDB3)|(1<<DDB0));
  PORTB &= ~(1<<PB0);/////
  //set voltage to internal, right and Right adjust
  ADMUX &= ~((1<<REFS1)|(1<<REFS0)|(1<<ADLAR));
  
  //set input as ADC2 (PIN 4)
  ADMUX |= (1<<MUX1);

  //set free running mode (dont need ADCSRA |= (1<<ADSC); every read)
  ADCSRB &=~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));

  //Digital input disable (not required)
  DIDR0 |= (1<<ADC2D);

  //enable ADC
  ADCSRA |= ((1<<ADEN)|(1<<ADATE));

  //convert
  ADCSRA |= (1<<ADSC);

  while(1){
    PORTB |= (1<<PB0);/////
    while ((ADCSRA & (1<<ADSC))); //wait for conversion to finish
    _delay_ms(1000);/////
    PORTB &= ~(1<<PB0);/////
    
    // ADCH last 2 bits of result in HIGH register shift to combine into 10 bit result
    analogResult = (ADCH<<8)|ADCL;
    PORTB |= (1<<PB0);
    
    if(analogResult>250){
      PORTB |= (1<<PB3);
    }else{
      PORTB &= ~(1<<PB3);
    }
  }
  return 0;
}