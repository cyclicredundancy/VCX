#!/usr/bin/env python2.7  

# vdhar: copied in parts from:
# http://RasPi.tv/how-to-use-interrupts-with-python-on-the-raspberry-pi-and-rpi-gpio-part-3  

import RPi.GPIO as GPIO  
GPIO.setmode(GPIO.BCM)  

GPIO.setup(17, GPIO.IN)  
  
# now we'll define two threaded callback functions  
# these will run in another thread when our events are detected  
def my_callback(channel):  
    print "falling edge detected on 17"  
  
def my_callback2(channel):  
    print "falling edge detected on 23"  
  
raw_input("Press Enter when ready\n>")  
  
##GPIO.add_event_detect(17, GPIO.FALLING, callback=my_callback, bouncetime=300)  
#GPIO.add_event_detect(17, GPIO.RISING, callback=my_callback2, bouncetime=300)

try:  
    while True :
        print "Waiting for rising edge on port 17"  
        GPIO.wait_for_edge(17, GPIO.RISING)  
        print "Rising edge detected on port 17. Here endeth the third lesson."  
  
except KeyboardInterrupt:  
    GPIO.cleanup()       # clean up GPIO on CTRL+C exit  
GPIO.cleanup()           # clean up GPIO on normal exit  
