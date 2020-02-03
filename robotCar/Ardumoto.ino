/*
  Ardumoto Code

  1/17/2020
*/

// Variables for control switches
int input1 = 9;
int input2 = 10;
int input1State = 0;
int input2State = 0;

// Variables for motors
int dirA = 12;
int pwmA = 3;
int dirB = 13;
int pwmB = 11;

// Status indicators
int standbyLED = 5;
int forwardLED = 6;
int reverseLED = 7;
int turnLED = 8;

// Infrared sensors
int IRsensorPin = A0;
int IRsensorValue = 0;
int IRsensorPinRight = A1;
int IRsensorValueRight = 0;

// for the wall follow part:
int LastIRsensorValueRight;

// wait routine
void wait();
int waitTime = 750;

void setup() {
  Serial.begin(9600);

  // Motor controller
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(dirA, OUTPUT);
  pinMode(dirB, OUTPUT);

  // Switches
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);

  // Status indicators
  pinMode(standbyLED, OUTPUT);
  pinMode(forwardLED, OUTPUT);
  pinMode(reverseLED, OUTPUT);
  pinMode(turnLED, OUTPUT);

  // Wall Follow:
  LastIRsensorValueRight = analogRead(IRsensorPinRight);
}


void loop() {
  // Getting state
  input1State = digitalRead(input1);
  input2State = digitalRead(input2);

  // 00 - Sit ---------------------------------------------------------------------------
  if (input2State == LOW && input1State == LOW) {
    // wait routine
    wait();
  }


  // 01 - Straight and speed ------------------------------------------------------------

  else if (input2State == LOW && input1State == HIGH) {
    // wait a few seconds first
    wait();

    // turning on forwardLED indicator
    digitalWrite(forwardLED, HIGH);

    // Set dirA to HIGH and dirB to LOW so it drives straight
    // dirB is reversed for some reason
    digitalWrite(dirA, HIGH);
    digitalWrite(dirB, LOW);

    // Update serial moniter
    Serial.println("Status: moving forward...");

    // Motor power 0.20m/s for 10 seconds going forward
    Serial.println("Status: moving forward at 150 PWM...");
    analogWrite(pwmA, 65);
    analogWrite(pwmB, 65);
    delay(10000);

    // wait
    wait();

    // Motor power 0.4m/s for 5 seconds going forward
    Serial.println("Status: moving forward at 255 PWM...");
    analogWrite(pwmA, 130);
    analogWrite(pwmB, 130);
    delay(5000);

    // wait
    wait();

    // Reversing direction to go backwards
    Serial.println("Reversing direction");
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, HIGH);

    // Motor power 0.20m/s for 10 seconds going backward
    Serial.println("Status: moving backward at 150 PWM...");
    analogWrite(pwmA, 65);
    analogWrite(pwmB, 65);
    delay(10000);

    // wait
    wait();

    // Motor power 0.4m/s for 5 seconds going backward
    Serial.println("Status: moving backward at 255 PWM...");
    analogWrite(pwmA, 130);
    analogWrite(pwmB, 130);
    delay(5000);

    // repeat
  }


  // 10 - Pivot and turn ---------------------------------------------------------------
  else if (input2State == HIGH && input1State == LOW) {
    // Turn off standby led
    digitalWrite(standbyLED, LOW);
    // Turn off forward led
    digitalWrite(forwardLED, LOW);
    // Turn off reverse led
    digitalWrite(reverseLED, LOW);
    // Turn on turn led
    digitalWrite(turnLED, HIGH);

    wait();

    // Set one direction low and one high so that it spins (clockwise)
    digitalWrite(turnLED, HIGH);
    digitalWrite(dirA, HIGH);
    digitalWrite(dirB, HIGH);
    // Motor power low (127)
    analogWrite(pwmA, 127);
    analogWrite(pwmB, 127);
    // The robot was spinning about 5 times in 5 seconds so for 3 spins:
    delay(2600);

    wait();

    // Spin the other direction (counter-clockwise)
    // Home calibration: same time as last one but spins a little bit farther.
    digitalWrite(turnLED, HIGH);
    digitalWrite(dirA, LOW);
    digitalWrite(dirB, LOW);
    // Motor power low (150)
    analogWrite(pwmA, 127);
    analogWrite(pwmB, 127);
    delay(2650);

    wait();
    digitalWrite(turnLED, HIGH);
    // Turn 90 degrees
    digitalWrite(dirA, HIGH);
    // Motor power to one wheel
    analogWrite(pwmA, 150);
    analogWrite(pwmB, 0);
    delay(400);

    wait();
    digitalWrite(turnLED, HIGH);
    // Turn -90 degrees
    digitalWrite(dirA, LOW);
    // Motor power to one wheel
    analogWrite(pwmA, 150);
    analogWrite(pwmB, 0);
    delay(400);

    wait();
    digitalWrite(turnLED, HIGH);
    // Turn 90 degrees
    digitalWrite(dirB, HIGH);
    // Motor power to one wheel
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 150);
    delay(400);

    wait();
    digitalWrite(turnLED, HIGH);
    // Turn -90 degrees
    digitalWrite(dirB, LOW);
    // Motor power to one wheel
    analogWrite(pwmA, 0);
    analogWrite(pwmB, 150);
    delay(400);

    wait();
    digitalWrite(turnLED, HIGH);
    // Update serial moniter
    Serial.println("Status: turning...");
    delay(500);
  }


  // 11 - Follow the wall ----------------------------------------------------------------
  else if (input2State == HIGH && input1State == HIGH) {
    IRsensorValue = analogRead(IRsensorPin);
    IRsensorValueRight = analogRead(IRsensorPinRight);
    Serial.print("Front Sensor: ");
    Serial.print(IRsensorValue);
    Serial.print("\tRight Sensor: ");
    Serial.println(IRsensorValueRight);

    
    if (IRsensorValue <= 300)
    {
      // forward
      digitalWrite(dirA, LOW);
      digitalWrite(dirB, HIGH);
      analogWrite(pwmA, 200);               // reset these numbers for different floor.
      analogWrite(pwmB, 200);
    }

    else if (IRsensorValue >= 300)
    {
      // stop,
      analogWrite(pwmA, 0);
      analogWrite(pwmB, 0);

      digitalWrite(turnLED, HIGH);
      // Turn -90 degrees using right wheel

      digitalWrite(dirA, HIGH);
      // Motor power to one wheel
      analogWrite(pwmA,150);
      delay(400);                          // same number calibrated above with 90 deg turns
      }
    }

    else
    {
      digitalWrite(turnLED,LOW);
      // distance from wall adjustments:
      int count = 0;

      while (IRsensorValueRight < 350 && IRsensorValue <= 300)      // when RobotCar is too far from wall.
      {                                                             // second part checks for stuff in front
        IRsensorValueRight = analogRead(IRsensorPinRight);
        analogWrite(pwmB, 150);
        analogWrite(pwmA, 200);
        if (count > 100)            // if it gets stuck and slowing pwmB is not enough:
        {
          analogWrite(pwmA, 250);
          analogWrite(pwmB, 150);
          delay(1500);
        }
        count++;
      }

      count = 0;
      while (IRsensorValueRight > 450 && IRsensorValue <= 300)      // when RobotCar is too close
      {                                                             // and there is nothing in front.
        IRsensorValueRight = analogRead(IRsensorPinRight);
        analogWrite(pwmA, 150);
        analogWrite(pwmB,200);
        if (count > 100)
        {
          analogWrite(pwmB, 250);
          analogWrite(pwmA, 150);
          delay(1500);
        }
        count++;
      }

      // if it runs out of wall:
      int diff;
      // Initialize "LastIRsensorValueRight" for wall follow:
      diff = abs(IRsensorValueRight - LastIRsensorValueRight);
      if (diff > 200 && IRsensorValue <= 200)                       // depending how quickly the sensor value changes : NEEDS 2 B CALIBRATED/RECALIBRATED/RE-RECALIBRATED
      {
        analogWrite(pwmA, 0);
        analogWrite(pwmB, 0);
        analogWrite(dirB, LOW);
        while (IRsensorValueRight > 600)
        {
          IRsensorValueRight = analogRead(IRsensorPinRight);
          analogWrite(pwmB, 80);
          analogWrite(pwmA, 80);
        }
      }
      // used for detecting changes in the wall distance.
      LastIRsensorValueRight = IRsensorValueRight;
    }
}




// wait routine, waits for 3 seconds, blinks led twice
void wait() {
  // Printing to serial
  Serial.println("Status: waiting. . .");

  // Turn off forward led
  digitalWrite(forwardLED, LOW);
  // Turn off reverse led
  digitalWrite(reverseLED, LOW);
  // Turn off turn led
  digitalWrite(turnLED, LOW);

  // Motor power to 0
  analogWrite(pwmA, 0);
  analogWrite(pwmB, 0);

  // blinks LED every 750 ms, since it blinks four times the whole wait routine lasts 3000 ms or 3 seconds
  delay(waitTime);
  digitalWrite(standbyLED, HIGH);
  delay(waitTime);
  digitalWrite(standbyLED, LOW);
  delay(waitTime);
  digitalWrite(standbyLED, HIGH);
  delay(waitTime);
  digitalWrite(standbyLED, LOW);
}
