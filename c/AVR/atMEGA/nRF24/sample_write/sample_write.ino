#include "nRF24L01.h"
#include "RF24.h"
//arduino only
//#include "printf.h"

//RF24 radio(9,10); //atmega
RF24 radio(3,4); //arduino
uint64_t addresses[] = { 0xe7e7e7e7e7LL, 0xc2c2c2c2c2LL };

// Set up roles to simplify testing 
boolean role_ping_out = 1, role_pong_back = 0;   // The two different roles.
boolean role = role_ping_out;                                 // The main role variable, holds the current role identifier

void setup() {
  /**  --- arduino only ----
  Serial.begin(9600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");
  **/
  
  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setPayloadSize(32);
  radio.setChannel(0x76);
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(25,25);                // Max delay between retries & number of retries
  //set channels to recieve
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  if ( c == 'T' && role == role_pong_back ){    
    role = role_ping_out;                  // Become the primary transmitter (ping out)
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);  
  }else if ( c == 'R' && role == role_ping_out ){
    role = role_pong_back;                // Become the primary receiver (pong back)
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
}

void loop() {
  unsigned long got_time = micros();
  if (role == role_ping_out){
    radio.stopListening(); 
    const char text[] = "Hello World";
    if (!radio.write(&got_time, sizeof(got_time))){  
      printf("Send failed.\n\r");  
    }  
    delay(1000); 
  }
}