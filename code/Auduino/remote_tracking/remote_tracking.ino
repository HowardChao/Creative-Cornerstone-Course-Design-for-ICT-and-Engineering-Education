//#define DEBUG
#include<Wire.h>
#include<SoftwareSerial.h>
#include <SPI.h>

enum ControlState {
  START_STATE,
  REMOTE_STATE,
  TRACING_STATE
};

// L298N 馬達驅動板
// 宣告 MotorR 為右邊
// 宣告 MotorL 為左邊
#define MotorRR_I3     4  //定義 I1 接腳
#define MotorRR_I4     5 //定義 I2 接腳
#define MotorLL_I1     6 //定義 I3 接腳
#define MotorLL_I2     7 //定義 I4 接腳
#define MotorR_PWMR    3  //定義 ENA (PWM調速) 接腳
#define MotorL_PWML    9  //定義 ENB (PWM調速) 接腳

// 循線模組
// observator : sitting in the back of the car
#define L2  A0  // Define Second Left Sensor Pin
#define L1  A1  // Define First Left Sensor Pin
#define M   A2  // Define Middle Sensor Pin
#define R1  A3  // Define First Right Sensor Pin
#define R2  A4  // Define Second Right Sensor Pin

SoftwareSerial BT(11, 10);  //bluetooth RX,TX

/*pin definition*/

/**************************************/
/*   Variables Define Here            */
/*   Initialized in setup() Funciton  */
/**************************************/
ControlState _state;               // Control State
char   _cmd;                        // Command
int right_motor = 0;
int left_motor = 0;

/**************************************/
/*   Function Prototypes Define Here  */
/*   Finish TODO in Blacking Function */
/**************************************/
void Tracing_Mode();
void Remote_Mode();
void Start_Mode();
void MotorWriting(double& vR, double& vL);
void get_cmd(char& cmd);

/***************************/
/*   Functions Definition  */
/***************************/

void setup() {
  // Setting PINMode
  Serial.begin(9600);
  BT.begin(9600); //bluetooth initialization
  pinMode(MotorRR_I3, OUTPUT);
  pinMode(MotorRR_I4, OUTPUT);
  pinMode(MotorLL_I1, OUTPUT);
  pinMode(MotorLL_I2, OUTPUT);
  pinMode(MotorR_PWMR, OUTPUT);
  pinMode(MotorL_PWML, OUTPUT);
  pinMode(R1, INPUT);
  pinMode(R2, INPUT);
  pinMode(M,  INPUT);
  pinMode(L1, INPUT);
  pinMode(L2, INPUT);
  // Initialize Variables
  // You can Define you own value here.
  _state = START_STATE;      // Control State
  _cmd = 'n';                // Command
#ifdef DEBUG
  Serial.println("Start!");
#endif
}


void loop() {
  /*TODO*/
  if (_cmd != 'n') {
    Serial.println(_cmd);
  }
  if (_state == START_STATE) Start_Mode();
  else if (_state == REMOTE_STATE) Remote_Mode();
  else if (_state == TRACING_STATE) Tracing_Mode();
  SetState();
}


void Start_Mode() {
  /********************/
  /* Activate the car */
  /********************/
  MotorWriting(0, 0);
}

void SetState() {
  /**************************************************/
  /* Get command first and change _state for given  */
  /* _cmd and current _state                        */
  /* Remember to assign _cmd = 'n' as a dummy inst. */
  /**************************************************/
  get_cmd(_cmd);
  if (_state == START_STATE) {
    if (_cmd == 's') {
      _state = REMOTE_STATE;
#ifdef DEBUG
      Serial.println("Changing to Remote Mode...");
#endif
    } else {
      _state = _state;
    }
  } else if (_state == REMOTE_STATE) {
    if (_cmd == 'a') {
      _state = TRACING_STATE;
#ifdef DEBUG
      Serial.println("Changing to Tracking Mode...");
#endif
    } else {
      _state = _state;
    }
  } else if (_state == TRACING_STATE) {
    if (_cmd == 'a') {
      _state = REMOTE_STATE;
#ifdef DEBUG
      Serial.println("Backing to Remote Mode...");
#endif
      MotorWriting(0, 0);
    } else {
      _state = _state;
    }
  }
}


void Tracing_Mode() {
  // Simple Tracing
  /*************************************************/
  /* Read the sensor value and determine wether to */
  /* turn left or turn right or go straight        */
  /*************************************************/

  // Initialize Senor value
  int r2 = digitalRead(R2);
  int r1 = digitalRead(R1);  // right-inner sensor
  int m = digitalRead(M); // middle sensor
  int l1 = digitalRead(L1); // left-inner sensor
  int l2 = digitalRead(L2);
#ifdef DEBUG
  Serial.println(r2);
  Serial.println(r1);
  Serial.println(m);
  Serial.println(l1);
  Serial.println(l2);
  Serial.println();
#endif

  // Using "MotorWriting()" to turn or go straight
  // depending on the sensors value
  // HIGH = when black road is sensed

  if ((r2 == HIGH) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
    // big right turn
    MotorWriting(-180, 250);
  } else if ((r2 == HIGH) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
    // small right turn
    MotorWriting(-150, 250);
  } else if ((r2 == HIGH) && (r1 == HIGH) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
    // small right turn
    MotorWriting(-100, 200);
  } else if ((r2 == LOW) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
    // small right turn
    MotorWriting(50, 200);
  } else if ((r2 == LOW) && (r1 == HIGH) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
    // small right turn
    MotorWriting(100, 150);
  } else if ((r2 == LOW) && (r1 == HIGH) && (m == HIGH) && (l1 == HIGH) && (l2 == LOW)) {
    // center
    MotorWriting(100, 100);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
    // center
    MotorWriting(100, 100);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == HIGH) && (l2 == LOW)) {
    // small left turn
    MotorWriting(150, 100);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == LOW)) {
    // small left turn
    MotorWriting(200, 50);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == HIGH) && (l2 == HIGH)) {
    // small left turn
    MotorWriting(200, -100);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == HIGH)) {
    // small left turn
    MotorWriting(250, -150);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == HIGH)) {
    // big left turn
    MotorWriting(250, -180);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
    MotorWriting(100, 100);
  } else {
    MotorWriting(-60, -60);
  }
}

void MotorWriting(double vR, double vL) {

  /*************************************************/
  /* Assign vR to right-motor and vL to left-motor */
  /* Reverse the motor direction if necessary      */
  /*************************************************/
  if (vR >= 0) {
    digitalWrite(MotorRR_I3, LOW);
    digitalWrite(MotorRR_I4, HIGH);
  } else if (vR < 0) {
    digitalWrite(MotorRR_I3, HIGH);
    digitalWrite(MotorRR_I4, LOW);
    vR = -vR;
  }
  if (vL >= 0) {
    digitalWrite(MotorLL_I1, LOW);
    digitalWrite(MotorLL_I2, HIGH);
  } else if (vL < 0) {
    digitalWrite(MotorLL_I1, HIGH);
    digitalWrite(MotorLL_I2, LOW);
    vL = -vL;
  }
  analogWrite(MotorL_PWML, vL);
  analogWrite(MotorR_PWMR, vR);
}

void Remote_Mode() {
  /***********************************************/
  /* Accroding to the instruction recieved(_cmd) */
  /* Assign different movements for the car(Turn */
  /* Right, Turn Left, Backward, Forward, Stop)  */
  /* You need to consider '5' conditions         */
  /***********************************************/
  // check the value of _cmd (it should contains the command corresponding to the app on the cellphone)
  if (_cmd == 'r') {
    // turn right
    Serial.println("Turn right!");
    MotorWriting(0, 200);
  } else if (_cmd == 'l') {
    // turn left
    Serial.println("Turn left!");
    MotorWriting(200, 0);
  } else if (_cmd == 'd') {
    // backward
    Serial.println("Turn back!");
    MotorWriting(-200, -200);
  } else if (_cmd == 'u') {
    // forward
    Serial.println("Turn forward!");
    MotorWriting(200, 200);
  } else if (_cmd == 's') {
    // stop
    Serial.println("Stop!");
    MotorWriting(0, 0);
  }
}

void get_cmd(char &cmd) {
  delay(30); // Don't delete its!!
  /*************************************/
  /* Using BT object to get command */
  /* Assign value to cmd               */
  /*************************************/
  if (true) {
    // just record the char what BT read
    if (BT.available()) {
      cmd = BT.read();
#ifdef DEBUG
      Serial.print("Received: ");
      Serial.println(cmd);
#endif
    } else {
      cmd = 'n';
    }
  }
}
