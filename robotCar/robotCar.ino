// Variables for control switches
int input1 = 9;
int input2 = 10;
int input1State = 0;
int input2State = 0;

// Variables for motors
int DirA = 12;
int PWM_A = 3;
int DirB = 13;
int PWM_B = 11;

// Status indicators
int standbyLED = 5;
int forwardLED = 6;

void setup() {
  // Beginning serial
  Serial.begin(9600);

  // Switches are inputs
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);

  // Motor controllers are outputs
  pinMode(DirA, OUTPUT);
  pinMode(DirB, OUTPUT);
  pinMode(PWM_A, OUTPUT);
  pinMode(PWM_B, OUTPUT);

  // Status indicators are outputs
  pinMode(standbyLED, OUTPUT);
  pinMode(forwardLED, OUTPUT);
}

void loop() {
  // Getting what state the car should be in
  input1State = digitalRead(input1);
  input2State = digitalRead(input2);

  // 00 is do nothing
  if (input2State == LOW && input1State == LOW) {
    // Turn on standby led
    digitalWrite(standbyLED, HIGH);
    // Turn off forward led
    digitalWrite(forwardLED, LOW);

    // Set motor power to 0
    analogWrite(PWM_A, 0);
    analogWrite(PWM_B, 0);

    // Updating serial monitor
    Serial.println("Status: standby...");
    delay(1000);
  }

  // 01 is drive forward
  else if (input2State == LOW && input1State == HIGH) {
    // Turn off standby led
    digitalWrite(standbyLED, LOW);
    // Turn on forward led
    digitalWrite(forwardLED, HIGH);

    // Set both directions to high so it drives forward
    digitalWrite(DirA, HIGH);
    digitalWrite(DirB, HIGH);

    // Set motor power to max (255)
    analogWrite(PWM_A, 255);
    analogWrite(PWM_B, 255);

    // Updating serial monitor
    Serial.println("Status: moving forward...");
  }

  // 10 is ???
  else if (input2State == HIGH && input1State == LOW) {
    
  }

  // 11 is ???
  else if (input2State == HIGH && input2State == HIGH) {
    
  }
}
