#include<SoftwareSerial.h>

// TODO: return the direction based on the command you read
int ask_direction(){  
//   if (true) {
//     //TODO
//     // just record the char what BT read
//     if (BT.available()) {
//       _final_project_cmd = BT.read();
//     }else{
//      _final_project_cmd = 'n';
//     }
//     // what I write END
//     // For debugging you can ignore this
////     #ifdef DEBUG
//     Serial.print("After ask direction: ");
//     Serial.println(_final_project_cmd);
////     #endif
//   }
}

void reach_node_send_to_python () {
  if (true) {
    if (BT.available()) {
      BT.write('N');
    }
//    Serial.print("Sent id: ");
//    Serial.println('N');
  }
}

// TODO: send the id back by BT
void send_byte(byte *id, byte idSize){
	if(true) {
      if (BT.available()) {
       BT.write(*id);
      }
     // what I write END
     // For debugging you can ignore this
     #ifdef DEBUG
//     Serial.print("Sent id: ");
//     Serial.println(id);
     #endif
   }
}
