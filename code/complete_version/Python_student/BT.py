from time import sleep
import serial
# these codes are for bluetooth
# hint: please check the function "sleep". how does it work?

# TODO: Define your own bluetooth class!
# Suggest methods: 
#  1. do_connect(): connect your computer to bluetooth device. (need to handle connection failure)
#  2. disconnect(): disconnect your computer to bluetooth device.
#  3. SerialWrite(): Send information from your computer to arduino. 
#  4. SerialReadNodeInfo(): Read information from arduino after car detects a node.
#  5. SerialReadRFID(): Read information from arduino after car gets an UID string.
#  6. Other helpful methods!

class bluetooth:
    def __init__(self):
        self.ser = serial.Serial()
