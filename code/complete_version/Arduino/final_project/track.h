// Write the functions about tracing from what you learned in former labs.
bool Tracing() {
  /**************************************************/
  /* Read the sensor value and determine whether to */
  /* turn left, turn right, or go straight          */
  /**************************************************/

  // Initialize Senor value
  int r2 = digitalRead(R2);  // right-outer sensor
  int r1 = digitalRead(R1);  // right-inner sensor
  int m = digitalRead(M);    // middle sensor
  int l1 = digitalRead(L1);  // left-inner sensor
  int l2 = digitalRead(L2);  // left-outer sensor

#ifdef DEBUG
  Serial.print("Sensor values: ");
  Serial.print(r2);
  Serial.print(' ');
  Serial.print(r1);
  Serial.print(' ');
  Serial.print(m);
  Serial.print(' ');
  Serial.print(l1);
  Serial.print(' ');
  Serial.println(l2);
#endif
  // Using "MotorWriting()" to turn or go straight depending on the sensors value

  // HIGH = when black road is sensed
  int high_number = 0;
  high_number = (r2 == HIGH) + (r1 == HIGH) + (m == HIGH) + (l1 == HIGH) + (l2 == HIGH);
#ifdef DEBUG
  Serial.print("High_number: ");
  Serial.println(high_number);
#endif

  if (high_number < 3) {
    if ((r2 == HIGH) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      // big right turn
      MotorWriting(-100, 250);
    } else if ((r2 == HIGH) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      // small right turn
      MotorWriting(-80, 250);
    } else if ((r2 == HIGH) && (r1 == HIGH) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
      // small right turn
      MotorWriting(-50, 200);
    } else if ((r2 == LOW) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      // small right turn
      MotorWriting(50, 200);
    } else if ((r2 == LOW) && (r1 == HIGH) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
      // small right turn
      MotorWriting(90, 180);
    } else if ((r2 == LOW) && (r1 == HIGH) && (m == HIGH) && (l1 == HIGH) && (l2 == LOW)) {
      // center
      MotorWriting(120, 200);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
      // center
      MotorWriting(120, 200);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == HIGH) && (l2 == LOW)) {
      // small left turn
      MotorWriting(170, 100);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == LOW)) {
      // small left turn
      MotorWriting(200, 50);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == HIGH) && (l2 == HIGH)) {
      // small left turn
      MotorWriting(200, -50);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == HIGH)) {
      // small left turn
      MotorWriting(250, -80);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == HIGH)) {
      // big left turn
      MotorWriting(250, -100);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      MotorWriting(100, 100);
    } else {
      MotorWriting(-60, -60);
    }
    return false;  // Doesn't meet a node.
  }
  else {
    // Here high_number >= 3 is the condition of meeting a node.
#ifdef DEBUG
    Serial.println("The car now meets a node.");
#endif
    return true;  // Meet a node.
  }
}
