#!/usr/bin/python

import RPi.GPIO as GPIO

# Use BCM GPIO references instead of physical pin numbers
GPIO.setmode(GPIO.BCM)

# Define GPIO to use on Pi
GPIO_PIR = 17

# Set pin as input
GPIO.setup(GPIO_PIR,GPIO.IN)

Current_State  = 0
Previous_State = 0

try:

  print "Waiting for PIR to settle ..."

  # Loop until PIR output is 0
  while GPIO.input(GPIO_PIR)==1:
    Current_State  = 0    

  print "  Ready monitoring"     
    
  # Loop until users quits with CTRL-C
  while True :
   
    # Read PIR state
    Current_State = GPIO.input(GPIO_PIR)
   
    if Current_State==1 and Previous_State==0:
      # PIR is triggered
      print "  Motion detected!"

      # Record previous state
      Previous_State=1
    elif Current_State==0 and Previous_State==1:
      # PIR has returned to ready state
      print "  Ready monitoring"
      Previous_State=0
      
except KeyboardInterrupt:
  print "  Quit" 
  # Reset GPIO settings
  GPIO.cleanup()

