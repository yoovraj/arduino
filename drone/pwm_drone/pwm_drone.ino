/*
 * Author : Yoovraj Shinde
 * Date : 2016/06/14
 */
const int numReadings = 10;

int STOP = 7;
int REVERSE = 8;

// pwm pins left_1 and right_1
int LEFT_1 = 5;
int RIGHT_1 = 6;
int LEFT_2 = 9;
int RIGHT_2 = 10;

int LEFT_PWM = 128;
int RIGHT_PWM = 128;

// analog range
int ANALOG_RANGE=663;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // pwm
  pinMode(LEFT_1, OUTPUT);
  pinMode(LEFT_2, OUTPUT);
  pinMode(RIGHT_1, OUTPUT);
  pinMode(RIGHT_2, OUTPUT);

  // default forward
  digitalWrite(LEFT_2, LOW);
  digitalWrite(RIGHT_2, LOW);

  // input pin
  pinMode(REVERSE, INPUT);
  pinMode(STOP, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  int stopStatus = digitalRead(STOP);
  int reverseStatus = digitalRead(REVERSE);
  if (stopStatus == HIGH) {
    Serial.println("STOP");
    stopDrone();
  } else {
    // read the input on analog pin 0:
    int sensorValue = getAverageDistance();
    int convertedValue = convert(sensorValue);
    
    if (reverseStatus == LOW) {
      runDrone(convertedValue, 255-convertedValue);
    } else {
      runReverse(convertedValue);
    }
  }

}

void stopDrone() {
    digitalWrite(LEFT_1, LOW);
    digitalWrite(LEFT_2, LOW);
    digitalWrite(RIGHT_1, LOW);
    digitalWrite(RIGHT_2, LOW);
}
void runDrone(int leftPWM, int rightPWM) {
  analogWrite(LEFT_1, leftPWM);
  analogWrite(RIGHT_1, rightPWM);
  digitalWrite(LEFT_2, LOW);
  digitalWrite(RIGHT_2, LOW);
}

void runReverse(int value) {
  if (value < 64) {
    //turn left
    Serial.println("REVERSE_LEFT");
    digitalWrite(LEFT_1, LOW);
    digitalWrite(LEFT_2, HIGH);
    digitalWrite(RIGHT_1, HIGH);
    digitalWrite(RIGHT_2, LOW);

  } else if (value > 192) {
    // turn right
    Serial.println("REVERSE_RIGHT");
    digitalWrite(LEFT_1, HIGH);
    digitalWrite(LEFT_2, LOW);
    digitalWrite(RIGHT_1, LOW);
    digitalWrite(RIGHT_2, HIGH);
    
  } else {
    // simple back
    Serial.println("REVERSE_");
    digitalWrite(LEFT_1, LOW);
    digitalWrite(LEFT_2, HIGH);
    digitalWrite(RIGHT_1, LOW);
    digitalWrite(RIGHT_2, HIGH);
    
  }
}
// convert the analog reading to 0-255 range
int convert(int value) {
  float tmp = 255.0 * value / ANALOG_RANGE;
  if (tmp > 255) {
    return 255;
  }
  return tmp;
}

// get average analog reading to filter noise
int getAverageDistance() {
  float total = 0.0;
  for (int i = 1; i <= numReadings; i++) {
    // read from the sensor:
    total = total + analogRead(A0);
  }
  return total/numReadings;
}
