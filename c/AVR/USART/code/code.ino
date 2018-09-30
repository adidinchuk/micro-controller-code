#include <SoftwareSerial.h>

#define RX    3   // *** D3, Pin 2
#define TX    4   // *** D4, Pin 3

SoftwareSerial Monitor(RX, TX);

void setup()
{
  Serial.begin(9600);
  pinMode(0, OUTPUT);
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  // ***
  // *** Initialize the Serial port
  // ***
  
  Serial.println("Dallas Temperature IC Control Library Initializing...");
  // ***
  // *** Show a message that setup has completed.
  // ***
  Serial.println("Initialization complete.");
}

void loop()
{
  digitalWrite(0, HIGH);
  Serial.print("hello, ON");
  
  digitalWrite(0, LOW);
  Serial.print("hello, OFF");
  delay(2000);
}