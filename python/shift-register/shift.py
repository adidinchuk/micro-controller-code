import RPi.GPIO as gpio
from time import sleep 
gpio.setmode(gpio.BCM)

DS = 16
CLK = 20
SHCP = 21

gpio.setup(DS, gpio.OUT)
gpio.setup(CLK, gpio.OUT)
gpio.setup(SHCP, gpio.OUT)

gpio.output(DS, gpio.LOW)
gpio.output(CLK, gpio.LOW)
gpio.output(SHCP, gpio.LOW)

pulse_time = 0.01

data = [[1, 1, 1, 1, 1, 1, 0],
[0, 1, 1, 0, 0, 0, 0],
[1, 1, 0, 1, 1, 0, 1],
[1, 1, 1, 1, 0, 0, 1],
[0, 1, 1, 0, 0, 1, 1],
[1, 0, 1, 1, 0, 1, 1],
[1, 0, 1, 1, 1, 1, 1],
[1, 1, 1, 0, 0, 0, 0],
[1, 1, 1, 1, 1, 1, 1],
[1, 1, 1, 1, 0, 1, 1]]

def pulse(pin, t):
	gpio.output(pin, 1)
	sleep(t)
	gpio.output(pin, 0)
	return

try:
	while True:
		pulse(CLK, pulse_time)
		for i in data:
                        print(i)
			for x in i:
				gpio.output(16, x)
				pulse(CLK, pulse_time)
   			sleep(0.1)
			pulse(SHCP, pulse_time)

except KeyboardInterrupt:
	gpio.cleanup()
