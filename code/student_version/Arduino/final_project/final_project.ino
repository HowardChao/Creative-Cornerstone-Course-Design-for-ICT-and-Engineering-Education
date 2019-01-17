#define DEBUG

//#include <Wire.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

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
  // TODO : 
}