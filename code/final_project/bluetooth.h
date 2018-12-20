#include<SoftwareSerial.h>

// TODO: return the direction based on the command you read
int ask_direction(){  
	
}

// TODO: send the id back by BT
void send_byte(byte *id, byte idSize){
	if(true) {
      if (BT.available()) {
       BT.write(id);
      }
     // what I write END
     // For debugging you can ignore this
     #ifdef DEBUG
     Serial.print("Sent id: ");
     Serial.println(id);
     #endif
   }
}

