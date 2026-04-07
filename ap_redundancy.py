from PyAccessPoint import pyaccesspoint
import socket
import time

# Defining constants
ssid = 'dannywoo'
password = 'dannywoo'
ip = '192.168.45.1'
wlan = 'wlan0'
netmask='255.255.255.0'
inet = None

# Access point object
ap = pyaccesspoint.AccessPoint(wlan='wlan0', inet=inet, ip='192.168.45.1', ssid='dannywoo', password='dannywoo')

# Start AP indefinitely
def ap_setup():
    ap.start()
    print("Connected!")

ap_setup()
