
#include <SD.h>
#include <Dictionary.h>
#include <DictionaryDeclarations.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MAX31865.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>
#include "constants.h"


Servo stripMotor;
Servo electrodeLiftServo; 
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address, cols, rows //TODO: Double check address
// tell the screen to write on the top row
// lcd.setCursor(0,0);
// tell the screen to write “hello, from” on the top  row
// lcd.print(“Hello, From”);  
// tell the screen to write on the bottom  row
// lcd.setCursor(0,1);
/*Any value greater than 90 causes the servo to rotate clockwise, and determines the speed.

//https://docs.google.com/document/d/1tfQDYl6ygSYfJ7vo3q0bGWBjyIYwakpL/edit

For instance, myservo.write(95) will make the motor start rotating very slowly while using myservo.write(180) sets the motor at full clockwise speed.

Similarly, using a value less between 0 and 90 will reverse the direction of rotation. */
int strip0Pos = 0;
int servoPos = 0; // the amount of ms from the 0 position the servo is.
int electrode0Pos = 0;
int electrodePos = 130; // start slightly above center, might want to be closer to 20-30 depending on orientation of servo
Adafruit_MAX31865 thermo = Adafruit_MAX31865(MAX31865_pin_0, MAX31865_pin_1, MAX31865_pin_2, MAX31865_pin_3); 
bool start = false;
int userSetTemp = 0; 
int currentWell = 0;
Dictionary &screens = *(new Dictionary());
Dictionary &currentScreen = *(new Dictionary()); 
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *lensMotor = AFMS.getStepper(64, 1);

int joystickXValue = 0 ; // To store value of the X axis
int joystickYValue = 0 ; // To store value of the Y axis
// unsigned long time;
bool reset = false;
int buttonValue = 0;
int loading_time = 0;

void loadingCycle(){
  Serial.println("Loading Cycle");
  
  // Serial.println(digitalRead(STRIP_MOVEMENT_LIMIT_PIN) != HIGH);

  // Move servo StripServo until limit switch StripLim is reached, reset counter Strip0Pos 
  while(digitalRead(STRIP_MOVEMENT_LIMIT_PIN) != HIGH){ // Move forward until we limit switch is reached
    stripMotor.write(SERVO_FORWARD); //can increase if necessary, will make it move faster
    // Serial.println(digitalRead(STRIP_MOVEMENT_LIMIT_PIN) != HIGH);
  }
  stripMotor.write(SERVO_STOP); // Stop the servo
  Serial.println(loading_time);
  Serial.println(stripMotor.readMicroseconds());
  // 351
  stripMotor.write(SERVO_STOP); // Stop the servo
  stripMotor.write(SERVO_BACKWARD); // Move to the image position
  delay(STRIP_IMG_POS); // Wait for the servo to move to the image position
  stripMotor.write(SERVO_STOP); // Stop the servo
  servoPos = STRIP_IMG_POS;

}

void unloadingCycle(){
  //Move servo StripServo backwards to Img0Pos
  stripMotor.write(SERVO_FORWARD);
  delay(EJECT_POS-servoPos);
  stripMotor.write(SERVO_STOP);
}

void electrodeServoConf(){
  // move electrode servo backwards until limit switch ElektrLim is reached, reset counter Electr0Pos, then move servo into stimulation position
  int electrodePos = 0;
  Serial.print("electrode config started electrode at ");
  Serial.println(electrodePos);
  Serial.println(digitalRead(ELECTRODE_LIFT_LIMIT_PIN) != HIGH);
  while (digitalRead(ELECTRODE_LIFT_LIMIT_PIN) != HIGH)
  {
    electrodePos+=1; // Might need to be -- depending on orientation of servo
    electrodeLiftServo.write(electrodePos);
    delay(1);
    Serial.println(digitalRead(ELECTRODE_LIFT_LIMIT_PIN));
  }
  Serial.println("electrode done");
  electrode0Pos = electrodePos;
  electrodeLiftServo.write(0);
}
// Temporary function, determine how to determine the stimulation position and how to move electrode there.
void moveElectrodeToStim(){
  electrodeLiftServo.write(ELECTRODE_STIM_POS);
  delay(1000); // Wait for the servo to move to the stimulation position
}
// TODO: this function
void cameraTrigger(){
  analogWrite(ELECTRODE_LED_PIN, ELECTRODE_LED_POWER);
  // More information required to determine how to trigger the camera 
  analogWrite(ELECTRODE_LED_PIN, 0);
}
//TODO: this function
void electrodeTrigger(){
  // More information required to determine how to trigger the electrode
}
// TESTME
void moveElectrodeToRest(){
  for (int currentPos = electrodeLiftServo.read(); currentPos < electrode0Pos; currentPos++)
  {
    electrodeLiftServo.write(currentPos);
    delay(10);
  }
}

void configCamera(){
  Serial.println("Configuring Camera");
  Serial.println(digitalRead(LENS_MOVEMENT_LIMIT_PIN) != HIGH);
  lensMotor -> onestep(BACKWARD, DOUBLE);
  // lensMotor -> step(100, BACKWARD, SINGLE);

  while(digitalRead(LENS_MOVEMENT_LIMIT_PIN) != HIGH){
  }
  // lensMotor -> step(CAMERA_POS, BACKWARD, MICROSTEP);
}

// TODO: this function
void setHeat(bool heat){
  if(heat){
    //Enable heat, TODO
  } else {
    //Disable heat, TODO
  }
}

void tempControlLoop(){
  setHeat(thermo.temperature(MAX31865_RNOMINAL, MAX31865_RREF) < userSetTemp);
}

void moveStripByWellDist(){
  stripMotor.write(SERVO_FORWARD);
  delay(WELL_DIST);
  stripMotor.write(SERVO_STOP);
}
// TODO
boolean canceled(){
  if (true){ // make actual input check
    return false;
  }
  else{
    start = false;
    return true;
  }
}
// TODO 
void updateInputs(){
  // gets the inputs from the user and updates all related variables.
  // joystickXValue = analogRead(JOYSTICK_X_PIN);  
  // joystickYValue = analogRead(JOYSTICK_Y_PIN);
  // buttonValue = 0;
  // if (digitalRead(JOYSTICK_BUTTON_PIN) && !reset){
  //   time = millis();
  //   reset = true;
  // }
  // else if (!digitalRead(JOYSTICK_BUTTON_PIN) && reset)
  //   reset = false;
  // if (digitalRead(JOYSTICK_BUTTON_PIN) && (millis() - time >= LONG_PRESS_DURATION)){
  //   buttonValue = 2;
  // }
  // else if (digitalRead(JOYSTICK_BUTTON_PIN) && (millis() - time >= SHORT_PRESS_DURATION)){
  //   buttonValue = 1;
  // }
}

void stimCycle(){
  if (currentWell == 8){
    start = false;
    return unloadingCycle();
  }
  currentWell++;
  moveElectrodeToStim();
  // TODO: Determine which LEDs to turn on
  analogWrite(SIDE_LED_PIN, SIDE_LED_POWER);
  analogWrite(BOTTOM_LED_PIN, BOTTOM_LED_POWER);
  delay(DELAY_DURATATION);
  //Camera Trigger: TBD
  cameraTrigger();
  delay(DELAY_DURATATION);
  //Electrode Trigger: TBD
  electrodeTrigger();
  analogWrite(SIDE_LED_PIN, 0);
  analogWrite(BOTTOM_LED_PIN, 0);
  delay(DELAY_DURATATION);
  moveElectrodeToRest();
  moveStripByWellDist();
}

void allLEDsOn(){
  analogWrite(SIDE_LED_PIN, SIDE_LED_POWER);
  analogWrite(BOTTOM_LED_PIN, BOTTOM_LED_POWER);
  analogWrite(ELECTRODE_LED_PIN, ELECTRODE_LED_POWER);
}
void allLEDsOff(){
  analogWrite(SIDE_LED_PIN, 0);
  analogWrite(BOTTOM_LED_PIN, 0);
  analogWrite(ELECTRODE_LED_PIN, 0);
}

void setup() {
  // File a = SD.open("screens.json"); // Open the screens file
  // screens.jload(a); // Load the screens file into the screens dictionary
  // a.close(); // Close the screens file
  // currentScreen.jload(screens["Status Screen"]);
  stripMotor.attach(STRIP_SERVO_PIN);
  stripMotor.write(SERVO_STOP);
  electrodeLiftServo.attach(ELECTRODE_SERVO_PIN);
  pinMode(STRIP_MOVEMENT_LIMIT_PIN, INPUT_PULLUP); // HIGH reading means switch is down, or not connected to ground
  pinMode(ELECTRODE_LIFT_LIMIT_PIN, INPUT_PULLUP); // HIGH reading means switch is down, or not connected to ground
  pinMode(LENS_MOVEMENT_LIMIT_PIN, INPUT_PULLUP); // HIGH reading means switch is down, or not connected to ground
  pinMode(ELECTRODE_LED_PIN, OUTPUT);
  pinMode(BOTTOM_LED_PIN, OUTPUT);
  pinMode(SIDE_LED_PIN, OUTPUT);
  allLEDsOff();
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  lensMotor -> setSpeed(60);
  Serial.begin(9600);
  // loadingCycle();
  // electrodeServoConf();
  configCamera();
}

void loop() { 
  // updateInputs();
  // if (start && !canceled()){
  //   stimCycle();
  // }
  // electrodeLiftServo.detach();
  // stripMotor.detach();
} 
