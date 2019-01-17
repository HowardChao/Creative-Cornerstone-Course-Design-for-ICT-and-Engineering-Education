#include<SoftwareSerial.h>

// TODO: return the direction based on the command you read
int ask_direction() {

}

// TODO: send the id back by BT
void send_byte(byte *id, byte idSize) {
  for (byte i = 0; i < idSize; i++) {  // Send UID consequently.
    BT.write(id[i]);
  }
#ifdef DEBUG
  Serial.print("Sent id: ");
  for (byte i = 0; i < idSize; i++) {  // Show UID consequently.
    Serial.print(id[i], HEX);
  }
  Serial.println();
#endif
}
