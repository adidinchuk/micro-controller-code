import RPi.GPIO as gpio
from time import sleep 
gpio.setmode(gpio.BCM)

# shift pin
SHIFT = 26

gpio.setup(SHIFT, gpio.OUT)
gpio.output(SHIFT, gpio.HIGH)

ONE = 1
ZERO = 15
PAUSE = 50

def pulse_low(pin, t):
	gpio.output(pin, 0)
    sleepM
    gpio.output(pin, 1)
	
	
	return

try:
	while True:
        pulse(SHIFT, )


except KeyboardInterrupt:
	gpio.cleanup()
