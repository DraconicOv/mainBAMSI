#include <LiquidCrystal_I2C.h>
#include <Adafruit_MAX31865.h>
#include <Servo.h>
#include <constants.h>

Servo stripMotor;
Servo electrodeLiftServo; 
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address, cols, rows //TODO: Double check address
// tell the screen to write on the top row
// lcd.setCursor(0,0);
// tell the screen to write “hello, from” on the top  row
// lcd.print(“Hello, From”);  
// tell the screen to write on the bottom  row
// lcd.setCursor(0,1);

void setup() {
  stripMotor.attach(STRIP_SERVO_PIN);
  electrodeLiftServo.attach(ELECTRODE_SERVO_PIN);
}

void loop() {
  // stripMotor.write();
}
