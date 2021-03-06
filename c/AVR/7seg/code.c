     /* 8 bit lcd Demo */
// ------- Preamble -------- //
#define F_CPU 1000000L                      /* atTiny85 Bautrate */
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */

void pulse_sr(int pin, int t);
void my_delay_ms(int ms);
void init_7_seg(int DS, int CLK, int SHCP);

int DataSignalChannel; // shift register data signal
int ClockChannel; // artificial clock signal for shift register
int ShiftClockChannel; // data out signal for the shift register
//button number
int BTN;
int PAUSE = 1;
int old_state = 0;

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

void init_7_seg(int DS, int CLK, int SHCP){
  DataSignalChannel = DS;
  ClockChannel = CLK;
  ShiftClockChannel = SHCP;
}

void push_to_sr(int data[8], int bit_count){ 
  int dp;
  for (dp = 0; dp < bit_count; dp++)     {
    if (data[dp])
      PORTB |= (1<<DataSignalChannel);
    else
      PORTB &= ~(1<<DataSignalChannel);                
    pulse_sr(ClockChannel, PAUSE);
  }    
}

int main(void) {
  BTN = 3;
  init_7_seg(0, 1, 2);
  // -------- Inits --------- //
  DDRB |= ((1<<DataSignalChannel)|(1<<ClockChannel)|(1<<ShiftClockChannel));
  /* Data Direction Register B: set first 3 pins as out. */
  while (1)  {
    int i;    
    for (i = 0; i < sizeof(digits)/sizeof(digits[0]); i++)   {
        push_to_sr(digits[i], sizeof(digits[i])/sizeof(digits[i][0]));        
        pulse_sr(ShiftClockChannel, PAUSE);
        while ((PINB & (1<<BTN)) == 0x00){}
    }
  }
  return 0;
}

// pulse function to pulse a pin
// pin - GPIO pin to pulse
// t - pulse delay between high and low
void pulse_sr(int pin, int t){
  old_state = PORTB;
  PORTB |= (1<<pin);
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