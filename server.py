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

def send_rec_name(name):
    mes = b'\x01\x00' + str.ljust(name[0:15], 16, '\0').encode()
    print(f"Sending RESOURCE-NAME message: {mes}")
    konsole.write(mes)

# Test of resources
while True:
    time.sleep(5)
    print("Sending data")
    #send_rec_clear()
    send_rec_name(vessel.resources.names[0])
    while konsole.in_waiting > 0:
        print(f"»{konsole.read(1)[0]}«")

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

