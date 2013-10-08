#!/usr/bin/python -u

#NOTE: using unbuffered IO with -u arg above

import getopt
# import pprint
# import jsonpickle
import serial
import time
import sys
import yaml
import re
from optparse import OptionParser

def serial_cellular_machine_driver(request, machine) :
    print "current machine state:\n%s" % (machine['currentState'])
    print "machine driver received this request of %d chars/bytes:\n%s" % (len(request), request) 
    #if (request) :
    if (1) :
        for tranx in machine['allStates'][machine['currentState']]['transactionContracts']:
            if (re.search(tranx['expectedMessage'], request)):
                print "found matching transaction:\n%s" % (tranx['name'])
                machine['currentState'] = tranx['nextState']
                print "state changed to %s" % (machine['currentState'])
                return tranx
        print "request does not match any expected transaction"
    else:
        print "received empty request, doing nothing"
    return None

def main():
    parser = OptionParser()
    parser.add_option("-f", "--file", help="path to the state machine YAML file", dest="filepath")
    parser.add_option("-p", "--port", help="name of serial port device", dest="port", default="/dev/ttyAMA0")
    parser.add_option("-b", "--baud", help="baud rate", dest="baud", default="115200")
    (options, args) = parser.parse_args()
    
    print "INFO: Welcome to state machine based serial modem driver";
    
    print "INFO: loading state machine setup from YAML: %s" % (options.filepath)
    fd = open(options.filepath)
    my_mach = yaml.load(fd)
    fd.close()
    
    #fd = open('cellular_state_machine.yaml', "w")
    #yaml.dump(my_mach, fd, indent=4, default_flow_style=False)
    #fd.close()
    
    #pprint.pprint(my_mach)
    #time.sleep(1)
    
    print "INFO: about to open handle to serial port, your board may get reset."
    time.sleep(1)
<<<<<<< HEAD
    #ser = serial.Serial(port='COM6',baudrate='9600')
    ser = serial.Serial(port='/dev/ttyUSB0',baudrate='115200')
    #settings = ser.getSettingsDict()
=======
    ser = serial.Serial(port=options.port, baudrate=options.baud)
    settings = ser.getSettingsDict()
>>>>>>> 85a97235099e2f2b960b34b07d22e32bae747e0e
    #print "connection details:\n"+ str(settings)
    #Use this to modify settings on the fly, if allowed
    settings['timeout'] = 1;
    ser.applySettingsDict(settings)
    print "INFO: emptying leftover messages from serial read buffer"
    while (1):
        data = ser.readline()
        if (data):
            print("clear: "+data)
        else:
            break
    
    # seed initial machine excitation
    ser.write("AT\r\n")
    print "initial excitation:\n%s\n" % (my_mach['initialExcitation'])
    ser.write(my_mach['initialExcitation']+"\r\n")

    # clean up the pipe, first line is junk
    print("init0: "+ser.readline())
    print("init1: "+ser.readline())
    settings['timeout'] = None;
    ser.applySettingsDict(settings)
    
    while (1):
        print "---------------------------------------------------"
        data = ser.readline()
        print ("rx0: "+data)
        data = ser.readline()
        print ("rx1: "+data)
        if (data) :
            tranx = {}
            tranx = serial_cellular_machine_driver(data.rstrip("\n\r"), my_mach)
            if (tranx == None) :
                print "received None, not sending any response"
            else :
                response = tranx['transitionResponse']
                if (response != None) :
                    print "will send this response of %d chars/bytes:\n%s" % (len(response), response)
                    if ( 'ctrlZ' in tranx and tranx['ctrlZ'] == 1 ):
                        ser.write(response+"\r\n")
                        response = tranx['responsePart2']
                        print "will send this response of %d chars/bytes with ctrlZ:\n%s" % (len(response+"\x1a"), response)
                        time.sleep(1)
                        data = ser.readline()
                        print "read prompt as:%s" % (data) 
                        data = ser.read(2)
                        print "read prompt as:%s" % (data) 
                        ser.write(response+"\x1a")
                    else:
                        if ( 'nowait' not in tranx or tranx['nowait'] != 1 ):
                            time.sleep(1)
                        ser.write(response+"\r\n")
                    if ( 'readFlush' in tranx and tranx['readFlush'] == 1 ):
                        print "INFO: readFlush"
                        settings['timeout'] = 1;
                        ser.applySettingsDict(settings)
                        while (1):
                            data = ser.readline()
                            if (data):
                                print("flush: "+data)
                            else:
                                break
                        settings['timeout'] = None;
                        ser.applySettingsDict(settings)
        else:
            print "oops, probably received None data"
            break
    
    print "All done"
    ser.close()


main()
