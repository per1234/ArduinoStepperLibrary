#include "Stepper.h"

Stepper stepper;

byte pin_step = 26; //Define Step pin
byte pin_dir = 28; //Define Direction pin
byte pin_en = 24; //Define Enable pin

void setup() {
  //setup stepper:
  stepper = Stepper(); //Create stepper
  stepper.init(pin_step, pin_dir, pin_en); //Init stepper
  stepper.setStepsPerRotation(200); //Set steps per rotation
  stepper.setDiameter(60); //mm
  //stepper.setDiameter(8.0/PI); //For Thread: T8; Pitch:2mm; 4 Threads; 2*4=8mm pitch
  stepper.setDir(1); //Set direction
  stepper.setFeedrate(0.05f); //Set feedrate in m/s
  stepper.setAccelration(0.025f); //Set accelration in m/(s*s)
  stepper.setStopFeedrate(0.025f); //Set negative accelration in m/(s*s)
  stepper.setEnabled(true); //Enable stepper for control
  stepper.setAllowed(true); //If allowed is false, but enabled true, the stepper will hold on position
  //setup of stepper until here

  stepper.setPosition(300); //300mm to drive
  while(stepper.halfStep()); //Run Stepper as long as position is driven
}

void loop() {
  // put your main code here, to run repeatedly:

}
