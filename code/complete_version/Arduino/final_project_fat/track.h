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
  if (high_number != 5) {
    if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      r2 = r2_p;
      r1 = r1_p;
      m = m_p;
      l1 = l1_p;
      l2 = l2_p;
    }
    if ((r2 == HIGH) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      MotorWriting(-100, 200);
    } else if ((r2 == HIGH) && (r1 == HIGH) && (m == LOW) && (l1 == LOW) && (l2 == LOW)) {
      MotorWriting(0, 200);
    } else if ((r2 == HIGH) && (r1 == HIGH) && (m == HIGH) && (l1 == LOW) && (l2 == LOW)) {
      MotorWriting(80, 200);
    } else if ((r2 == HIGH) && (r1 == HIGH) && (m == HIGH) && (l1 == HIGH) && (l2 == LOW)) {
      MotorWriting(180, 200);
    } else if ((r2 == LOW) && (r1 == HIGH) && (m == HIGH) && (l1 == HIGH) && (l2 == LOW)) {
      MotorWriting(200, 200);
    } else if ((r2 == LOW) && (r1 == HIGH) && (m == HIGH) && (l1 == HIGH) && (l2 == HIGH)) {
      MotorWriting(200, 180);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == HIGH) && (l1 == HIGH) && (l2 == HIGH)) {
      MotorWriting(200, 80);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == HIGH) && (l2 == HIGH)) {
      MotorWriting(200, 0);
    } else if ((r2 == LOW) && (r1 == LOW) && (m == LOW) && (l1 == LOW) && (l2 == HIGH)) {
      MotorWriting(200, -100);
    } else {
//      MotorWriting(-60, -60);
    }
      r2_p = r2;
      r1_p = r1;
      m_p = m;
      l1_p = l1;
      l2_p = l2;
    return false;  // Doesn't meet a node.
  } else {
    if (counter >= COUNTER_CONST) {
      // qualify for new node
      MotorWriting(0, 0);
    } else {
      MotorWriting(200, 200);
    }
    // Here high_number >= 5 is the condition of meeting a node.
#ifdef DEBUG
    Serial.println("The car now meets a node.");
#endif
    return true;  // Meet a node.
  }
}
