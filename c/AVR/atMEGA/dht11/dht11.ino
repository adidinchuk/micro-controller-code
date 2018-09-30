//
//    FILE: dht_test.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.07
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//     URL: http://arduino.cc/playground/Main/DHTLib
//
// Released to the public domain
//
#include <SoftwareSerial.h>
#include "dht.h"

dht DHT;

#define DHT11_PIN 3
#define RX    0   // (PCINT16/RXD) PD0
#define TX    1   // (PCINT17/TXD) PD1
#define LED_PIN  2 // *** (PCINT18/INT0) PD2 ***///

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

SoftwareSerial Monitor(RX, TX);

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop()
{
  
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
 // DISPLAY DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);
  unsigned long got_time = micros();
        const char temp[12] = "HI";
        const char humi[12] = "BUY";
        const char tmp[100] = "BUY";
        const char* temp_str = "{temp:";
        const char* humi_str = ",humi:";
        const char* date_str = ",hash:";
        const char* term = "}";
        strcpy(temp, String(DHT.temperature, 1).c_str());
        strcpy(humi, String(DHT.humidity, 1).c_str());
        strcpy(tmp, String(got_time).c_str());
        char date[5]; 
        strncpy(date, tmp, 5);
        char data[100];
        //data = malloc(60); /* make space for the new string (should check the return value ...) */
        strcpy(data, temp_str); /* copy name into the new var */
        strcat(data, temp); /* copy name into the new var */
        strcat(data, humi_str); /* copy name into the new var */
        strcat(data, humi); /* copy name into the new var */
        strcat(data, date_str); /* copy name into the new var */
        strcat(data, date); /* copy name into the new var */
        strcat(data, term); /* copy name into the new var */
  
  Serial.println(data);

  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}
//
// END OF FILE
//
