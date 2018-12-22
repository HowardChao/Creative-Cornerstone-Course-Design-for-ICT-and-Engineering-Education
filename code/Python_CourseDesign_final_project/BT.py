from time import sleep
import serial
# these codes are for bluetooth
# hint: please check the function "sleep". how does it work?

class bluetooth:
    def __init__(self):
        self.ser = serial.Serial()

    def do_connect(self, port):
        self.ser.close()
        print("Connecting...")
        try:
            self.ser = serial.Serial(port, 9600, timeout=2)
            print("connect success")
            print("")
        except serial.serialutil.SerialException:
            print("fail to connect")
            print("")
            return False
        return True

    def disconnect(self):
        self.ser.close()

    def SerialWrite(self, output):
        # send = 's'.encode("utf-8")
        send = output.encode("utf-8")
        print(send)
        self.ser.write(send)

    def SerialReadString(self):
        sleep(0.01)
        waiting = self.ser.in_waiting
        if (waiting != 0):
            rv = [chr(c) for c in self.ser.read(waiting)]
            self.ser.reset_input_buffer()
            return "".join(rv)
        return ""

    def SerialReadByte(self):
        #TODO: Get the UID from bytes. Notice that the return type should be transformed into hex. 
        return 0


