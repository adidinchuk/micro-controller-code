/*
This is a sample script to allow for an atMega chip to communicate over RX/TX
This sample was testing with a ELEGOO UNO R3

NOTES:
1. Ensure atmega RX is connected to the RX arduino pin
2. Ensure atmega RT is connected to the TX arduino pin
3. Ensure arduino is set as arduinoISP (not as ISP!)
*/

#include <SoftwareSerial.h>

#define RX    0   // (PCINT16/RXD) PD0
#define TX    1   // (PCINT17/TXD) PD1
#define LED_PIN  2 // *** (PCINT18/INT0) PD2 ***///

SoftwareSerial Monitor(RX, TX);

void setup() {
  
Monitor.begin(9600); 
    pinMode(2, OUTPUT);
    pinMode(TX, OUTPUT);
    pinMode(RX, INPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  Monitor.println("LED on");
  delay(1000);
  digitalWrite(2, LOW);
  Monitor.println("LED off");
  delay(1000);
}
