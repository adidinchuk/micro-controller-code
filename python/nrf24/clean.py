import RPi.GPIO as GPIO  

# the rest of your code would go here 

GPIO.setmode(GPIO.BCM);
GPIO.setup(22, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(10, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(9, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(11, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(25, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)
GPIO.setup(8, GPIO.IN, pull_up_down = GPIO.PUD_DOWN)


# when your code ends, the last line before the program exits would be...  
GPIO.cleanup() 

# remember, a program doesn't necessarily exit at the last line! 
