#include "Stepper.h"

Stepper stepper;

byte pin_step = 26;
byte pin_dir = 28;
byte pin_en = 24;

void setup() {
  //setup stepper:
  stepper = Stepper();
  stepper.init(pin_step, pin_dir, pin_en);
  stepper.setStepsPerRotation(200);
  stepper.setDiameter(60); //mm
  //stepper.setDiameter(8.0/PI); //For Thread: T8; Pitch:2mm; 4 Threads; 2*4=8mm pitch
  stepper.setDir(1);
  stepper.setFeedrate(0.05f);
  stepper.setAccelration(0.025f);
  stepper.setStopFeedrate(0.025f);
  //setup of stepper until here

  stepper.setPosition(300); //300mm to drive
  while(stepper.halfStep());
}

void loop() {
  // put your main code here, to run repeatedly:

}
