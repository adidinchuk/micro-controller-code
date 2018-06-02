#include <wiringPi.h>

void pinHandler1 (void);
void printChannel (int c);

int IN = 0;
int DS = 4;
int CLK = 5;
int SHCP = 6;

int channel = 0;
int minChannel = 0;
int maxChannel = 9;

double pulseTime = 5;

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

void pulse (int pin, double t){
   digitalWrite (pin, HIGH);
   delay(t);
   digitalWrite (pin, LOW);
}

int main (void){
   printf ("Raspberry Pi Gertboard Button Test\n") ;
   wiringPiSetup ();
   pinMode(IN, INPUT);
   pinMode (DS, OUTPUT) ;
   pinMode (CLK, OUTPUT) ;
   pinMode (SHCP, OUTPUT) ;
   printChannel(channel);
   pullUpDnControl(IN, PUD_DOWN);
   wiringPiISR(IN, INT_EDGE_RISING, pinHandler1);
   while(1){}
}

void pinHandler1 (void){
   if (digitalRead(IN) == HIGH){
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

void printChannel(int c){
   pulse(CLK, pulseTime);
   for (int x = 0; x < sizeof(digits[c])/sizeof(digits[c][0]); x++)
   {
        digitalWrite (DS, digits[c][x]);
	pulse(CLK, pulseTime);
   }
   delay(pulseTime);
   pulse(SHCP, pulseTime);
   printf ("Current channel: %i\n", c);
}
