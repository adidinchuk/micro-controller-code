/**
 * Example for Getting Started with nRF24L01+ radios. 
 *
 * This code is a sketch for an arduino this code is intended to test connectivity with python/nRF24/send.py and python/nRF24/response.py
 */
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 
RF24 radio(9,10);
int rec[10] = {};
uint64_t addresses[] = { 0xe7e7e7e7e7LL, 0xc2c2c2c2c2LL };

// Set up roles to simplify testing 

boolean role_ping_out = 1, role_pong_back = 0;   // The two different roles.
boolean role = role_pong_back;                                    // The main role variable, holds the current role identifier

void setup() {

  Serial.begin(9600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setPayloadSize(32);
  radio.setChannel(0x76);
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_LOW);
  radio.enableDynamicPayloads();
  radio.setRetries(25,25);                // Max delay between retries & number of retries

  //set channels to recieve
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void){
    

/****************** Ping Out Role ***************************/
  if (role == role_ping_out)  {
    unsigned long got_time = micros();
    radio.stopListening();                                    // First, stop listening so we can talk.
    printf("Now sending.. \n\r");

    char data[12] = "Hello there!";                             // Take the time, and send it.  This will block until complete
        
     if (!radio.write( &data, sizeof(data) )){  
      printf("Send failed.\n\r");  
     }
     
    unsigned long started_waiting_at = micros(); 
    radio.startListening();                                    // Now, continue listening

    // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        printf("Failed, response timed out.\n\r");
    }else{
      char ack[2];
      radio.read( &ack, sizeof(ack) );
      Serial.print("received ack payload is : ");
      printf("%.2s", ack);              

      printf("\nGot %.2s, round-trip delay: %lu microseconds\n\r", ack, micros()-got_time);
    }
    // Try again 1s later
    delay(1000);
  }

/****************** Pong Back Role ***************************/

  if ( role == role_pong_back )
  {
    radio.startListening();
    if( radio.available()){
      char data[22];                                     // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &data, sizeof(data) );             // Get the payload
        //printf("%.22s\n", data);
      }    

      char response[2] = "OK";
      // First, stop listening so we can talk   
      radio.writeAckPayload(1, &response, sizeof(response));

      //radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      //Serial.println("Sent response ");  
      //printf("%.6s\n", response);
      //Serial.println("\n\r"); 
   }else{
    //printf("No data \n\r");
   }
   delay(1000);
 }


/****************** Change Roles via Serial Commands ***************************/
/*
  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      role = role_ping_out;                  // Become the primary transmitter (ping out)
      radio.openWritingPipe(addresses[1]);
      radio.openReadingPipe(1,addresses[0]);
  
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");
      
       role = role_pong_back;                // Become the primary receiver (pong back)
       radio.openWritingPipe(addresses[0]);
       radio.openReadingPipe(1,addresses[1]);
    }
  }*/
}
