#!/usr/bin/python
import sys
import Adafruit_DHT
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

read_pin = 18

GPIO.setup(read_pin, GPIO.OUT)
GPIO.output(read_pin,GPIO.HIGH)

while True:
    print 'reading'
    humidity, temperature = Adafruit_DHT.read_retry(11, read_pin)
    print 'Temp: {0:0.1f} C  Humidity: {1:0.1f} %'.format(temperature, humidity)
