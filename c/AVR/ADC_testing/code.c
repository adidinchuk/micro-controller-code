/* LDR - Sensor Demo -- using ADC  */
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

#define THRESHOLD 800//Threashold (out of 1023)

volatile int analogResult = 0;

int main(void){
  //pin 4 read, pin 3 write
  DDRB &= ~(1<<DDB4);
  DDRB |= ((1<<DDB3)|(1<<DDB2));
  
  //set voltage to internal and right adjust (ADLAR)
  ADMUX &= ~((1<<REFS1)|(1<<REFS0)|(1<<ADLAR));
  
  //set input as ADC2 (PIN 4)
  ADMUX |= (1<<MUX1);

  //enable ADC (ADEN) and enables ADC interrupt (ADIE)
  //Set ADATE to ensure interrupt can trigger at any time
  ADCSRA |= ((1 << ADPS1) | (1 << ADPS0));
  ADCSRA |= (1<<ADEN);

  while(1){
    //convert analog to digital
    ADCSRA |= ( 1 << ADSC );

    while( ADCSRA & ( 1 << ADIF ) );//wait for the value to be computed
   
    analogResult = ((ADCH<<8)|ADCL);
    //enable Pin 3 output if value is over threashold
    if(analogResult>THRESHOLD){
      PORTB |= (1<<PB3);
    }else{
      PORTB &= ~(1<<PB3);
    }    
    ADCSRA |= ( 1 << ADIF );
  }
  return 0;
}