#define DEBUG

//#include <Wire.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

enum ControlState {
  START_STATE,
  TRACING_STATE,
  WAITING_STATE
};

// L298N 馬達驅動板：宣告 MotorR 為右邊馬達， MotorL 為左邊馬達。
#define MotorR_IN1     4  //定義 I1 接腳
#define MotorR_IN2     5  //定義 I2 接腳
#define MotorL_IN3     6  //定義 I3 接腳
#define MotorL_IN4     7  //定義 I4 接腳
#define MotorR_PWM_ENA    3  //定義 ENA (PWM調速) 接腳
#define MotorL_PWM_ENB    9  //定義 ENB (PWM調速) 接腳

// 循線模組
// observator : sitting in the back of the car
#define L2  A0  // Define Second Left Sensor Pin
#define L1  A1  // Define First Left Sensor Pin
#define M   A2  // Define Middle Sensor Pin
#define R1  A3  // Define First Right Sensor Pin
#define R2  A4  // Define Second Right Sensor Pin

#define RST_PIN      A5        // 讀卡機的重置腳位
#define SS_PIN       10        // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件
SoftwareSerial BT(2, 8); // bluetooth's TXD, RXD

/*pin definition*/

/**************************************/
/*   Variables Define Here            */
/*   Initialized in setup() Funciton  */
/**************************************/
ControlState _state;               // Control State
int right_motor = 0;
int left_motor = 0;
char _cmd = 'n';

/**************************************/
/*   Function Prototypes Define Here  */
/**************************************/
void Start_State();
void Tracing_State();
void Waiting_State();
void MotorWriting(double vR, double vL);
void get_cmd(char& cmd);

/***************************/
/*   Functions Definition  */
/***************************/
void setup() {
  // Setting PINMode
  Serial.begin(9600);
  BT.begin(9600);  //bluetooth initialization
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(MotorR_IN1, OUTPUT);
  pinMode(MotorR_IN2, OUTPUT);
  pinMode(MotorL_IN3, OUTPUT);
  pinMode(MotorL_IN4, OUTPUT);
  pinMode(MotorR_PWM_ENA, OUTPUT);
  pinMode(MotorL_PWM_ENB, OUTPUT);
  pinMode(R1, INPUT);
  pinMode(R2, INPUT);
  pinMode(M, INPUT);
  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  // Initialize Variables
  _state = START_STATE;      // Control State
  _cmd = 'n';                // Command
#ifdef DEBUG
  Serial.println("Start!");
#endif
}

// Import header files.
#include "RFID.h"
#include "bluetooth.h"
#include "track.h"

void loop() {
  if (_state == START_STATE) Start_State();
  else if (_state == TRACING_STATE) Tracing_State();
  else if (_state == WAITING_STATE) Waiting_State();
}

void Start_State() {
  MotorWriting(0, 0);
#ifdef DEBUG
  Serial.println("Start...");
#endif
  get_cmd(_cmd);
  if (_cmd == 's') {
    // If command 's' is received, the car should change to Tracing State.
    _state = TRACING_STATE;
#ifdef DEBUG
    Serial.println("Changing to Tracing State...");  // Print out the message about the state change.
#endif
  }
}

void Tracing_State() {
#ifdef DEBUG
  Serial.println("Tracing...");
#endif

  if (Tracing()) {
    BT.write('N');
    _state = WAITING_STATE;
#ifdef DEBUG
    Serial.println("Changing to Waiting State...");  // Print out the message about the state change.
#endif
  }

  /*
    MotorWriting(100, 100);
    delay(1000);
    BT.write('N');
    _state = WAITING_STATE;
  */
}

void Waiting_State() {
  //  MotorWriting(0, 0);
#ifdef DEBUG
  Serial.println("Waiting...");
#endif
  get_cmd(_cmd);
  if (_cmd != 'n') {
    // If a command is received, the car should change to Tracing State.
    _state = TRACING_STATE;
    if (_cmd == 'f') {
      // Advance !!
      MotorWriting(200, 200);
      delay(300);
      MotorWriting(0, 0);
      delay(2000);
    } else if (_cmd == 'b') {
      // U-Turn
      MotorWriting(200, 200);
      delay(300);
      MotorWriting(0, 0);
      delay(2000);
      right_motor = 180;
      left_motor = -180;
      MotorWriting(right_motor, left_motor);
      delay(1100);
      MotorWriting(0, 0);
      delay(2000);
    } else if (_cmd == 'r') {
      MotorWriting(200, 200);
      delay(300);
      MotorWriting(0, 0);
      delay(2000);
      right_motor = -80;
      left_motor = 220;
      MotorWriting(right_motor, left_motor);
      delay(700);
      MotorWriting(0, 0);
      delay(2000);
    } else if (_cmd == 'l') {
      // Turn right
      MotorWriting(200, 200);
      delay(300);
      MotorWriting(0, 0);
      delay(2000);
      right_motor = 220;
      left_motor = -50;
      MotorWriting(right_motor, left_motor);
      delay(1100);
      MotorWriting(0, 0);
      delay(2000);
    } else if (_cmd == 'h') {
      // Halt
      MotorWriting(0, 0);
      delay(300);
      right_motor = 0;
      left_motor = 0;
      MotorWriting(right_motor, left_motor);
      delay(5000);
    }
//    byte* read_UID = 0;
//    byte UID_Size = 0;
//    read_UID = rfid(&UID_Size);
//    if (UID_Size > 0) {
//#ifdef DEBUG
//      Serial.print("UID Size: ");
//      Serial.println(UID_Size);
//      Serial.print("UID: ");
//      for (byte i = 0; i < UID_Size; i++) {  // Show UID consequently.
//        Serial.print(read_UID[i], HEX);
//      }
//      Serial.println();
//#endif
//      send_byte(read_UID, UID_Size);
//    }
//    else {
//#ifdef DEBUG
//      Serial.println("No card.");
//#endif
//    }
  }
}
void get_cmd(char &cmd) {
  // Using BT object to get command. Assign value to cmd.
  if (BT.available()) {
    // A command received. Just record the char what BT read.
    cmd = BT.read();
  }
  else {
    // No command received. Set a dummy command.
    cmd = 'n';
  }
}

void MotorWriting(double vR, double vL) {
  // Assign vR to right-motor and vL to left-motor
  if (vR >= 0) {
    // vR is positive.
    digitalWrite(MotorR_IN1, LOW);
    digitalWrite(MotorR_IN2, HIGH);
  }
  else {
    // vR is negative.
    digitalWrite(MotorR_IN1, HIGH);
    digitalWrite(MotorR_IN2, LOW);
    vR = -vR;
  }
  if (vL >= 0) {
    // vL is positive.
    digitalWrite(MotorL_IN3, LOW);
    digitalWrite(MotorL_IN4, HIGH);
  }
  else {
    // vL is negative.
    digitalWrite(MotorL_IN3, HIGH);
    digitalWrite(MotorL_IN4, LOW);
    vL = -vL;
  }
  // Write analog values.
  analogWrite(MotorR_PWM_ENA, vR);
  analogWrite(MotorL_PWM_ENB, vL);
}
