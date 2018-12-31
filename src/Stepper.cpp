#include "Arduino.h"
#include "Stepper.h"

Stepper::Stepper(){};

//setup stepper
void Stepper::init(byte stepPin, byte dirPin, byte enablePin) {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(enablePin, HIGH);

  pin_step = stepPin;
  pin_dir = dirPin;
  pin_enable = enablePin;

  stepState = false;
  dir = 0;
  en = false;
  al = true;

  maxFeedrate = 300;
  accelration = 3000;
  diameter = 60;

  stepsPerRotation = 200;
  
  calculate();
  }

//recalculate some variables
void Stepper::calculate() {
	
	if (distanceToDrive > 0 && !drivingStarted) {
		stopAccelrationDistance = maxFeedrate/accelration*maxFeedrate/2;
		brakingDistance = (pow(maxFeedrate,2)-pow(stopFeedrate,2))/(2*accelration);
		double accelrationTillStopFeedrateDistance = pow(stopFeedrate,2)/(2*accelration);
		if (accelrationTillStopFeedrateDistance > distanceToDrive) {
			startBrakingDistance = distanceToDrive;
		} if (brakingDistance+stopAccelrationDistance > distanceToDrive) {
			stopAccelrationDistance = (distanceToDrive-accelrationTillStopFeedrateDistance)/2.0;
			startBrakingDistance = stopAccelrationDistance;
		} else {
			startBrakingDistance = distanceToDrive-brakingDistance;
		}
	}
}

//set direction
void Stepper::setDir(byte direction) {
    if (direction >= 1)
      direction = 1;
    else 
      direction = 0;
    dir = direction;
    digitalWrite(pin_dir, dir);
  }
  
//set enabled-state
void Stepper::setEnabled(boolean enabled) {
    en = enabled;
    digitalWrite(pin_enable, !en);
  }
//set enabled-state
void Stepper::setAllowed(boolean allowed) {
    al = allowed;
  }

//set step-state
void Stepper::setStepState(boolean state) {
  stepState = state;
    digitalWrite(pin_step, stepState);
  }

//set feedrate in m/s and convert to mm/s
void Stepper::setFeedrate(float f) {
  maxFeedrate = f*1000.0;
  calculate();
  }
void Stepper::setStopFeedrate(float f) {
  stopFeedrate = f*1000.0;
  calculate();
  }

//set diameter of wheel; can be dynamic
void Stepper::setDiameter(float d) {
  diameter = d;
  mmPerStep = diameter*PI/stepsPerRotation;
  }

//set steps per rotation
void Stepper::setStepsPerRotation(int spr) {
  stepsPerRotation = spr*2;
  mmPerStep = diameter*PI/(double)stepsPerRotation;
  }
  
//set accelration in m/(s*s) and convert to mm/(s*s)
void Stepper::setAccelration(float acc) {
  accelration = acc*1000.0;
  calculate();
  }

//set distance to drive
void Stepper::setPosition(float pos) {
  distanceToDrive = pos;
  drivenDistance = 0;
  drivingStarted = false;
  calculate();
  }
  
  
  long t2ForCalculation; // time till reach braking; only used for halfStep() function
  float brakingDistanceForCalculation; // distance when braking started; only used for halfStep() function
  float powMaxFeedrateForCalculation; // pow maxFeedrate; only used for halfStep() function
  long nextStepAtCalculation;
  
//set half step (switch from 0 to 1 or from 1 to 0)
boolean Stepper::halfStep() {
	if (drivenDistance < distanceToDrive) {
		if (!drivingStarted) {
			if (distanceToDrive > 0) {
				drivingStarted = true;
				drivingStartedAt = micros();
				drivenDistance = 0;
				nextStepAt = sqrt(2.0f*drivenDistance/accelration);
			  
				t2ForCalculation = (startBrakingDistance+stopAccelrationDistance)/maxFeedrate; // time till reach braking; only used for halfStep() function
				powMaxFeedrateForCalculation = pow(maxFeedrate,2); // pow maxFeedrate; only used for halfStep() function
				nextStepAtCalculation = drivingStarted+nextStepAt;
			}
			executeHalfStep();
		} else {
			if (micros() >= drivingStartedAt+nextStepAt) {
					executeHalfStep();
				if (drivenDistance < stopAccelrationDistance) { // accelrate
					nextStepAt = sqrt(2.0f*drivenDistance/accelration)*1000000.0;
					//nextStepAt = sqrt(2*drivenDistance/accelration)*1000000.0;
				} else if (drivenDistance < startBrakingDistance) { // drive
					nextStepAt = ((drivenDistance+stopAccelrationDistance)/maxFeedrate)*1000000.0;
					//nextStepAt = ((drivenDistance-stopAccelrationDistance+stopAccelrationDistance*2)/maxFeedrate)*1000000.0;
				} else {	//braking
					nextStepAt = (t2ForCalculation+((maxFeedrate-sqrt(powMaxFeedrateForCalculation-accelration*2.0f*(drivenDistance-startBrakingDistance)))/accelration))*1000000.0;
				}

			}
	  }
	} else {
		return false;
	}
  return true;
}

void Stepper::switchStep() {
	if (en && al) {
		stepState = !stepState;
		digitalWrite(pin_step, stepState);
	}
}

void Stepper::executeHalfStep() {
	stepState = !stepState;
    digitalWrite(pin_step, stepState);
	drivenDistance += mmPerStep;
}

//get enabled-state for stepper
boolean Stepper::getEnabled() {
    return en;
  }
//get enabled-state for stepper
boolean Stepper::getAllowed() {
    return al;
  }
  
//get position-state
boolean Stepper::getOnPosition() {
    return (distanceToDrive <= drivenDistance);
  }
  
//get distanceToDrive
double Stepper::getPosition() {
    return drivenDistance;
  }
  
//get direction
byte Stepper::getDir() {
    return dir;
  }

  
