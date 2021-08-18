#!/usr/bin/env python
import time
import struct
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
    konsole.write(struct.pack('>B', result))
    time.sleep(0.1)

