/*
 * button.c:
 * listen on a pin for a rising edge, increment a counter and push the 
 * counter's value through a shift register connected to a 7 segment
 * Author: Andrew Didinchuk <adidinchuk@gmail.com>
 ***********************************************************************
 ***********************************************************************

*/
#include <wiringPi.h>

void pinHandler (void);
void printChannel (int c);
void pulse (int pin, double t);


// button pin - BCM 17
int BTN_IN = 0;
// shift register data signal
int DS = 4;
// artificial clock signal for shift register
int CLK = 5;
// data out signal for the shift register
int SHCP = 6;

//--- channel settings ---//
int channel = 0;
int minChannel = 0;
int maxChannel = 9;
//--- end ---//

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
   printf ("Raspberry Pi button test \n") ;
   wiringPiSetup ();
   pinMode(BTN_IN, INPUT);
   pinMode (DS, OUTPUT) ;
   pinMode (CLK, OUTPUT) ;
   pinMode (SHCP, OUTPUT) ;
   printChannel(channel);
   // pull button pin down and listen for an interrupt
   pullUpDnControl(BTN_IN, PUD_DOWN);
   wiringPiISR(BTN_IN, INT_EDGE_RISING, pinHandler);
   while(1){}
}

// pulse function to pulse a pin
// pin - GPIO pin to pulse
// t - pulse delay between high and low
void pulse (int pin, double t){
   digitalWrite (pin, HIGH);
   delay(t);
   digitalWrite (pin, LOW);
}

// button click handler to increment channel
void pinHandler (void){
    // increment channel on click
   if (digitalRead(BTN_IN) == HIGH){
        channel++;
        if (channel > maxChannel){
	        channel = minChannel;
        }
        printChannel(channel);
        delay(300);
   }else{
	delay(300);
   }
}

// print the value passed to screen and push through the register
void printChannel(int c){
   //register logic
   pulse(CLK, pulseTime);
   for (int x = 0; x < sizeof(digits[c])/sizeof(digits[c][0]); x++){
        digitalWrite (DS, digits[c][x]);
	pulse(CLK, pulseTime);
   }
   delay(pulseTime);
   pulse(SHCP, pulseTime);
   printf ("Current channel: %i\n", c);
}
