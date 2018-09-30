# this program is intended to run with the ../c/AVR/nRF24/send/send.ino

import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev
import datetime
GPIO.setmode(GPIO.BCM)

#set up pipes
pipes = [[0xe7, 0xe7, 0xe7, 0xe7, 0xe7], [0xc2, 0xc2, 0xc2, 0xc2, 0xc2]]

radio = NRF24(GPIO, spidev.SpiDev())
# ce pin 0 and csn pin 22 (GPIO)
radio.begin(0, 22)

radio.setPayloadSize(32)

radio.setChannel(0x76)
#Set data rate, CRC length and power level (this should match the reciever)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)
radio.setCRCLength(NRF24.CRC_16)
radio.setAutoAck(True)
radio.enableDynamicPayloads()
#radio.enableAckPayload()

radio.openReadingPipe(1, pipes[1])
radio.printDetails()
radio.startListening()

while(1):
    while not radio.available():
        time.sleep(1 / 100)    
    receivedMessage = []
    radio.read(receivedMessage, radio.getDynamicPayloadSize())
    print("Received: {}".format(receivedMessage))
    print("Translating the receivedMessage into unicode characters")
    string = ""
    for n in receivedMessage:
        # Decode into standard unicode set
        if (n >= 32 and n <= 126):
            string += chr(n)
    now = datetime.datetime.now()
    #print data to a file (testing purposes)
    with open("test.txt", "a") as myfile:
        myfile.write(now.strftime("%Y-%m-%d %H:%M:%S")  + " : Out received message decodes to: {}".format(string) + $
    print("Out received message decodes to: {}".format(string))
