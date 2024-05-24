#include <Adafruit_MAX31865.h>
#include <Servo.h>
#include <constants.h>

Servo stripMotor;
Servo electrodeLiftServo;


void setup() {
  stripMotor.attach(STRIP_SERVO_PIN);
  electrodeLiftServo.attach(ELECTRODE_SERVO_PIN);
}

void loop() {
  
}
