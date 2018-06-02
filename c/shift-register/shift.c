#include <wiringPi.h>

void pulse(int pin, double t){
  digitalWrite (pin, HIGH) ;
  delay (t) ;
  digitalWrite (pin, LOW);
}

int main (void)
{
  int DS = 4;
  int  CLK = 5;
  int SHCP = 6;
  double pulseTime = 0.01 ;
  wiringPiSetup () ;
  int data[10][7] = {
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
  printf("%d, World\n", sizeof(data)/sizeof(data[0])); 
  pinMode (DS, OUTPUT) ;
  pinMode (CLK, OUTPUT) ;
  pinMode (SHCP, OUTPUT) ;
  for (;;)
  {
    pulse(CLK, 50) ;
   for (int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
   {
     for (int x = 0; x < sizeof(data[i])/sizeof(data[i][0]); x++)
     {
       digitalWrite (DS, data[i][x]);
       pulse(CLK, 50);
     }
     delay(50);
     pulse(SHCP, 50);
   }
  }
  return 0 ;
}
