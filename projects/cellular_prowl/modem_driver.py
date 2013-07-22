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
                #return tranx['transitionResponse']
                return tranx

        print "request does not match any transactions"
    else:
        print "received empty request"
    return None

def main():
    parser = OptionParser()
    parser.add_option("-f", "--file", help="path to the state machine YAML file", dest="filepath")
    (options, args) = parser.parse_args()
    
    print "[welcome to modem_driver]";
    
    print "loading state machine setup from YAML: %s" % (options.filepath)
    f = open(options.filepath)
    my_mach = yaml.load(f)
    f.close()
    
    #f = open('cellular_state_machine.yaml', "w")
    #yaml.dump(my_mach, f, indent=4, default_flow_style=False)
    #f.close()
    
    #pprint.pprint(my_mach)
    #time.sleep(1)
    
    print "about to open handle to serial port, your board will get reset."
    time.sleep(1)
    ser = serial.Serial(port='COM6',baudrate='9600')
    #settings = ser.getSettingsDict()
    #print "connection details:\n"+ str(settings)
    #Use this to modify settings on the fly, if allowed
    #settings['timeout'] = 0;
    #ser.applySettingsDict(settings)
    
    # seed initial machine excitation
    #ser.write("echo\n")
    
    # clean up the pipe, first line is junk
    #data = ser.readline()
    
    while (1):
        print "---------------------------------------------------"
        data = ser.readline()
        data = ser.readline()
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
                #ser.write(response)
                #ser.write("\n\r")
        else:
            print "oops, probably received None data"
            break
    
    print "All done"
    ser.close()


main()
