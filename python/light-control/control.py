'''
 * light-control/control.py:
 * Controller for a LED array. The system is intended to use a switch for the light control
 * in case the Raspberry Pi wattage or voltage is insufficient to power the array. 
 * The system also allows for a single light detecotr to disable lights when external light
 * sources are present.
 * Author: Andrew Didinchuk <adidinchuk@gmail.com>
 ***********************************************************************
'''

#!/usr/bin/python
import RPi.GPIO as GPIO
import time
from datetime import datetime
from pytz import timezone


''' settings '''
# light sensor detector pin
detector_pin = 19
# relay/switch control pin
relay_pin = 14
# minimum light threshold (photoresistor)
min_light = 110
delay = 0.1
# diable photoresistor input
always_on = True

#light on time ranges (5AM - 7AM EST)
start_time = datetime.strptime("1900-01-01 05:00:00", "%Y-%m-%d %H:%M:%S")
end_time = datetime.strptime("1900-01-01 19:00:00", "%Y-%m-%d %H:%M:%S")
tz = 'EST'

GPIO.setmode(GPIO.BCM)
GPIO.setup(relay_pin, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(detector_pin, GPIO.LOW)

# light state (ON/OFF)
light = 1

while(True):
        count = 0
        if not always_on:
            # Reset pin and pull down
            GPIO.setup(detector_pin, GPIO.OUT)
            GPIO.output(detector_pin, GPIO.LOW)
            time.sleep(delay)            
            GPIO.setup(detector_pin, GPIO.IN)

            #Count until the pin goes high to guage light intensty
            while (GPIO.input(detector_pin) == GPIO.LOW):
                    count += 1
        
        # turn on relay/switch
        if((count > min_light or always_on) and (datetime.now(timezone(tz)).time() > start_time.time() and datetime.now(timezone(tz)).time() < end_time.time()) and light == 0):
            print("turning on light....")
            GPIO.output(relay_pin,GPIO.LOW)
            light = 1

        # turn off relay/switch 
        elif (light == 1 and ((count < min_light and not always_on) or (datetime.now(timezone(tz)).time() < start_time.time() or datetime.now(timezone(tz)).time() > end_time.time()))):
            print("turning off light....")
            GPIO.output(relay_pin,GPIO.HIGH)
            light = 0
