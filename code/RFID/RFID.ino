#define DEBUG
#include<SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN      A5        // 讀卡機的重置腳位
#define SS_PIN       10        // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
SoftwareSerial BT(2, 8);  //RX,TX


void setup()
{
  Serial.begin(9600);
  Serial.println("hello world");
  BT.begin(9600);

  SPI.begin();
  mfrc522.PCD_Init();

  while (1)
  {
    if (BT.available())
    {
      char start = BT.read();
      if (start == 's') break;
    }
  }
}

#include "bluetooth.h"
#include "RFID.h"
void loop()
{
  // This part is for reading messages via bluetooth.
  /*
    if (BT.available()) {
    char result = BT.read();
    Serial.println(result);
    }
  */

  // This part is for sending messages via bluetooth.
  byte* read_UID;
  byte UID_Size;
  read_UID = rfid(&UID_Size);
  if (UID_Size > 0) {
    send_byte(read_UID, UID_Size);
#ifdef DEBUG
    Serial.print("Successfully send out a UID: ");
    Serial.println(*read_UID);
#endif
  }
  else {
#ifdef DEBUG
    Serial.println("Nothing is read.");
#endif
  }
  delay(50);
}