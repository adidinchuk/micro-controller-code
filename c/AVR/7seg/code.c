     /* 8 bit lcd Demo */
// ------- Preamble -------- //
#define F_CPU 1000000L                      /* atTiny85 Bautrate */
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

void pulse_register(int pin, int t);
void my_delay_ms(int ms);
void init_7_seg(int DS, int CLK, int SHCP);

int DataSignalChannel; // shift register data signal
int ClockChannel; // artificial clock signal for shift register
int ShiftClockChannel; // data out signal for the shift register
//button number
int BTN = 0b00001000;

int PAUSE = 50;
int old_state = 0b00000000;

// 7 segment display encoding 0-9
const int DIGITS[10][8] = {
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

void init_7_seg(int DS, int CLK, int SHCP){
  DataSignalChannel = DS;
  ClockChannel = CLK;
  ShiftClockChannel = SHCP;
}  

int main(void) {
  init_7_seg(0b00000001, 0b00000010, 0b00000100);
  // -------- Inits --------- //
  DDRB |= 0b00000111;            /* Data Direction Register B:
                                   set first 3 pins as out. */
  int twos = 0b00000000;        /* 2s complement holder */

  while (1)  {    
    int i, x;
    for (i = 0; i < sizeof(DIGITS)/sizeof(DIGITS[0]); i++)   {
        for (x = 0; x < sizeof(DIGITS[i])/sizeof(DIGITS[i][0]); x++)     {
        if (DIGITS[i][x]){
            PORTB = PORTB | DataSignalChannel;
        }else{
                twos = ~DataSignalChannel;
                PORTB = PORTB & twos;
        }                 
        pulse_register(ClockChannel, PAUSE);
        }
        my_delay_ms(PAUSE);
        pulse_register(ShiftClockChannel, PAUSE);
        while ((PINB & BTN) == 0x00){}
    }
        
  }    
  return 0;                            /* This line is never reached */
}

// pulse function to pulse a pin
// pin - GPIO pin to pulse
// t - pulse delay between high and low
void pulse_register(int pin, int t){
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