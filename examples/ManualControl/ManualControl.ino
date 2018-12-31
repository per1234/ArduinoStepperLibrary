#include "Stepper.h"

Stepper stepper;

byte pin_step = 26; //Define Step pin
byte pin_dir = 28; //Define Direction pin
byte pin_en = 24; //Define Enable pin

void setup() {
  //setup stepper:
  stepper = Stepper(); //Create stepper
  stepper.init(pin_step, pin_dir, pin_en); //Init stepper
  stepper.setEnabled(true); //Enable stepper for control
  stepper.setAllowed(true); //If allowed is false, but enabled true, the stepper will hold on position
  //setup of stepper until here
  for (int i = 0; i < 400;i++) { //make 200 steps
    stepper.switchStep(); //Switch step pin from high to low or from low to high
    delayMicroseconds(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
