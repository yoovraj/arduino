int ML[2] = {11,10};
int MR[2] = {13,12};
const int numReadings = 10;
int DIST_SENSOR = A0;
int PWM_PIN_L = 5;
int PWM_PIN_R = 6;

void setup() {
  // left motor
  pinMode(ML[0], OUTPUT);
  pinMode(ML[1], OUTPUT);
  
  // roght motor
  pinMode(MR[0], OUTPUT);
  pinMode(MR[1], OUTPUT);

  // pwm
  pinMode(PWM_PIN_L, OUTPUT);
  pinMode(PWM_PIN_R, OUTPUT);
  
  Serial.begin(9600);

}

void loop() {
    // read the input on analog pin 0:
  int sensorValue = getAverageDistance();
  Serial.println(sensorValue);
  analogWrite(PWM_PIN, 255/sensorValue);
}

int getAverageDistance() {
  int total = 0;
  for (int i = 1; i <= numReadings; i++) {
    // read from the sensor:
    total = total + analogRead(DIST_SENSOR);
  }
  int avg = total/numReadings;
  int adjVal = 0;
  if (avg <100) {
    adjVal = 1;
  } else if (avg < 600) {
    adjVal = (avg - 100) / 100;
  } else {
    adjVal = 5;
  }
  return adjVal;
}
