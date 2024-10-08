#include "zumo_state.h"

// setup a set of states here for the zumo to follow

/* -------- SET MOTOR VALUES---------- */

// These define statements allow us to set some useful values for later
// This type of construction is often used to make a system clearer to
// understand

// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)

#define SPEED_MAX 200
#define SPEED_HALT 0

/* -----------STATE ENCODING---------- */

// These defines define the state encoding - a mapping from a name to a numerical code
// Add new state definitions by copying the lines here and incrementing the number by one.
// Note: make sure each encoding is unique

enum class STATE {
  FORWARD,
  LEFT,
  RIGHT,
  HALT,
  FOLLOW_LINE,
  SHARP_RIGHT,
  SHARP_LEFT,
};

enum class TRACK_POS {
  END,
  LAST_BEND,
  AWAIT_LOCK_LAST_BEND,
  BLIND_GAP,
  CLR,
};

/* ---------END STATE ENCODING---------- */



// Create a variable to hold a state.  This should be set to the default state.
STATE state = STATE::FOLLOW_LINE;


TRACK_POS track_pos = TRACK_POS::CLR;

/* -----------STATE IMPLEMENTATION---------- */

// Each state should be implemented by a function.
// Each function should also report its state to the wireless serial

// Serial  - usb connected serial interface
// Serial0 - wireless connected serial interface

// Each state implements a set of actions.  In this case, we can set the motor
// speeds.

void forward()
{
  Serial.println("forward");
  Serial0.println("forward");
  motors.setSpeeds(SPEED_MAX, SPEED_MAX);
}

void turnLeft()
{
  Serial.println("left");
  Serial0.println("left");
  motors.setSpeeds(SPEED_HALT, SPEED_MAX);
}

void turnRight()
{
  Serial.println("right");
  Serial0.println("right");
  motors.setSpeeds(SPEED_MAX, SPEED_HALT);
}

void sharpLeft()
{
  Serial.println("sharpLeft");
  Serial0.println("sharpLeft");
  motors.setSpeeds(-SPEED_MAX, SPEED_MAX);
}

void sharpRight()
{
  Serial.println("sharpRight");
  Serial0.println("sharpRight");
  motors.setSpeeds(SPEED_MAX, -SPEED_MAX);
}

void halt()
{
  Serial.println("halt");
  Serial0.println("halt");
  motors.setSpeeds(SPEED_HALT, SPEED_HALT);
}

bool isOnLine() {
  unsigned int sensorReadings[6] = {0};
  reflectanceSensors.readCalibrated(sensorReadings);
  int nOnLine = 0;
  for (int i = 0; i < 6; i++) {
    Serial0.print("Sensor: ");
    Serial0.print(i);
    Serial0.print(" val: ");
    Serial0.println(sensorReadings[i]);
    if (sensorReadings[i] > 400) {
      nOnLine++;
    }
  }
  return nOnLine >= 1;
}

void followLine() {
  int position = reflectanceSensors.readLine(sensors);
  if (position > 2500) {
    turnRight();
  } else {
    turnLeft();
  }
}


/* -------END STATE IMPLEMENTATION---------- */

/* -----------STATE CHOICE---------- */

void selectState()
{
  static int strikes = 3;
  position = reflectanceSensors.readLine(sensors);
  switch (track_pos) {
    case TRACK_POS::END:
      Serial0.println("END");
      state = STATE::HALT;
      break;
    case TRACK_POS::BLIND_GAP:
      Serial0.println("BLIND_GAP");
      state = STATE::FORWARD;
      if (isOnLine()) {
        track_pos = TRACK_POS::AWAIT_LOCK_LAST_BEND;
      }
      break;
    case TRACK_POS::AWAIT_LOCK_LAST_BEND:
      Serial0.println("AWAIT_LOCK_LAST_BEND");
      state = STATE::SHARP_RIGHT;
      delay(100);
      if (isOnLine()) {
        track_pos = TRACK_POS::LAST_BEND;
      }
      break;
    case TRACK_POS::LAST_BEND:
      Serial0.println("LAST_BEND");
      state = STATE::FOLLOW_LINE;
      if (!isOnLine()) {
        strikes--;
      } else {
        strikes = 3;
      }
      if (strikes == 0) {
        track_pos = TRACK_POS::END;
      }
      break;
    case TRACK_POS::CLR: // Credit position
      Serial0.println("CLR");
      state = STATE::FOLLOW_LINE;
      if (!isOnLine()) {
        strikes--;
      } else {
        strikes = 3;
      }
      if (strikes == 0) {
        track_pos = TRACK_POS::BLIND_GAP;
      }
      break;
  }

  switch (state)
  {
    case STATE::FORWARD:
      forward();
      break;

    case STATE::LEFT:
      turnLeft();
      break;

    case STATE::RIGHT:
      turnRight();
      break;

    case STATE::SHARP_LEFT:
      sharpLeft();
      break;


    case STATE::SHARP_RIGHT:
      sharpRight();
      break;

    case STATE::HALT:
      halt();
      exit(0);
      break;

    case STATE::FOLLOW_LINE:
      followLine();
      break;

    default:
      halt();
  }
}

/* -----------END STATE CHOICE---------- */

#include "zumo_driver.h"
