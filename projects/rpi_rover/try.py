# python script to control Raspberry Pi Rover made using Pololu DRV8835 Dual Motor Driver Kit
# https://www.pololu.com/product/2753

from __future__ import print_function
import time
from pololu_drv8835_rpi import motors, MAX_SPEED

motors.setSpeeds(0, 0)

import sys

sys.stdout.write("MAX_SPEED is %d MIN_SPEED is -%d\n" % (MAX_SPEED, MAX_SPEED))

import readchar
#c = readchar.readchar()
#key = readchar.readkey()
print("Type x to exit!!!")

# Both motors rotate clockwise (looking into the shaft) with +ve values
# 1 is left motor 
speed1=0
# 2 is right motor
speed2=0


try:
    print("")

    count = 0
    while True:
        count += 1
        c = readchar.readchar()
        if c == 'x':
            exit()
        # quickly reset motors using o or r
        if c == 'r':
            speed1 =0
            speed2 =0
        if c == 'o':
            speed1 =0
            speed2 =0
        # debugging keys using qa and ed
        if c == 'q':
            speed1 +=20
        if c == 'a':
            speed1 -=20
        if c == 'e':
            speed2 +=20
        if c == 'd':
            speed2 -=20
        # steering wheel like controls using ijkl cluster.
        if c == 'i':
            speed1 +=20
            speed2 -=20
        if c == 'k':
            speed2 +=20
            speed1 -=20
        if c == 'j':
            speed1 -=20
            speed2 -=20
        if c == 'l':
            speed1 +=20
            speed2 +=20
        # clamp speed at max values
        if (-MAX_SPEED > speed1 or speed1 > MAX_SPEED):
            speed1 = speed1/abs(speed1)*MAX_SPEED
        if (-MAX_SPEED > speed2 or speed2 > MAX_SPEED):
            speed2 = speed2/abs(speed2)*MAX_SPEED
        motors.motor1.setSpeed(speed1)
        motors.motor2.setSpeed(speed2)
        time.sleep(0.05)
        sys.stdout.write("(%d,%d)\n" % (speed1, speed2))
 

finally:
    # Stop the motors, even if there is an exception
    # or the user presses Ctrl+C to kill the process.
    motors.setSpeeds(0, 0)
    print(".")
