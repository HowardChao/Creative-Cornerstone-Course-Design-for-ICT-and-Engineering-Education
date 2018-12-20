//#define DEBUG
#include<Wire.h>
#include<SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

// App Controller State
enum ControlState {
   START_STATE,
   REMOTE_STATE,
   TRACKING_STATE,
   SETTING_STATE,
   FINAL_STATE
};

#define RST_PIN      4        // RFID reset pin
#define SS_PIN       13        // RFID selection pin
#define RFID_SDA      9
#define RFID_SCK     13
#define RFID_MOSI    11
#define RFID_MISO    12

MFRC522 mfrc522(SS_PIN, RST_PIN);  // MFRC522 object declaration

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

SoftwareSerial BT(11,10);   //bluetooth RX,TX

/*pin definition*/

/**************************************/
/*   Variables Define Here            */
/*   Initialized in setup() Funciton  */
/**************************************/
ControlState _state;               // Control State
double _Tp;                         // Velocity of Car
double _w2;                         // Weight Value for the Outer Sensor
double _w1;                         // Weight Value for the Inner Sensor
double _Kp;                         // _Kp Parameter
double _Kd;                         // _Kd Parameter
double _Ki;                         // _Ki Parameter
double _LastError;                  // Error in Last Stage
char   _cmd;                        // Command
//char   _final_project_cmd;          // Final project Command
double _integral;                   // Integral from Starting point
bool R_dir = true;                  // if dir == ture, mean right-motor is forwarding. On the other hand, backwarding.
bool L_dir = true;                  // if dir == ture, mean Left-motor is forwarding. On the other hand, backwarding.
int right_motor = 0;
int left_motor = 0;
int node_time = 0;

/**************************************/
/*   Function Prototypes Define Here  */
/*   Finish TODO in Blacking Function */
/**************************************/
void Tracing_Mode();
void Remote_Mode();
void Start_Mode();
void Setting_Mode();
void Final_Mode();
void MotorWriting(double& vR, double& vL);
//void MotorInverter(int motor, bool& _cmd);
void get_cmd(char& cmd);
void Send_OK();
void SetState();

/***************************/
/*   Functions Definition  */
/***************************/

void setup()
{
   // Setting PINMode
    Serial.begin(9600);
    BT.begin(9600); //bluetooth initialization
    pinMode(MotorRR_I3,   OUTPUT);
    pinMode(MotorRR_I4,   OUTPUT);
    pinMode(MotorLL_I1,   OUTPUT);
    pinMode(MotorLL_I2,   OUTPUT);
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
//   _final_project_cmd = 'n';
   _Tp    = 130;              // Velocity of Car
   _w2    = 13.5;             // Second Stage Weight
   _w1    = _w2/2.0;          // First Stage Weight
   _Kp    = 15;               // _Kp Parameter
   _Kd    = 0.03;             // _Kd Parameter
   _Ki    = 0.0;              // _Ki Parameter
   _LastError = 0;            // Error in Last Stage
   _integral  = 0;
   Serial.println("Start!");

    SPI.begin();         //RFID initialization
    mfrc522.PCD_Init();
    /*define your pin mode*/
}

#include "track.h"
#include "node.h"
#include "bluetooth.h"
#include "RFID.h"

void loop()
{
    /*TODO*/
   if (_cmd != 'n') {
    Serial.println(_cmd);    
   }
   if(_state == START_STATE) Start_Mode();
   else if(_state == REMOTE_STATE) Remote_Mode();
   else if(_state == TRACKING_STATE) Tracing_Mode();
   else if(_state == SETTING_STATE) Setting_Mode();
   else if(_state == FINAL_STATE) Final_Mode();
   SetState();
}


void Start_Mode() {
   /****************/
   /* Stop the car */
   /****************/
   MotorWriting(0,0);
}

void SetState() {
   /**************************************************/
   /* Get command first and change _state for given  */
   /* _cmd and current _state                        */
   /* Remember to assign _cmd = 'n' as a dummy inst. */
   /**************************************************/
   get_cmd(_cmd);
   if(_state == START_STATE) {
      //TODO
      if(_cmd == 's') { 
         // change _state and reinitialize _cmd
         _state = REMOTE_STATE;
         // For debugging you can ignore this
//         #ifdef DEBUG
         Serial.println("Changing to Remote Mode..."); 
//         #endif
      } else { _state = _state; }
   } else if(_state == REMOTE_STATE) {
      //TODO
      if(_cmd == 'a') {
         // change _state and reinitialize _cmd
         _state = TRACKING_STATE;
         // For debugging you can ignore this
         #ifdef DEBUG
         Serial.println("Changing to Tracking Mode..."); 
         #endif
      } else if(_cmd == 't') {
         //TODO
         // change _state and reinitialize _cmd
         _state = SETTING_STATE;
         Serial.println("Changing to Setting Mode...");
         // For debugging you can ignore this
         #ifdef DEBUG
         Serial.println("Changing to Setting Mode...");
         #endif
      } else if (_cmd == 'z') {
        _state = FINAL_STATE;
        /// Do something to check
        Serial.println("Changing to FINAL_State Mode...");
        #ifdef DEBUG
        Serial.println("Changing to Setting Mode...");
        #endif
      } else { _state = _state; }
   } else if(_state == TRACKING_STATE) {
      //TODO
      if(_cmd == 'a') { 
         // change _state and reinitialize _cmd
         _state = REMOTE_STATE;
         // For debugging you can ignore this
         #ifdef DEBUG
         Serial.println("Backing to Remote Mode..."); 
         #endif
         MotorWriting(0,0);
       } else if(_cmd == 't') {
         //TODO
         // change _state and reinitialize _cmd
         _state = SETTING_STATE;
         // For debugging you can ignore this
         #ifdef DEBUG
         Serial.println("Changing to Setting Mode...");
         #endif
         MotorWriting(0,0);
      } else { _state = _state; }
   } else if(_state == SETTING_STATE) {
      //TODO
      if(_cmd == 't') { 
         // change _state and reinitialize _cmd
         _state = REMOTE_STATE;
         // For debugging you can ignore this
         #ifdef DEBUG
         Serial.println("Backing to Remote Mode..."); 
         #endif
      } else if (_cmd == 'a') {
         // change _state and reinitialize _cmd
         _state = TRACKING_STATE;
         // For debugging you can ignore this
         #ifdef DEBUG
         Serial.println("Changing to Tracking Mode..."); 
         #endif
      } else { _state = _state; }
   }  else if (_state == FINAL_STATE) {
     if(_cmd == 'z') { 
         // change _state and reinitialize _cmd
         _state = REMOTE_STATE;
         // For debugging you can ignore this
         MotorWriting(0,0);
//         #ifdef DEBUG
         Serial.println("Backing to Remote Mode..."); 
//         #endif
      } 
      else { _state = _state; }
   }
}


void Tracing_Mode() {
   // Simple Tracking
   /*************************************************/
   /* Read the sensor value and determine wether to */
   /* turn left or turn right or go straight        */
   /*************************************************/

   //TODO
   // Initialize Senor value
   int r2 = digitalRead(R2);
   int r1 = digitalRead(R1);  // right-inner sensor
   int m = digitalRead(M); // middle sensor
   int l1 = digitalRead(L1); // left-inner sensor
   int l2 = digitalRead(L2);
   Serial.println(r2);
   Serial.println(r1);
   Serial.println(m);
   Serial.println(l1);
   Serial.println(l2);
   Serial.println();
  
   //TODO
   // Using "MotorWriting()" to turn or go straight
   // depending on the sensors value
   
   
   // HIGH = when black road is sensed
   
  if ((r2 == HIGH) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {  
    // big right turn
    MotorWriting(-250, 250);
  } else if ((r2 == HIGH) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
    // small right turn 
    MotorWriting(-150, 250);
  } else if ((r2 == HIGH) && (r1 == HIGH) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
    // small right turn 
    MotorWriting(0, 200);
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
    MotorWriting(200, 0);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == HIGH)) {
    // small left turn
    MotorWriting(250, -150);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == HIGH)) {
    // big left turn
    MotorWriting(250, -250);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
    MotorWriting(100, 100);
  } else {
    MotorWriting(-40, -40);
  }
    // don't know what to do; stop
//       delay(50);

}

//   //p-control 
////    int _Vr = 50;
////    int _Vl = 50;
////    double _w2 = 10;
////    double _w1 = 5;
////    double _Kp;
//
////   _Vr = _Vr + r2*_w2 + r1*_w1 + m - l1*_w1 - l2*_w2;
////   _Vl = _Vl + l2*_w2 + l1*_w1 + m - r1*_w1 - r2*_w2; 
////   if (_Vr >200){
////     _Vr = 200;
////   }
////   if(_Vl > 200 ){
////     _Vl = 200;
////   }
////   MotorWriting(_Vr,_Vl);
//}

void MotorWriting(double vR, double vL) {

   /*************************************************/
   /* Assign vR to right-motor and vL to left-motor */
   /* Reverse the motor direction if necessary      */
   /*************************************************/
   //TODO
   if (vR >= 0) {
     R_dir = true;
     digitalWrite(MotorRR_I3, LOW); 
     digitalWrite(MotorRR_I4, HIGH);
   } else if (vR < 0) {
     R_dir = false; 
     digitalWrite(MotorRR_I3, HIGH); 
     digitalWrite(MotorRR_I4, LOW);
     vR = -vR;
   }
   if (vL >= 0) {
     L_dir = true;
     digitalWrite(MotorLL_I1, LOW); 
     digitalWrite(MotorLL_I2, HIGH);
   } else if (vL < 0) {
     L_dir = false;
     digitalWrite(MotorLL_I1, HIGH); 
     digitalWrite(MotorLL_I2, LOW);
     vL = -vL;
   }
   analogWrite(MotorL_PWML,vL); 
   analogWrite(MotorR_PWMR,vR);
//   
//   if (R_dir == true) {
//     if (vR < 0) {
//       MotorInverter(1, R_dir); 
//       vR = abs(vR);
//     }
//   } else if (R_dir == false) {
//     if (vR > 0) {
//       MotorInverter(1, R_dir);
//     } 
//     vR = abs(vR);
//   }
//   
//   if (L_dir == true) {
//     if (vL < 0) {
//       MotorInverter(2, L_dir);
//       vL = abs(vL);
//     }
//   } else if (L_dir == false) {
//     if (vL > 0) {
//       MotorInverter(2, L_dir);
//     }
//     vL = abs(vL);
//   }

}   

//void MotorInverter(int motor, bool& dir) {
//   /*************************************/
//   /* Reverse the dir for the given dir */
//   /* value and motor(Left or Right)    */
//   /*************************************/
//   //TODO
//   // wheel direction
//   dir = !dir;
//   if (motor == 1) {
//     if (dir == true) {
//       digitalWrite(MotorRR_I3, HIGH); 
//       digitalWrite(MotorRR_I4, LOW);
//     } else {
//       digitalWrite(MotorRR_I3, LOW); 
//       digitalWrite(MotorRR_I4, HIGH);
//     }
//   } else if (motor == 2) {
//     if (dir == true) {
//       digitalWrite(MotorLL_I1, HIGH); 
//       digitalWrite(MotorLL_I2, LOW);
//     } else {
//       digitalWrite(MotorLL_I1, LOW); 
//       digitalWrite(MotorLL_I2, HIGH);
//     }
//   }
//}

void Remote_Mode() {
   //TODO
   /***********************************************/
   /* Accroding to the instruction recieved(_cmd) */
   /* Assign different movements for the car(Turn */
   /* Right, Turn Left, Backward, Forward, Stop)  */
   /* You need to consider '5' conditions         */
   /***********************************************/
   // what I write START
   // check the value of _cmd (it should contains the command corresponding to the app on the cellphone)
   if(_cmd == 'r') {
     // turn right
     Serial.println("Turn right!");
     MotorWriting(0, 200);
   }
   else if(_cmd == 'l') {
     // turn left
     Serial.println("Turn left!");
     MotorWriting(200, 0);
   }
   else if(_cmd == 'd') {
     // backward
     Serial.println("Turn back!");
     MotorWriting(-200, -200);
   }
   else if(_cmd == 'u') {
     // forward
     Serial.println("Turn forward!");
     MotorWriting(200, 200);
   }
   else if(_cmd == 's') {
     // stop
     Serial.println("Stop!");
     MotorWriting(0, 0);
   }
   // what I write END  
}


void Setting_Mode() {
   /**********************************/
   /* Use for setting PID parameters */
   /* Don't remove anything in it    */
   /**********************************/
//   Serial.flush();
//   Serial.println('setting');     
//   char text[2];
//   int s = 0;
//   bool sended = false;
//   while(1){
//     delay(50);
//     s = BT.available();
//     if(s) { sended = true; }
//     bool in = false;
//     if(s>0){
//       BT.readBytes(text,2);
//       Serial.print("Input size(be readBy): ");
//       Serial.println(s);
//       if(text[0] == 'p') {
//         _Kp = int(text[1]);
//         Serial.println(text[0]);
//         Serial.println(_Kp);
//         in = true;
//       }
//       else if(text[0] == 'd') { 
//         _Kd = double(int(text[1]))/100.0;
//         Serial.println(text[0]);
//         Serial.println(_Kd);
//         in = true;
//       }
//       else if(text[0] == 'i') { 
//         _Ki = double(int(text[1]));
//         Serial.println(text[0]);
//         Serial.println(_Ki);
//         in = true;
//       }
//       else if(text[0] == 'v') { 
//         _Tp = int((text[1]));
//         if(_Tp <0) _Tp += 256;
//         Serial.println(text[0]);
//         Serial.println(_Tp);
//         in = true;
//       }
//       Serial.println("Sending...");
//       Send_OK();  
//     }
//     if(sended && in == false){ Serial.println("Done..."); _cmd = 't'; break; }
//   }
//   _cmd = 't';
}

void Final_Mode() {
   //TODO
   /***********************************************/
   /* 1. Receive action from py */
   /* 2. Scan for RFID while tracking */
   /* 3. Send RFID when succesfully scanned */
   /* repeat from 1 */
   /***********************************************/
   /*   Start from first node
        python:   GetAction(car_dir, nd_from, nd_to)
                  SendAction to Arduino
        Arduino:  Receive action from python
                  Move to next node
                  read RFID
                  Send RFID to python
        python:   Receive RFID
                  Process and get next action
                  send to Arduino
        Arduino:  repeat from previous
        Python:   Repeat from previous until final node is reached
                  send 'e' (end cycle)
        Arduino:  stop motors
                  break connection              
      */
//  ask_direction();
  //Tracking
  Tracing();
}

void get_cmd(char &cmd) {
   delay(30); // Don't delete its!!
   
   /*************************************/
   /* Using BT object to get command */
   /* Assign value to cmd               */
   /*************************************/
   if(true) {
     //TODO
     // what I write START
     // just record the char what BT read
    if(BT.available()) {
        cmd = BT.read();
        Serial.print("Received: ");
        Serial.println(cmd);
    } else {
        cmd = 'n';
    }
     // what I write END
     // For debugging you can ignore this
     #ifdef DEBUG
     Serial.print("Cmd: ");
     Serial.println(cmd, DEC);
     #endif
   }
}


void MotorCheck() {
   MotorWriting(200, 200);
   delay(500);
   MotorWriting(0, 0);   
   delay(500);
   MotorWriting(-200, -200);
   delay(500);
   MotorWriting(0, 0);
   delay(500);
   MotorWriting(200, 0);
   delay(500);
   MotorWriting(0, 0);
   delay(500);
   MotorWriting(0, 200);
}

void Send_OK() {
   BT.write('O');
}
