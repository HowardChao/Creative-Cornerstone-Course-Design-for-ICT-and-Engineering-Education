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

int r2_p = LOW;
int r1_p = LOW;
int m_p = LOW;
int l1_p = LOW;
int l2_p = LOW;

int pre_motor_speed_right = 0;
int pre_motor_speed_left = 0;

/*pin definition*/

/**************************************/
/*   Variables Define Here            */
/*   Initialized in setup() Funciton  */
/**************************************/
ControlState _state;               // Control State
int right_motor = 0;
int left_motor = 0;
char _cmd = 'n';
int COUNTER_CONST = 15;
int counter = 15;
int BLACK_NODE_COUNTER = 2;
int black_node_counter = 0;

int FORWARD_SWING_COUNTER = 13;
int forward_swing_counter = 0;

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
  counter++;
  if (Tracing()) {
    if (counter >= COUNTER_CONST && black_node_counter == 2) {
      black_node_counter = 0;
      BT.write('N');
      _state = WAITING_STATE;
#ifdef DEBUG
      Serial.println("Changing to Waiting State...");  // Print out the message about the state change.
#endif
      counter = 0;
    }
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
  MotorWriting(100, 100); // Change here to read RFID!!!!!
//  delay(700);


byte* read_UID = 0;
byte UID_Size = 0;

  for (int i=0; i<35; i++) {
    byte* read_UID_tmp = 0;
    byte UID_Size_tmp = 0;
    read_UID_tmp = rfid(&UID_Size_tmp);
    if (*read_UID_tmp != 0 && UID_Size_tmp != 0) {
      read_UID = read_UID_tmp;
      UID_Size = UID_Size_tmp;
    }
#ifdef DEBUG
    Serial.print("UID Size: ");
    Serial.println(UID_Size);
    Serial.print("UID: ");
    for (byte i = 0; i < UID_Size; i++) {  // Show UID consequently.
      Serial.print(read_UID[i], HEX);
    }
#endif
  }
   get_cmd(_cmd);
   send_byte(read_UID, UID_Size);
  //  else {
  //#ifdef DEBUG
  //    Serial.println("No card.");
  //#endif
  //    BT.write('N');
  //  }
  if (_cmd != 'n') {
    // If a command is received, the car should change to Tracing State.
    _state = TRACING_STATE;
    if (_cmd == 'f') {
      // Advance !!
      MotorWriting(200, 200);
      delay(300);
      int r2_forward = digitalRead(R2);  // right-outer sensor
      int r1_forward = digitalRead(R1);  // right-inner sensor
      int m_forward = digitalRead(M);    // middle sensor
      int l1_forward = digitalRead(L1);  // left-inner sensor
      int l2_forward = digitalRead(L2);  // left-outer sensor
      while (r2_forward == LOW && r1_forward == LOW && m_forward == LOW && l1_forward == LOW && l2_forward == LOW) {        
        Serial.println("Inside while loop");
        forward_swing_counter++;
        if (forward_swing_counter < FORWARD_SWING_COUNTER) {
          Serial.println("Inside forward_swing_counter forrrr");
          right_motor = 0;
          left_motor = 100;
          MotorWriting(right_motor, left_motor);
        } else {
          Serial.println("Inside forward_swing_counter Breakkkkkkk");
          break ;
        }
        Serial.println("Read again !!!!");
        r2_forward = digitalRead(R2);  // right-outer sensor
        r1_forward = digitalRead(R1);  // right-inner sensor
        m_forward = digitalRead(M);    // middle sensor
        l1_forward = digitalRead(L1);  // left-inner sensor
        l2_forward = digitalRead(L2);  // left-outer sensor
      }
      MotorWriting(0, 0);
      Serial.println("Outside loop!!!!!");
      delay(500);
      forward_swing_counter = 0;
      r2_p = LOW;
      r1_p = LOW;
      m_p = LOW;
      l1_p = LOW;
      l2_p = HIGH;
    } else if (_cmd == 'b') {
      r2_p = LOW;
      r1_p = LOW;
      m_p = LOW;
      l1_p = LOW;
      l2_p = HIGH;
      // U-Turn
      //      MotorWriting(200, 200);
      //      delay(700);
      MotorWriting(0, 0);
      delay(700);
      right_motor = 180;
      left_motor = -180;
      MotorWriting(right_motor, left_motor);
      delay(900);
    } else if (_cmd == 'r') {
      r2_p = HIGH;
      r1_p = LOW;
      m_p = LOW;
      l1_p = LOW;
      l2_p = LOW;
      //      MotorWriting(200, 200);
      //      delay(700);
      MotorWriting(0, 0);
      delay(700);
      right_motor = -80;
      left_motor = 220;
      MotorWriting(right_motor, left_motor);
      delay(300);
    } else if (_cmd == 'l') {
      // Turn right
      r2_p = LOW;
      r1_p = LOW;
      m_p = LOW;
      l1_p = LOW;
      l2_p = HIGH;
      //      MotorWriting(200, 200);
      //      delay(700);
      MotorWriting(0, 0);
      delay(700);
      right_motor = 220;
      left_motor = -50;
      MotorWriting(right_motor, left_motor);
      delay(500);
    } else if (_cmd == 'h') {
      // Halt
      MotorWriting(0, 0);
      delay(500);
      right_motor = 0;
      left_motor = 0;
      MotorWriting(right_motor, left_motor);
      delay(5000);
    }
    MotorWriting(0, 0);
    delay(700);
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
