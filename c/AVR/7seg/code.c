                                                 /* 8 bit lcd Demo */

// ------- Preamble -------- //
#define F_CPU 1000000L                      /* atTiny85 Bautrate */
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

void pulse (int pin, int t);
void my_delay_ms(int ms);

// shift register data signal
int DS = 0b00000001;
// artificial clock signal for shift register
int CLK = 0b00000010;
// data out signal for the shift register
int SHCP = 0b00000100;
//button number
int BTN = 0b00001000;


int PAUSE = 50;
int old_state = 0b00000000;


// 7 segment display encoding 0-9
int digits[10][8] = {
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 0, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 1, 0, 0, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 0, 1, 1}
  };

int main(void) {

  // -------- Inits --------- //
  DDRB |= 0b00000111;            /* Data Direction Register B:
                                   set first 3 pins as out. */
  int twos = 0b00000000;        /* 2s complement holder */

  while (1)  {
    //pulse(CLK, PAUSE) ;
    int i, x;
    for (i = 0; i < sizeof(digits)/sizeof(digits[0]); i++)   {
        for (x = 0; x < sizeof(digits[i])/sizeof(digits[i][0]); x++)     {
        if (digits[i][x]){
            PORTB = PORTB | DS;
        }else{
                twos = ~DS;
                PORTB = PORTB & twos;
        }                 
        pulse(CLK, PAUSE);
        }
        my_delay_ms(PAUSE);
        pulse(SHCP, PAUSE);
        while ((PINB & BTN) == 0x00){}
    }
        
  }    
  return 0;                            /* This line is never reached */
}

// pulse function to pulse a pin
// pin - GPIO pin to pulse
// t - pulse delay between high and low
void pulse(int pin, int t){
  old_state = PORTB;
  PORTB = PORTB | pin;          
  my_delay_ms(t);
  PORTB = old_state;
}

void my_delay_ms(int ms)
{
  while (0 < ms)
  {  
    _delay_ms(1);
    --ms;
  }
}