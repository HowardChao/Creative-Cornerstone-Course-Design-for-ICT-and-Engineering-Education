import BT
import maze
import score

# hint: You may design additional functions to execute the input command, which will be helpful when debugging :)

class interface:
    def __init__(self):
        print("")
        print("Arduino Bluetooth Connect Program.")
        print("")
        self.ser = BT.bluetooth()
        port = input("PC bluetooth port name: ")
        # Have bug!!
        while(not self.ser.do_connect(port)):
            if(port == "quit"):
                self.ser.disconnect()
                quit()
            port = input("PC bluetooth port name: ")
        input("Press enter to start.")
        self.ser.SerialWrite('s')

    def wait_for_node(self):
        return self.ser.SerialReadByte()

    def send_action(self,dirc):
        if(dirc == maze.Action.ADVANCE):
            self.ser.SerialWrite('f')
            # print("send_action: ", maze.Action.ADVANCE)
        elif(dirc == maze.Action.U_TURN):
            self.ser.SerialWrite('b')
            # print("send_action: ", maze.Action.U_TURN)
        elif(dirc == maze.Action.TURN_RIGHT):
            self.ser.SerialWrite('r')
            # print("send_action: ", maze.Action.TURN_RIGHT)
        elif(dirc == maze.Action.TURN_LEFT):
            self.ser.SerialWrite('l')
            # print("send_action: ", maze.Action.TURN_LEFT)
        elif(dirc == maze.Action.HALT):
            self.ser.SerialWrite('h')
            # print("send_action: ", maze.Action.HALT)
        else:
            print ('Error: An invalid input for action.')

    def end_process(self):
        self.ser.SerialWrite('e')
        self.ser.disconnect()