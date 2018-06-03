
#include <wiringPi.h>

void pulse (int pin, double t);

// shift register data signal
int SHIFT = 7;

// sleep timing
int ONE = 1;
int ZERO = 15;
int PAUSE = 50;



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
  //for (int x = 0; x < 8; x++){
    pulse_low(SHIFT, ONE);
    delayMicroseconds (PAUSE) ;
    pulse_low(SHIFT, ZERO);
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
