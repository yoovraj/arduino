#include <Servo.h>

const int numReadings = 10;
Servo myservo;  // create a servo object
int pos = 0;

int ML[2] = {11,10};
int MR[2] = {13,12};
int SERVO_PIN = 9;
int DIST_SENSOR = A0;
void setup() {
  // put your setup code here, to run once:
  // left motor
  pinMode(ML[0], OUTPUT);
  pinMode(ML[1], OUTPUT);
  
  // roght motor
  pinMode(MR[0], OUTPUT);
  pinMode(MR[1], OUTPUT);

  myservo.attach(SERVO_PIN);

  Serial.begin(9600);

}

void loop() {
  // read the input on analog pin 0:
  int sensorValue = getAverageDistance();
//  Serial.println(sensorValue);

  if (sensorValue > 400) {
    // too close 
    // so stop and check left right 
    decide_turn();
  } else {
    // continue with nothing
    forward();
  }
//  freeStop();
//  forward();
//  
//  delay(3000);
//  freeStop();
//  right();
//
//  delay(3000);
//  freeStop();
//  left();
//  
//  delay(3000);
//  freeStop();
//  reverse();
//  
//  delay(3000);
}

void decide_turn() {

  // check right
  int left_count=0, right_count=0;
  for (pos = 110; pos < 110 + 70; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    int distance = getAverageDistance();
    if (distance > 400) {
      left_count++;
    }
  }

  // bring to center
  myservo.write(110);
  delay(15);

  // check left
  for (pos = 110; pos > 110 - 70; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    int distance = getAverageDistance();
    if (distance > 400) {
      right_count++;
    }
  }

  // decide
  Serial.println(left_count);
  Serial.println(right_count);
  
  if (left_count > right_count) {
    // turn right
    Serial.println("TURN RIGHT left>right");
    right();
  } else {
    // turn left
    Serial.println("TURN LEFT left<right");
    left();
  }

  // bring to center
  myservo.write(110);
  delay(15);
}

int getAverageDistance() {
  int total = 0;
  for (int i = 1; i <= numReadings; i++) {
    // read from the sensor:
    total = total + analogRead(DIST_SENSOR);
  }
  return total/numReadings;
}

// forward move
void forward() {
  // move left and right motors forward
  digitalWrite(ML[0], HIGH);
  digitalWrite(ML[1], LOW);
  digitalWrite(MR[0], HIGH);
  digitalWrite(MR[1], LOW);
}

// reverse move
void reverse() {
  // move left and right motors forward
  digitalWrite(ML[0], LOW);
  digitalWrite(ML[1], HIGH);
  digitalWrite(MR[0], LOW);
  digitalWrite(MR[1], HIGH);
}

// left move
void left() {
  // move left motor backwards 
  digitalWrite(ML[0], LOW);
  digitalWrite(ML[1], HIGH);
  
  // move right motor forward
  digitalWrite(MR[0], HIGH);
  digitalWrite(MR[1], LOW);
}

// forward the motor
void right() {
  // move left motor backwards 
  digitalWrite(ML[0], HIGH);
  digitalWrite(ML[1], LOW);
  
  // move right motor forward
  digitalWrite(MR[0], LOW);
  digitalWrite(MR[1], HIGH);
}

void freeStop() {
  digitalWrite(ML[0], LOW);
  digitalWrite(ML[1], LOW);
  digitalWrite(MR[0], LOW);
  digitalWrite(MR[1], LOW);
  delay(1000);
}
