#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>

using namespace std;

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

// Radio pipe addresses for the 2 nodes to communicate.
const uint8_t addr[6] = "00001";

int main(int argc, char** argv){
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1, addr);
  radio.startListening();

  // Dump the configuration of the rf unit for debugging
  radio.printDetails();

  int count = 0;

  while (1)
  {
    while ( radio.available() )
    {
      radio.read( &count, sizeof(count) );
      printf("Count: %d\n", count);
    }
    sleep(1);
  }

  return 0;
}