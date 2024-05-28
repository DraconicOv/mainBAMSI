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
int strip0Pos = 0;
int stripMaxPos = 0;
int servoPos = 0;
int electrode0Pos = 0;
Adafruit_MAX31865 thermo = Adafruit_MAX31865(MAX31865_pin_0, MAX31865_pin_1, MAX31865_pin_2, MAX31865_pin_3); 

int userSetTemp = 0; // User will set temp with buttons/joystick, TODO

void loadingCycle(){
  // Move servo StripServo until limit switch StripLim is reached, reset counter Strip0Pos
  while(digitalRead(STRIP_MOVEMENT_LIMIT_PIN) == HIGH){ // Move forward until we limit switch is reached
    stripMaxPos++;
    stripMotor.write(stripMaxPos);
    delay(10);
  }
  strip0Pos = stripMaxPos;
  // Move servo StripServo backwards until imaging position StripImPos is reached
  while(digitalRead(LENS_MOVEMENT_LIMIT_PIN) == HIGH){ // Once limit is reached, we're at the 0 pos
    strip0Pos--;
    stripMotor.write(strip0Pos);
    delay(10);
  }
}

void unloadingCycle(){
  //Move servo StripServo backwards to Img0Pos
  int currentPos = stripMotor.read();
  while(digitalRead(LENS_MOVEMENT_LIMIT_PIN) == HIGH){
    currentPos--;
    stripMotor.write(currentPos);
    delay(10);
  }
}

void electrodeServoConf(){
  //move electrode servo backwards until limit switch ElektrLim is reached, reset counter Electr0Pos, then move servo into stimulation position
  int electrodePos = electrodeLiftServo.read();
  while (digitalRead(ELECTRODE_LIFT_LIMIT_PIN) == HIGH)
  {
    electrodePos--;
    electrodeLiftServo.write(electrodePos);
    delay(10);

  }
  electrode0Pos = electrodePos;
  
}
// Temporary function, determine how to determine the stimulation position and how to move electrode there.
void moveElectrodeToStim(){
  // ???? TODO: What is the stimulation position?
}
// TODO
void cameraTrigger(){
  digitalWrite(LENS_LED_PIN, HIGH);
  // More information required to determine how to trigger the camera 
  digitalWrite(LENS_LED_PIN, LOW);
}
//TODO
void electrodeTrigger(){
  // More information required to determine how to trigger the electrode
}
// TESTME
void moveElectrodeToRest(){
  for (int currentPos = electrodeLiftServo.read(); currentPos < electrode0Pos; currentPos--)
  {
    electrodeLiftServo.write(currentPos);
    delay(10);
  }
}
// TODO
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
//TODO: idk how to do this, or what the distance should be
void moveStripByWellDist(){

}

void setup() {
  stripMotor.attach(STRIP_SERVO_PIN);
  electrodeLiftServo.attach(ELECTRODE_SERVO_PIN);
  pinMode(STRIP_MOVEMENT_LIMIT_PIN, INPUT_PULLUP); // LOW reading means switch is down
  pinMode(ELECTRODE_LIFT_LIMIT_PIN, INPUT_PULLUP); // LOW reading means switch is down
  pinMode(LENS_MOVEMENT_LIMIT_PIN, INPUT_PULLUP); // LOW reading means switch is down
  pinMode(LENS_LED_PIN, OUTPUT);
  pinMode(BOTTOM_LED_PIN, OUTPUT);
  pinMode(SIDE_LED_PIN, OUTPUT);
  loadingCycle();
  electrodeServoConf();
}

void loop() {
  moveElectrodeToStim();
  // TODO: Determine which LEDs to turn on
  digitalWrite(SIDE_LED_PIN, HIGH);
  digitalWrite(BOTTOM_LED_PIN, HIGH);
  delay(DELAY_DURATATION);
  //Camera Trigger: TBD
  delay(DELAY_DURATATION);
  //Electrode Trigger: TBD
  electrodeTrigger();
  digitalWrite(SIDE_LED_PIN, LOW);
  digitalWrite(BOTTOM_LED_PIN, LOW);
  delay(DELAY_DURATATION);
  moveElectrodeToRest();
  moveStripByWellDist();
} 
