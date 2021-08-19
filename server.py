#!/usr/bin/env python
import time
from struct import pack, unpack
import krpc
import serial
import PySimpleGUI as sg

print("Connecting to Konsole")
konsole = serial.Serial(port = "/dev/ttyACM0", baudrate = 115200, timeout = .1)
print("Connection established")

print("Connecting to kRPC")
conn = krpc.connect()
print("Connection established")

vessel = conn.space_center.active_vessel

def send_rec_clear():
    mes = b'\x03'
    print(f"Sending RESOURCE-CLEAR message: {mes}")
    konsole.write(mes)

def send_rec_name(id, name):
    mes = b'\x01' + chr(id).encode() + str.ljust(name[0:15], 16, '\0').encode()
    print(f"Sending RESOURCE-NAME message: {mes}")
    konsole.write(mes)

def send_rec_val(id, value):
    mes = b'\x02' + chr(id).encode() + pack('<f', value);
    print(f"Sending RESOURCE-VALUE message: {mes}")
    konsole.write(mes)

# Test of resources
ri = 0
while True:
    time.sleep(5)
    print("Sending data")
    #send_rec_clear()
    res = vessel.resources.names[ri]
    send_rec_name(ri, res)
    send_rec_val(ri, vessel.resources.amount(res) / vessel.resources.max(res))
    ri = (ri + 1) % min(3, len(vessel.resources.names))
    while konsole.in_waiting > 0:
        c = konsole.read(1)[0]
        if c < 64:
            print(f"»{hex(c)}«")
        else:
            print(f"»{chr(c)}«") 

#rcs_status = conn.add_stream(getattr, vessel.control, 'rcs')
#while True:
#    if konsole.in_waiting > 1:
#    	konsole.read(konsole.in_waiting - 1)
#    if konsole.in_waiting == 1:
#    	read = konsole.read(1)[0]
#    	print(f"Read: {read}")
#    	vessel.control.rcs = (read & 1) == 1
#
#    result = 0
#    if rcs_status():
#        result |= 1
#    konsole.write(pack('>B', result))
#    time.sleep(0.1)

