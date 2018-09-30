#include "nRF24L01.h"
#include "RF24.h"
#include <SoftwareSerial.h>
#include "dht.h"
//arduino only
//#include "printf.h"
dht DHT;
RF24 radio(9,10); //atmega
//set channels
uint64_t addresses[] = { 0xe7e7e7e7e7LL, 0xc2c2c2c2c2LL };
#define DHT11_PIN 1
#define RX    3   // *** D3, Pin 2
#define TX    4   // *** D4, Pin 3
#define LED_PIN  2 // *** (PCINT18/INT0) PD2 ***///

// Set up roles to simplify testing 
boolean role_ping_out = 1, role_pong_back = 0;   // The two different roles.
boolean role = role_ping_out;                                 // The main role variable, holds the current role identifier
SoftwareSerial Monitor(RX, TX);
void setup() {
    //  --- arduino only ----
    Monitor.begin(9600);
    //Serial.begin(9600);
    //printf_begin();
    // Setup and configure rf radio
    pinMode(RX, INPUT);
    pinMode(TX, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    Monitor.println("\n\rRF24/examples/GettingStarted/\n\r");
    Monitor.println("*** PRESS 'T' to begin transmitting to the other node\n\r");
    radio.begin();                          // Start up the radio
    radio.setAutoAck(1);                    // Ensure autoACK is enabled
    radio.setPayloadSize(32);
    radio.setChannel(0x76);
    radio.enableAckPayload();
    radio.setPALevel(RF24_PA_MIN);
    radio.setRetries(25,25);                // Max delay between retries & number of retries
    //set channels to recieve
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);

    radio.startListening();                 // Start listening
    //radio.printDetails();                   // Dump the configuration of the rf unit for debugging
    if ( role == role_ping_out ){    
        // Become the primary transmitter (ping out)
        radio.openWritingPipe(addresses[1]);
        radio.openReadingPipe(1,addresses[0]);  
    }else if ( role == role_pong_back ){
        // Become the primary receiver (pong back)
        radio.openWritingPipe(addresses[0]);
        radio.openReadingPipe(1,addresses[1]);
    }
}

void loop() {
    //read the DHT11 chip data
    int chk = DHT.read11(DHT11_PIN);
    switch (chk){
        case DHTLIB_OK:  
            Monitor.println("OK,\t"); 
            break;
        case DHTLIB_ERROR_CHECKSUM: 
            Monitor.println("Checksum error,\t\n"); 
            break;
        case DHTLIB_ERROR_TIMEOUT: 
            Monitor.println("Time out error,\t\n"); 
            break;
        default: 
            Monitor.println("Unknown error,\t\n"); 
            break;
    }
    unsigned long got_time = micros();
    //currently the date is used to confirm that the RX register has a new package 
    if (role == role_ping_out){
        radio.stopListening(); 
        unsigned long got_time = micros();
        //define data variables and placeholders
        const char temp[12] = "";
        const char humi[12] = "";
        const char tmp[100] = "";
        //initialize response structure {t:t, h:h, d:d}
        // packet size has to be <= 32 bytes!
        const char* temp_str = "{t:'";
        const char* humi_str = "',h:'";
        const char* date_str = "',c:'";
        const char* term = "'}";
        //check to see if the DHT11 data has been read correctly (-999 value if not)
        if(DHT.humidity > 0){
          strcpy(temp, String(DHT.temperature, 1).c_str());
          strcpy(humi, String(DHT.humidity, 1).c_str());
        }else{
          strcpy(temp, "n//a");
          strcpy(humi, "n//a");
        }        
        strcpy(tmp, String(got_time).c_str());
        char date[5]; 
        strncpy(date, tmp, 5);
        char data[100];
        //compile the response string (doing it the long way while I learn c)
        strcpy(data, temp_str); 
        strcat(data, temp); 
        strcat(data, humi_str); 
        strcat(data, humi); 
        strcat(data, date_str); 
        strcat(data, date); 
        strcat(data, term);      
        //send the data packet
        if (!radio.write(&data, sizeof(data))){  
          Monitor.println("Send failed.\n\r");  
        }else{
          Monitor.println("Success.\n\r");  
        }
    }
    //flip LED (debug)
    digitalWrite(LED_PIN, HIGH);
    delay(10000);
    digitalWrite(LED_PIN, LOW);
 
}
