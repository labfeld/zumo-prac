/*
 * Zumo Demo
 *
 * Based on Polulu line-following example
 */

#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>

#define condition true

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

int lastError = 0;

unsigned int sensors[6];

int position = 0;
