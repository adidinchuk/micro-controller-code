 /*
 * dht11.c:
 * Simple test program to read serial stream from a DHT11 sensor
 * Author: Andrew Didinchuk <adidinchuk@gmail.com>
 ***********************************************************************
 * This file is part of wiringPi:
 ***********************************************************************
*/

#include <SoftwareSerial.h>

#define MAXTIMINGS  85
// DHT data pin
#define DHT   1
#define RX    3   // *** D3, Pin 2
#define TX    4   // *** D4, Pin 3

SoftwareSerial Serial(RX, TX);
/* 8bit integral humidity data
 * 8bit decimal humidity data
 * 8bit integral temp data
 * 8bit decimal temp data
 * 8bit check sum
*/
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

int read_dht11_dat()
{
  uint8_t laststate = HIGH;
  uint8_t counter   = 0;
  uint8_t j   = 0, i;
  float fahrenheit;
  
  // clear data storage
  dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

  // pull pin down for 18 milliseconds to request data
  pinMode( DHT, OUTPUT );
  digitalWrite( DHT, LOW );
  delay( 18 );
    // pull pin high for response data
  digitalWrite( DHT, HIGH );
  delayMicroseconds( 40 );
  // read pin
  pinMode( DHT, INPUT );
  
  for ( i = 0; i < MAXTIMINGS; i++ ){
    counter = 0;
    // detect high signal duration length ( if > 50um --> 1, else --> 0)
    while ( digitalRead( DHT ) == laststate ){
      counter++;
      delayMicroseconds( 1 );
      // break on constant high
      if ( counter == 255 ){
        Serial.print("\nError, constand high...");
        break;
      }
    }
    // update current signal state
    laststate = digitalRead( DHT );
    // break on constant high
    if ( counter == 255 )
      break;

    // DHT start signal is 80us low, 80us down
    // every bit starts with 50us low 
    // ignore first 3 transitions and every start bit read signal (50um low)
    if ( (i >= 4) && (i % 2 == 0) ){
      // left shift bit and update padded 0 with a 1 if signal > 50um
      dht11_dat[j / 8] <<= 1;
      if ( counter > 50 ){
        Serial.print("1");
        dht11_dat[j / 8] |= 1;
      }else{
        Serial.print("0");
      }
      j++;
    }
  }

  /*
   * Ensure that a total of 40 bits were processed
   * Validate checksum 
   */
  if ( (j >= 40) && 
       (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ){
    return 1;    
  }else{
    Serial.print("\nFAIL..");
    return 0;
  }
}

void setup(){  
  digitalWrite(0, LOW);
  delay( 500 ); /* wait 1sec to refresh */
  digitalWrite(0, HIGH);
  delay( 500 ); /* wait 1sec to refresh */ 
  pinMode(0, OUTPUT);  
  Serial.begin(9600);
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  Serial.println("Starting data collection...");
  return 0;
}

void loop()
{
  digitalWrite(0, LOW);
  delay( 3000 ); /* wait 1sec to refresh */
  read_dht11_dat();   
  Serial.print("\nHumid : ");
  Serial.print(dht11_dat[0]);
  Serial.print(".");
  Serial.print(dht11_dat[1]);
  Serial.print(" - Temp : ");  
  Serial.print(dht11_dat[2]);
  Serial.print(".");
  Serial.print(dht11_dat[3]);
}
