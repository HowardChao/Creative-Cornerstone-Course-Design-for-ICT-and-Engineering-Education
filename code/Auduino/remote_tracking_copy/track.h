#include<SoftwareSerial.h>

// TODO: Write the functions from what you learned in former labs.
void Tracing() {
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

//   Serial.println(r2);
//   Serial.println(r1);
//   Serial.println(m);
//   Serial.println(l1);
//   Serial.println(l2);

  
   //TODO
   // Using "MotorWriting()" to turn or go straight
   // depending on the sensors value
   
   
   // HIGH = when black road is sensed
   int high_number = 0;
   high_number = (r2 == HIGH) + (r1 == HIGH) + (m == HIGH) + (l1 == HIGH) + (l2 == HIGH); 
//   Serial.print("High_number : ");`
//   Serial.println(high_number);
  if ((r2 == HIGH) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {  
    // big right turn
    MotorWriting(-100, 250);
  } else if ((r2 == HIGH) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
    // small right turn 
    MotorWriting(-50, 250);
  } else if ((r2 == LOW) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
    // small right turn 
    MotorWriting(50, 200);
  } else if ((r2 == LOW) && (r1 == HIGH) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
    // small right turn
    MotorWriting(150, 200);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
    // center
    MotorWriting(150, 180);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == HIGH) && (l2 == LOW)) {
    // small left turn
    MotorWriting(200, 150);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == LOW)) {
    // small left turn
    MotorWriting(200, 50);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == HIGH)) {
    // small left turn
    MotorWriting(250, -50);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == HIGH)) {
    // big left turn
    MotorWriting(250, -100);
  } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
    MotorWriting(100, 100);
  } else if (high_number >= 3) {
      /**
       * Here is the place to meet the node !!!
       */
     // Send back to Python !!  
    if (true) {
        if (BT.available()) {
//          BT.write('N');
        }
//        Serial.print("Sent id: ");
        Serial.println('N');
        while(1) {
          get_cmd(_cmd);
          if (_cmd == 'f' ||_cmd == 'b' ||_cmd == 'r' ||_cmd == 'l' || _cmd == 'h') {
            break;
          }
        }
     }
         if (_cmd == 'f') {
            // Advance !!
            right_motor = 230;
            left_motor = 200;
          } else if (_cmd == 'b') {
            // U-Turn
            right_motor = 200;
            left_motor = -200;
          } else if (_cmd == 'r') {
            right_motor = 0;
            left_motor = 230;
          } else if (_cmd == 'l') {
            // Turn right
            right_motor = 200;
            left_motor = 0;
          } else if (_cmd == 'h') {
            // Halt
            right_motor = 0;
            left_motor = 0;
            MotorWriting(right_motor, left_motor);
            delay(5000);
          }
        MotorWriting(right_motor, left_motor);
        delay(700);
//        MotorWriting(100, 100);
//        delay(300);
            /**
             * Testing only!!!!
             */
//             Left turn
//            Serial.println("Invalid cmd!!!");
//            Serial.println(_cmd);
   }
}
