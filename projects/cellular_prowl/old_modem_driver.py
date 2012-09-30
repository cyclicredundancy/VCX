#!/cygdrive/d/Python27/python2.7 -u

#NOTE: using unbuffered IO with -u arg above

import pprint
import jsonpickle
import serial
import time
import sys
import yaml
import re

print "[$Revision: $]\n";

#print "About to open handle to serial port, your board will get reset.\n"
#time.sleep(1)
#
#ser = serial.Serial(port='COM6',baudrate='9600')
#
#settings = ser.getSettingsDict()
#
#print "Connection details:\n", settings
#
##Use this to modify settings on the fly, if allowed
##settings['timeout'] = 0;
##ser.applySettingsDict(settings)
#
#
#
#while (1):
#    data = ser.readline()
#    if (data) :
#        print "Received " , len(data) , " chars/bytes: \n" + data
#
#
#
#print "all done\n"
#ser.close()


#class SerialCellularTransitionTransaction:
#    "defines a state change transaction with the expected message, its response message and the next state"
#    def __init__(self):
#        print "Class SerialCellularTransitionTransaction ready\n"
#    "if we match this regex then we transition to the nextState"
#    expectedMessage = None
#    "when transitioning to the nextState we send this response"
#    transitionResponse = None
#    "this is a reference to the SerialCellularState object for the next expected input state"
#    nextState = None
#
#
#class SerialCellularState:
#    "defines the state entries managed by the SerialCellularStateMachine"
#    def __init__(self):
#        print "Class SerialCellularState ready\n"
#    "the name of this state"
#    name = None
#    "this is a list of SerialCellularTransitionTransaction objects for this state"
#    transactionContracts = []
#
#
#class SerialCellularStateMachine:
#    "implement a state machine to communicate with the SM5100B-D cellular modem over a serial port"
#    def __init__(self):
#        print "Class SerialCellularStateMachine ready\n"
#    initialState = None
#    currentState = None
#    allStates = []


#stateA = SerialCellularState()
#stateA.name = "A"
#stateB = SerialCellularState()
#stateB.name = "B"
#
#tranxA1 = SerialCellularTransitionTransaction()
#tranxA1.expectedMessage = "echo"
#tranxA1.transitionResponse = "echo"
#tranxA1.nextState = stateB
#tranxB1 = SerialCellularTransitionTransaction()
#tranxB1.expectedMessage = "echo"
#tranxB1.transitionResponse = "echo"
#tranxB1.nextState = stateA
#
#stateA.transactionContracts.append(tranxA1)
#stateB.transactionContracts.append(tranxB1)
#
#machine = SerialCellularStateMachine()
#machine.allStates.append(stateA)
#machine.allStates.append(stateB)
#machine.initialState = stateA
#machine.currentState = stateA
#
#f = open('cellular_state_machine.yaml')
#dataMap = yaml.load(f)
#f.close()
#f = open('cellular_state_machine.yaml', "w")
#yaml.dump(machine, f)
#f.close()
#
#pprint.pprint(machine)
#
#print jsonpickle.encode(machine)
#print jsonpickle.encode(stateA)
#print jsonpickle.encode(stateB)


#machine2 = {
#    'name' : 'bistable_echo',
#    'allStates' : {
#        'stateA' : {   
#            'name' : 'stateA',
#            'transactionContracts' : [
#                {   
#                    'name' : 'tx1A',
#                    'expectedMessage' : 'echo',
#                    'transitionResponse' : 'echo',
#                    'nextState' : 'stateB',
#                },
#            ]
#        },
#        'stateB' : {   
#            'name' : 'stateB',
#            'transactionContracts' : [
#                {   
#                    'name' : 'tx1B',
#                    'expectedMessage' : 'echo',
#                    'transitionResponse' : 'echo',
#                    'nextState' : 'stateA',
#                },
#            ]
#        },
#    },
#    'initialState' : 'stateA',
#    'currentState' : 'stateA',
#}
#pprint.pprint(machine2)
#
#f = open('cellular_state_machine.yaml', "w")
#yaml.dump(machine2, f, indent=4, default_flow_style=False)
#f.close()

print "loading state machine setup from YAML"
f = open('cellular_state_machine.yaml')
my_mach = yaml.load(f)
f.close()

#f = open('cellular_state_machine.yaml', "w")
#yaml.dump(my_mach, f, indent=4, default_flow_style=False)
#f.close()

pprint.pprint(my_mach)
time.sleep(2)


def serial_cellular_machine_driver(request, machine) :
    print "current machine state:\n%s" % (machine['currentState'])
    print "machine driver received this request of %d chars/bytes:\n%s" % (len(request), request) 
    #if (request) :
    if (1) :
        for tranx in machine['allStates'][machine['currentState']]['transactionContracts']:
            if (re.search(tranx['expectedMessage'], request)):
                print "found matching transaction:\n%s" % (tranx['name'])
                machine['currentState'] = tranx['nextState']
                return tranx['transitionResponse']

        print "request does not match any transactions"
    else:
        print "received empty request"
    return None

print "about to open handle to serial port, your board will get reset."
time.sleep(1)
ser = serial.Serial(port='COM6',baudrate='9600')
settings = ser.getSettingsDict()
print "connection details:\n"+ str(settings)
#Use this to modify settings on the fly, if allowed
#settings['timeout'] = 0;
#ser.applySettingsDict(settings)

# seed initial machine excitation
#ser.write("echo\n")

# clean up the pipe, first line is junk
#data = ser.readline()

while (1):
    print "---------------------------------------------------"
    time.sleep(4)
    data = ser.readline()
    if (data) :
        #print "Received " , len(data) , " chars/bytes: \n" + data
        response = serial_cellular_machine_driver(data.rstrip("\n\r"), my_mach)
        if (response == None) :
            print "received None response, not doing anything"
        else :
            print "will send this response of %d chars/bytes:\n%s" % (len(response), response)
            ser.write(response)
            ser.write("\n\r")
    else:
        print "received empty data"

print "All done"
ser.close()


