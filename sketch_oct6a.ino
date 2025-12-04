
//my car
char val;
int leftSensor;
int rightSensor;

const int trigPin = 11;
const int echoPin = 10;
const int leftLineSensor = 2;
const int rightLineSensor = 12;
const int enA = 9; // Motor speed control for motor 1
const int enB = 3; // Motor speed control for motor 2
const int in1 = 8;
const int in2 = 7;
const int in3 = 5;
const int in4 = 4;

// Define pin for the LED
const int ledPin = 13;

// Variables to store the duration and distance
long duration;
int distance;

// Mode flag: 0 = manual, 1 = auto
int mode = 0;

void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  
  // Set the trigPin as an OUTPUT
  pinMode(trigPin, OUTPUT);
  
  // Set the echoPin as an INPUT
  pinMode(echoPin, INPUT);
  
  // Set the ledPin as an OUTPUT
  pinMode(ledPin, OUTPUT);

  // Initialize motor driver pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(leftLineSensor, INPUT);
  pinMode(rightLineSensor, INPUT);
}

void loop() {

  leftSensor = digitalRead(leftLineSensor);
  rightSensor = digitalRead(rightLineSensor);
  
  // Read from Serial
  if (Serial.available()) {
    val = Serial.read();
    Serial.println(val);
    
    // Switch to auto mode if 'M' is received
    if (val == 'A' && mode != 1) {
      mode = 1;
      stopMotors();  // Stop motors when switching to auto mode
    }
    // Switch to manual mode if 'A' is received
    else if (val == 'M' && mode != 0) {
      mode = 0;
      stopMotors();  // Stop motors when switching to manual mode
    }
  }

  // Auto mode (Obstacle avoidance)
  if (mode == 1) {
    autoMode();
  }
  
  // Manual mode (Bluetooth control)
  else if (mode == 0) {
    manualMode();
  }

  delay(100);
}

// Auto mode: obstacle avoidance
void autoMode() {
    distance = getDistance();
  // Stop the motors if an obstacle is closer than 20 cm
  if (distance < 20) {
    stopMotors();
    digitalWrite(ledPin, HIGH);
  } else  {
    moveForward();
    digitalWrite(ledPin, LOW);

    if(leftSensor == 1){
       turnLeft();
       delay(2500); 
    }else if(rightSensor == 1){
      turnRight();
       delay(2500); 
    }
  }
    
}

// Function to measure distance using the HC-SR04 ultrasonic sensor
int getDistance() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  
  duration = pulseIn(echoPin, HIGH);
  
  
  int distance = duration * 0.034 / 2;
  
  return distance;
}



// Manual mode: controlled  Bluetooth commands
void manualMode() {
  if (val == '1') {
    turnRight();
  } else if (val == '2') {
    stopMotors();
  } else if (val == '3') {
    moveForward();
  } else if (val == '4') {
    turnLeft();
  }else if(val == '5'){
    moveReverce();
  }
}




void moveForward() {
  Serial.println("Move Forward");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
}

void moveReverce() {
  Serial.println("Move Reverce");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 100);
  analogWrite(enB, 100);
}

void stopMotors() {
  Serial.println("Stop");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void turnLeft() {
  Serial.println("Turn Left");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 0);  
  analogWrite(enB, 100);  
}

void turnRight() {
  Serial.println("Turn Right");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 100); 
  analogWrite(enB, 0);  
}