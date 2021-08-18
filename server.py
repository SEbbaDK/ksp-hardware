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

# Test of resources
while True:
    time.sleep(1)
    print("Sending data")
    konsole.write(pack('<B', 3))
    #konsole.write(pack('<B', 1))
    #konsole.write(pack('<B', 0))
    name = vessel.resources.names[0][0:8].encode()
    print(f"Name: {name}")
    #konsole.write(name)

rcs_status = conn.add_stream(getattr, vessel.control, 'rcs')
while True:
    if konsole.in_waiting > 1:
    	konsole.read(konsole.in_waiting - 1)
    if konsole.in_waiting == 1:
    	read = konsole.read(1)[0]
    	print(f"Read: {read}")
    	vessel.control.rcs = (read & 1) == 1

    result = 0
    if rcs_status():
        result |= 1
    konsole.write(pack('>B', result))
    time.sleep(0.1)

