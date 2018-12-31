/*
  Stepper.h - Library for easy stepper control with a4988 driver.
  Created by Sebastian G. (sebiscodes@gmail.com) Sulgen, October 13, 2018.
*/
#ifndef Stepper_h
#define Stepper_h

#include "Arduino.h"

class Stepper
{
  public:
  Stepper();

  void init(byte stepPin, byte dirPin, byte enablePin);

  void setDir(byte direction); // 0/1
  void setEnabled(boolean enabled);
  void setAllowed(boolean enabled);
  void setStepState(boolean state);
  void setFeedrate(float f); // m/s
  void setStopFeedrate(float f); // m/s
  void setDiameter(float d); // mm
  void setStepsPerRotation(int spr); //steps per rotation
  void setAccelration(float acc); //m/(s*s)
  void setPosition(float pos);

  boolean halfStep();

  boolean getEnabled();
  boolean getOnPosition();
  double getPosition();
  byte getDir();

  void switchStep();
  
  private: 
  
  void executeHalfStep();
  
  byte pin_step;
  byte pin_dir;
  byte pin_enable;

  boolean stepState;
  byte dir;
  boolean en;

  float maxFeedrate = 300; //mm/s
  float stopFeedrate = 10; //mm/s
  float diameter = 60; //mm
  float accelration = 3000; //mm/(s*s)
  int stepsPerRotation = 200; //real spr*2
  double mmPerStep = diameter*PI/stepsPerRotation; // mm
  boolean drivingStarted = false;
  float distanceToDrive = 0; //mm
  float drivenDistance = 0; //mm

  float stopAccelrationDistance = 0; //mm
  float startBrakingDistance = 0; //mm
  float brakingDistance = 0; //mm
  
  long drivingStartedAt = 0; // us
  long nextStepAt = 0;

  void calculate();
};
#endif
