#include <Servo.h>

const int numReadings = 5;
Servo myservo;  // create a servo object
int pos = 0;

int ML[2] = {11,10};
int MR[2] = {13,12};
int PWM_PIN_L = 5;
int PWM_PIN_R = 6;
int PWM_L = 255;
int PWM_R = 255;
int PWM_DIFF = 256/4;

int SERVO_PIN = 9;
int DIST_SENSOR = A0;
int LIMIT_DISTANCE = 300;
void setup() {
  // put your setup code here, to run once:
  // left motor
  pinMode(ML[0], OUTPUT);
  pinMode(ML[1], OUTPUT);
  
  // roght motor
  pinMode(MR[0], OUTPUT);
  pinMode(MR[1], OUTPUT);

  // pwm
  pinMode(PWM_PIN_L, OUTPUT);
  pinMode(PWM_PIN_R, OUTPUT);

  myservo.attach(SERVO_PIN);

  Serial.begin(9600);

}

void loop() {
  // read the input on analog pin 0:
  int sensorValue = getAverageDistance();
//  Serial.println(sensorValue);
  Serial.println("-L-R-");
  Serial.println("dasdasda");
  Serial.println(PWM_L);
  Serial.println(PWM_R);

  decide_turn();
  
}

void decide_turn() {

  // check right
  int left_count=0, right_count=0;
  int cnt=0;
  long tmp=0;
  myservo.write(110);              // tell servo to go to position in variable 'pos'
  delay(100);                       // waits 15ms for the servo to reach the position
  for (pos = 110; pos < 110 + 70; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    int distance = getAverageDistance();
    tmp = tmp + distance;cnt++;
    if (distance > LIMIT_DISTANCE) {
      left_count++;
    }
  }

  int avgLeft = tmp / cnt;
//  PWM_L = normalizeDistance(avgLeft);
  // bring to center
  myservo.write(110);
  delay(100);

  // check left
  tmp = 0; cnt = 0;
  for (pos = 110; pos > 110 - 70; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    int distance = getAverageDistance();
    tmp = tmp + distance;cnt++;
    if (distance > LIMIT_DISTANCE) {
      right_count++;
    }
  }

  int avgRight = tmp / cnt;
  PWM_R = normalizeDistance(avgRight);

  // decide
  Serial.println(left_count);
  Serial.println(right_count);

  if (left_count > 3 || right_count > 3) {
    if (left_count > right_count) {
      // turn right
      Serial.println("TURN RIGHT left>right");
      right();
    } else {
      // turn left
      Serial.println("TURN LEFT left<right");
      left();
    }
  } else {
    PWM_L = 255;
    PWM_R = 255;
    forward();
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

int normalizeDistance(int val) {
  int MIN = 100;
  int MAX = 600;
  int adjVal = 0;
  if (val < MIN) {
    adjVal = 0;
  } else if (val < MAX) {
    adjVal = (val - MIN) * 255.0 / (MAX-MIN);
  } else {
    adjVal = 255;
  }
  Serial.println("-------");
  Serial.println(val);
  Serial.println(adjVal);
  Serial.println("-------");

  return adjVal;
}

void halfSpeed() {
  PWM_L = 255/2;
  PWM_R = 255/2;
  forward();
}
// forward move
void forward() {
  // move left and right motors forward
  //digitalWrite(ML[0], HIGH);
  analogWrite(PWM_PIN_L, PWM_L);
  digitalWrite(ML[1], LOW);
  
  //digitalWrite(MR[0], HIGH);
  analogWrite(PWM_PIN_R, PWM_R);
  digitalWrite(MR[1], LOW);
}

// reverse move
void reverse() {
  // move left and right motors forward
  //digitalWrite(ML[0], LOW);
  analogWrite(PWM_PIN_L, 255 - PWM_L);
  digitalWrite(ML[1], HIGH);
  
  //digitalWrite(MR[0], LOW);
  analogWrite(PWM_PIN_R, 255 - PWM_R);
  digitalWrite(MR[1], HIGH);
}

// left move
void left() {
  // move left motor backwards 
//  digitalWrite(ML[0], LOW);
  analogWrite(PWM_PIN_L, 255 - PWM_L);
  digitalWrite(ML[1], HIGH);
  
  // move right motor forward
//  digitalWrite(MR[0], HIGH);
  analogWrite(PWM_PIN_R, PWM_R);
  digitalWrite(MR[1], LOW);
}

// forward the motor
void right() {
  // move left motor backwards 
  //digitalWrite(ML[0], HIGH);
  analogWrite(PWM_PIN_L, PWM_L);
  digitalWrite(ML[1], LOW);
  
  // move right motor forward
  //digitalWrite(MR[0], LOW);
  analogWrite(PWM_PIN_R, 255-PWM_R);
  digitalWrite(MR[1], HIGH);
}

void freeStop() {
  //digitalWrite(ML[0], LOW);
  analogWrite(PWM_PIN_L, 255-PWM_L);
  digitalWrite(ML[1], LOW);
  
  //digitalWrite(MR[0], LOW);
  analogWrite(PWM_PIN_R, 255-PWM_R);
 digitalWrite(MR[1], LOW);
  delay(1000);
}
