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
      // big right turn: R 1 0 0 0 0 L
      MotorWriting(-100, 180);
    } else if ((r2 == HIGH) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      // small right turn: R 1 1 0 0 0 L
      MotorWriting(-100, 180);
    } else if ((r2 == LOW) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      // small right turn: R 0 1 0 0 0 L
      MotorWriting(0, 180);
    } else if ((r2 == LOW) && (r1 == HIGH) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
      // small right turn: R 0 1 1 0 0 L
      MotorWriting(100, 180);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
      // center: R 0 0 1 0 0 L
      MotorWriting(200, 135);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == HIGH) && (l2 == LOW)) {
      // small left turn: R 0 0 1 1 0 L
      MotorWriting(180, 100);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == LOW)) {
      // small left turn: R 0 0 0 1 0 L
      MotorWriting(180, 0);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == HIGH)) {
      // small left turn: R 0 0 0 1 1 L
      MotorWriting(180, -100);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == HIGH)) {
      // big left turn: R 0 0 0 0 1 L
      MotorWriting(180, -100);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      // what is this condition? R 0 0 0 0 0 L
      // MotorWriting(100, 100);
    } else {
      // what should do if coming into here?
      // MotorWriting(-60, -60);
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
