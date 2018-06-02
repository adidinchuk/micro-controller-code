#include <wiringPi.h>

void pulse (int pin, double t);

// shift register data signal
int DS = 4;
// artificial clock signal for shift register
int CLK = 5;
// data out signal for the shift register
int SHCP = 6;

// pulse time for shift register
double pulseTime = 5;

// 7 segment display encoding 0-9
int digits[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0},
    {1, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 1, 1},
    {1, 0, 1, 1, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1}
  };

int main (void){
  wiringPiSetup () ;
  printf("%d, World\n", sizeof(digits)/sizeof(digits[0])); 
  pinMode (DS, OUTPUT) ;
  pinMode (CLK, OUTPUT) ;
  pinMode (SHCP, OUTPUT) ;
  for (;;)  {
   pulse(CLK, 50) ;
   for (int i = 0; i < sizeof(digits)/sizeof(digits[0]); i++)   {
     for (int x = 0; x < sizeof(digits[i])/sizeof(digits[i][0]); x++)     {
       digitalWrite (DS, data[i][x]);
       pulse(CLK, 50);
     }
     delay(50);
     pulse(SHCP, 50);
   }
  }
  return 0 ;
}

// pulse function to pulse a pin
// pin - GPIO pin to pulse
// t - pulse delay between high and low
void pulse(int pin, double t){
  digitalWrite (pin, HIGH) ;
  delay (t) ;
  digitalWrite (pin, LOW);
}