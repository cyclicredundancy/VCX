#!/cygdrive/d/Python27/python2.7 -u

#NOTE: using unbuffered IO with -u arg above

import serial
import time
import sys

print "[$Revision: $]\n";
print "About to open handle to serial port, your board will get reset.\n"
time.sleep(1)

ser = serial.Serial(port='COM6',baudrate='9600')

settings = ser.getSettingsDict()

print "Connection details:\n", settings

#Use this to modify settings on the fly, if allowed
#settings['timeout'] = 0;
#ser.applySettingsDict(settings)



while (1):
    data = ser.readline()
    if (data) :
        print "Received " , len(data) , " chars/bytes: \n" + data



print "all done\n"
ser.close()


class CellularTransitionTransaction:
    "defines a state change transaction with the expected message, its response message and the next state"
    def __init__(self):
        print "Class CellularTransitionTransaction ready\n"
    "if we match this regex then we transition to the nextState"
    expectedMessage = None
    "when transitioning to the nextState we send this response"
    transitionResponse = None
    "this is a reference to the SerialCellularState object for the next expected input state"
    nextState = None


class SerialCellularState:
    "defines the state entries managed by the SerialCellularStateMachine"
    def __init__(self):
        print "Class SerialCellularState ready\n"
    "this is a list of CellularTransitionTransaction objects for this state"
    transactionContracts = None 


class SerialCellularStateMachine:
    "implement a state machine to communicate with the SM5100B-D cellular modem over a serial port"
    def __init__(self):
        print "Class SerialCellularStateMachine ready\n"
    currentState = None
    allStates = None


#import yaml
#f = open('tree.yaml')
#dataMap = yaml.load(f)
#f.close()
#
#f = open('newtree.yaml', "w")
#yaml.dump(dataMap, f)
#f.close()


