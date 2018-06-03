
#include <wiringPi.h>

void pulse (int pin, double t);

// shift register data signal
int SHIFT = 7;

// sleep timing
int ONE = 1;
int ZERO = 15;
int PAUSE = 50;

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

//testing output
int main (void){
  wiringPiSetup () ;
  pinMode (SHIFT, OUTPUT) ;
  digitalWrite (SHIFT, HIGH) ;
  //delay (ZERO);
  //digitalWrite (SHIFT, LOW);  
  //delay (ZERO);
  //digitalWrite(SHIFT, HIGH);
  
  for (;;)  {  
    for (int i = 0; i < sizeof(digits)/sizeof(digits[0]); i++){
      for (int x = 0; x < sizeof(digits[i])/sizeof(digits[i][0]); x++){
        if ( digits[i][x] == 1 )
          pulse_low(SHIFT, ONE);
        else 
          pulse_low(SHIFT, ZERO);
        delayMicroseconds (PAUSE) ;      
      }
      delay(500);      
    }
  }

}

// pulse function to pulse a pin
// pin - GPIO pin to pulse
// t - pulse delay between high and low
void pulse_low(int pin, int t){
  digitalWrite (pin, LOW);
  //printf("low");
  //delay( t);
  delayMicroseconds (t) ;
  digitalWrite (pin, HIGH) ; 
}
