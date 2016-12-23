/*
 * Author : Yoovraj Shinde
 * Date : 2016/12/15
 */
 #include <Servo.h>

 Servo s[4];
 int const SERVO_COUNT = 4;
 int SERVO_PORT[4]     = {13, 12, 11, 10};
 int SERVO_FEEDBACK[4] = {A0, A1, A2, A3};
 int const RECORDING_TIME = 15; // in seconds
 int const SAMPLE_INTERVAL = 100; // in ms
 int const SAMPLE_COUNT = RECORDING_TIME*1000/SAMPLE_INTERVAL;
 int const INTERPOLATION_VALUES = 1;
 int samples[SERVO_COUNT][SAMPLE_COUNT];
 int i=0; // will range from 0 - 4
 
 void setup() {
  // setup servo port and feedback pins
  for (i=0; i < SERVO_COUNT ; i++) {
    s[i].attach(SERVO_PORT[i]);
    pinMode(SERVO_FEEDBACK[i], INPUT);
  }
  Serial.begin(9600);
  Serial.print("RECORDING_TIME(s) = ");
  Serial.println(RECORDING_TIME);

  Serial.print("SAMPLE_INTERVAL(ms) = ");
  Serial.println(SAMPLE_INTERVAL);

  Serial.print("SAMPLE_COUNT = ");
  Serial.println(SAMPLE_COUNT);
  resetTo90();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Will enter programming mode after 1s");
  resetTo90();
  delay(1000);
  
  Serial.print("----- programming mode for (in seconds) : ");
  Serial.println(RECORDING_TIME);
  
  i=0;
  while(i < SAMPLE_COUNT) {
    delay(SAMPLE_INTERVAL);

    for (int j=0; j < SERVO_COUNT ; j++) {
      samples[j][i] = analogRead(SERVO_FEEDBACK[j]);
    }
    i++;
    if ((i*100/SAMPLE_COUNT) %20 == 0) {
      Serial.print("Percentage recorded : ");
      Serial.println(i*100/SAMPLE_COUNT);
    }
  }

  Serial.println("Recording complete, now trying to replicate the action");
  // replicate the action
  attachServos();
  i=0;
  int y[4]={90,90,90,90};
  int y0[4];
  while (i < SAMPLE_COUNT) {
    if (i == 0) {
      // for first data, midpoint is (0 + y) / 2;
      for (int j=0; j< SERVO_COUNT; j++) {
        y0[j] = (samples[j][i]-97)/2/2;
      }
    } else {
      for (int j=0; j< SERVO_COUNT; j++) {
        y0[j] = ((samples[j][i-1]+samples[j][i])/2 -97)/2;
      }
    }
    delay(SAMPLE_INTERVAL/2);
    for (int j=0; j< SERVO_COUNT; j++) {
      s[j].write(y0[j]);
    }
    
    for (int j=0; j< SERVO_COUNT; j++) {
      y[j] = (samples[j][i]-97)/2;
    }
    
    delay(SAMPLE_INTERVAL/2);
    for (int j=0; j< SERVO_COUNT; j++) {
      s[j].write(y[j]);
    }
    i++;
  }
  detachServos();
}

void resetTo90() {
  for (int j=0; j < SERVO_COUNT ; j++) {
    s[j].attach(SERVO_PORT[j]);
    s[j].write(90);
    s[j].detach();
  }
}

void attachServos() {
  for (int j=0; j < SERVO_COUNT ; j++) {
    s[j].attach(SERVO_PORT[j]);
  }
}

void detachServos() {
  for (int j=0; j < SERVO_COUNT ; j++) {
    s[j].detach();
  }
}

