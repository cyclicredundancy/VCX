from __future__ import print_function
import time
from pololu_drv8835_rpi import motors, MAX_SPEED

import sys

motors.setSpeeds(0, 0)

#import pygame, time
#from pygame.locals import *
#
#pygame.display.init()
#pygame.init()
##screen = pygame.display.set_mode((640, 480))
##pygame.display.set_caption('Pygame Keyboard Test')
##pygame.mouse.set_visible(0)
#
#
#print("doing a function")
#while True:
#    for event in pygame.event.get():
#        if event.type==QUIT:
#            exit()
#        if (event.type == KEYUP) or (event.type == KEYDOWN):
#            print("key pressed")
#            time.sleep(0.1)
#        if pygame.key.get_focused():
#            press=pygame.key.get_pressed()
#            for i in xrange(0,len(press)): 
#                if press[i]==1:
#                    name=pygame.key.name(i)
#            print("key pressed")
#
#        time.sleep(0.1)


import readchar
#c = readchar.readchar()
#key = readchar.readkey()
print("Type x to exit!!!")
while True:
    c = readchar.readchar()
    if c == 'x':
        exit()


# Set up sequences of motor speeds.
#test_forward_speeds = list(range(0, MAX_SPEED, 1)) + \
#  [MAX_SPEED] * 200 + list(range(MAX_SPEED, 0, -1)) + [0]  
#
#test_reverse_speeds = list(range(0, -MAX_SPEED, -1)) + \
#  [-MAX_SPEED] * 200 + list(range(-MAX_SPEED, 0, 1)) + [0]  

#print ''.join(test_forward_speeds)
#print ''.join(test_reverse_speeds)

sys.stdout.write("MAX_SPEED is %d MIN_SPEED is -%d\n" % (MAX_SPEED, MAX_SPEED))


try:
    for x in range(len(test_forward_speeds)):
        motors.motor1.setSpeed(test_forward_speeds[x])
        motors.motor2.setSpeed(test_reverse_speeds[x])
        time.sleep(0.005)

    print("")
    time.sleep(1)
    motors.motor1.setSpeed(100)
    motors.motor2.setSpeed(-100)
    time.sleep(3)
    motors.motor1.setSpeed(200)
    motors.motor2.setSpeed(-200)
    time.sleep(3)
    motors.motor1.setSpeed(400)
    motors.motor2.setSpeed(-400)
    time.sleep(3)
    motors.motor1.setSpeed(480)
    motors.motor2.setSpeed(-480)
    time.sleep(3)


finally:
    # Stop the motors, even if there is an exception
    # or the user presses Ctrl+C to kill the process.
    motors.setSpeeds(0, 0)
    print("")
