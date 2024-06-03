
#include <SD.h>
#include <Dictionary.h>
#include <DictionaryDeclarations.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MAX31865.h>
#include <Servo.h>
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

int joystickXValue = 0 ; // To store value of the X axis
int joystickYValue = 0 ; // To store value of the Y axis
unsigned long time;
bool reset = false;
int buttonValue = 0;

void loadingCycle(){
  // Move servo StripServo until limit switch StripLim is reached, reset counter Strip0Pos
  while(digitalRead(STRIP_MOVEMENT_LIMIT_PIN) == HIGH){ // Move forward until we limit switch is reached
    stripMotor.write(SERVO_BACKWARD); //can increase if necessary, will make it move faster
    delay(1);
  }
  stripMotor.write(SERVO_STOP); // Stop the servo
  stripMotor.write(SERVO_FORWARD); // Move to the image position
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
  // int electrodePos = electrodeLiftServo.read();
  while (digitalRead(ELECTRODE_LIFT_LIMIT_PIN) == HIGH)
  {
    electrodePos++; // Might need to be -- depending on orientation of servo
    electrodeLiftServo.write(electrodePos);
    delay(10);
  }
  electrode0Pos = electrodePos;
}
// Temporary function, determine how to determine the stimulation position and how to move electrode there.
void moveElectrodeToStim(){
  electrodeLiftServo.write(ELECTRODE_STIM_POS);
  delay(1000); // Wait for the servo to move to the stimulation position
}
// TODO: this function
void cameraTrigger(){
  digitalWrite(LENS_LED_PIN, HIGH);
  // More information required to determine how to trigger the camera 
  digitalWrite(LENS_LED_PIN, LOW);
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
  joystickXValue = analogRead(JOYSTICK_X_PIN);  
  joystickYValue = analogRead(JOYSTICK_Y_PIN);
  buttonValue = 0;
  if (digitalRead(JOYSTICK_BUTTON_PIN) && !reset){
    time = millis();
    reset = true;
  }
  else if (!digitalRead(JOYSTICK_BUTTON_PIN) && reset)
    reset = false;
  if (digitalRead(JOYSTICK_BUTTON_PIN) && (millis() - time >= LONG_PRESS_DURATION)){
    buttonValue = 2;
  }
  else if (digitalRead(JOYSTICK_BUTTON_PIN) && (millis() - time >= SHORT_PRESS_DURATION)){
    buttonValue = 1;
  }
}

void stimCycle(){
  if (currentWell == 8){
    start = false;
    return unloadingCycle();
  }
  currentWell++;
  moveElectrodeToStim();
  // TODO: Determine which LEDs to turn on
  digitalWrite(SIDE_LED_PIN, HIGH);
  digitalWrite(BOTTOM_LED_PIN, HIGH);
  delay(DELAY_DURATATION);
  //Camera Trigger: TBD
  cameraTrigger();
  delay(DELAY_DURATATION);
  //Electrode Trigger: TBD
  electrodeTrigger();
  digitalWrite(SIDE_LED_PIN, LOW);
  digitalWrite(BOTTOM_LED_PIN, LOW);
  delay(DELAY_DURATATION);
  moveElectrodeToRest();
  moveStripByWellDist();
}

void setup() {
  File a = SD.open("screens.json"); // Open the screens file
  screens.jload(a); // Load the screens file into the screens dictionary
  a.close(); // Close the screens file
  currentScreen.jload(screens["Status Screen"]);
  stripMotor.attach(STRIP_SERVO_PIN);
  electrodeLiftServo.attach(ELECTRODE_SERVO_PIN);
  pinMode(STRIP_MOVEMENT_LIMIT_PIN, INPUT_PULLUP); // LOW reading means switch is down
  pinMode(ELECTRODE_LIFT_LIMIT_PIN, INPUT_PULLUP); // LOW reading means switch is down
  pinMode(LENS_MOVEMENT_LIMIT_PIN, INPUT_PULLUP); // LOW reading means switch is down
  pinMode(LENS_LED_PIN, OUTPUT);
  pinMode(BOTTOM_LED_PIN, OUTPUT);
  pinMode(SIDE_LED_PIN, OUTPUT);
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  loadingCycle();
  electrodeServoConf();
}

void loop() { 
  updateInputs();
  if (start && !canceled()){
    stimCycle();
  }
  electrodeLiftServo.detach();
  stripMotor.detach();
} 
