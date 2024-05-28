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
const int LENS_LED_PIN = 11;
/**
 * @brief The pin number for the bottom LED.
 *
 * This constant represents the pin number used to control the lens LED.
 * It is set to 12, and the `analogWrite()` function can be used to control the LED brightness.
 */
const int BOTTOM_LED_PIN = 12;
/**
 * @brief The pin number for the side LED.
 *
 * This constant represents the pin number used to control the lens LED.
 * It is set to 13, and the `analogWrite()` function can be used to control the LED brightness.
 */
const int SIDE_LED_PIN = 13;

const int STRIP_MOVEMENT_LIMIT_PIN = 47;
const int ELECTRODE_LIFT_LIMIT_PIN = 49;
const int LENS_MOVEMENT_LIMIT_PIN = 51;

const int WELL_DIST; // We don't know what this is yet, but should be whatever unit moves the servo by the distance of one well

const int DELAY_DURATATION = 1000; // 1 second, but might need to be longer. Ask Mr. B

const int MAX31865_pin_0 = 10;
const int MAX31865_pin_1 = 11;
const int MAX31865_pin_2 = 12;
const int MAX31865_pin_3 = 13;
const int MAX31865_RREF = 430.0; //The value of the Rref resistor
const int MAX31865_RNOMINAL = 100;// The 'nominal' 0-degrees-C resistance of the sensor 100.0 for PT100, 1000.0 for PT1000
#endif