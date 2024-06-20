#include <RotaryEncoder.h>
#ifndef CONSTANTS_H
#define CONSTANTS_H

const int STRIP_SERVO_PIN = 9;
const int ELECTRODE_SERVO_PIN = 10;

/**
 * @brief The pin number for the lens LED.
 *
 * This constant represents the pin number used to control the lens LED.
 * It is set to 11, and the `analogWrite()` function can be used to control the LED brightness.
 */
const int ELECTRODE_LED_PIN = 11; //write it to max value
const int ELECTRODE_LED_POWER = 255;
/**
 * @brief The pin number for the bottom LED.
 *time
 * This constant represents the pin number used to control the lens LED.
 * It is set to 12, and the `analogWrite()` function can be used to control the LED brightness.
 */
const int BOTTOM_LED_PIN = 12; // cap out at 20
const int BOTTOM_LED_POWER = 20;
/**
 * @brief The pin number for the side LED.
 *
 * This constant represents the pin number used to control the lens LED.
 * It is set to 13, and the `analogWrite()` function can be used to control the LED brightness.
 */
const int SIDE_LED_PIN = 13; // cap out at 70 or 75
const int SIDE_LED_POWER = 72;

const int SERVO_FORWARD = 116; // Speed that moves servo forward, if servo is reversed might need to be switched with @SERVO_BACKWARD
const int SERVO_BACKWARD = 56; // Speed that moves servo backward, if servo is reversed might need to be switched with @SERVO_FORWARD
const int SERVO_STOP = 86; // Speed that stops the servo-- shouldn't be changed
const int STRIP_MOVEMENT_LIMIT_PIN = 46;
const int ELECTRODE_LIFT_LIMIT_PIN = 48;
const int LENS_MOVEMENT_LIMIT_PIN = 50;

const int WELL_DIST = -1; // We don't know what this is yet, but should be whatever unit moves the servo by the distance of one well

const int DELAY_DURATATION = 1000; // 1 second, but might need to be longer. Ask Mr. B

const int STRIP_IMG_POS = 0; //6470 // We don't know what this is yet, but should be the amount of time it takes the servo to move to the strip image position from the 0pos in ms-- estimated to be 6470
const int EJECT_POS = 0; // Unknown, the amount of ms it takes for the strip to move from the 0 pos to the eject pos in ms.

const int ELECTRODE_STIM_POS = 0; // Unknown, should be an absoulte amount of degrees that represents the electrode stimulation position

const int MAX31865_pin_0 = 10;
const int MAX31865_pin_1 = 11;
const int MAX31865_pin_2 = 12;
const int MAX31865_pin_3 = 13;
const int MAX31865_RREF = 430.0; //The value of the Rref resistor
const int MAX31865_RNOMINAL = 100;// The 'nominal' 0-degrees-C resistance of the sensor 100.0 for PT100, 1000.0 for PT1000
const int JOYSTICK_BUTTON_PIN = 39;
const int JOYSTICK_X_PIN = A8;
const int JOYSTICK_Y_PIN = A9;

const int ROTARY_ENCODER_PIN1 = 41;
const int ROTARY_ENCODER_PIN2 = 43;
const RotaryEncoder::LatchMode ROTARY_ENCODER_LATCH_MODE = RotaryEncoder::LatchMode::TWO03;

const int SHORT_PRESS_DURATION = 1000; // 1 second, might be updated by user
const int LONG_PRESS_DURATION = 3000; // 3 seconds, might be updated by user

const int CAMERA_POS = 20; // The number of steps forward from the limit switch the camera should be
const int CAMERA_STEPS = 100; // The number of steps per cycle the camera should move.
#endif