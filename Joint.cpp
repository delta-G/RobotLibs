/*

Joint.cpp
     Copyright (C) 2017  David C.

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

     */

#include "Joint.h"

Joint::Joint(uint8_t aPin, uint16_t aPos) {
	pin = aPin;
	position = aPos;
	target = aPos;
	speed = 100;

	length = 100;
	offset = 0;

	//TODO:
	// moving can be calculated from target and speed
	// no real need for a whole extra variable to have to keep track of.
	moving = false;
	calibration.calibrate(544, 0.0, 2400, 180.0);
	lastStickUpdate = millis();
	lastRunTime = millis();

	max_refresh_rate = 100;
}

Joint::Joint(uint8_t aPin, uint16_t aPos, uint16_t aLength, uint16_t aMinMicros, float aMinAngle, uint16_t aMaxMicros, float aMaxAngle) {
	pin = aPin;
	position = aPos;
	target = aPos;
	length = aLength;
	offset = 0;
	speed = 100;
	moving = false;
	lastStickUpdate = millis();
	lastRunTime = millis();
	calibration.calibrate(aMinMicros, aMinAngle, aMaxMicros, aMaxAngle);

	max_refresh_rate = 100;
}

Joint::Joint(uint8_t aPin, uint16_t aPos, uint16_t aLength, uint16_t aOffset, uint16_t aMinMicros, float aMinAngle, uint16_t aMaxMicros, float aMaxAngle) {
	pin = aPin;
	position = aPos;
	target = aPos;
	length = aLength;
	offset = aOffset;
	speed = 100;
	moving = false;
	lastStickUpdate = millis();
	lastRunTime = millis();
	calibration.calibrate(aMinMicros, aMinAngle, aMaxMicros, aMaxAngle);

	max_refresh_rate = 100;
}

void Joint::init(){
	attach(pin);
	moveToImmediate(position);
}

boolean Joint::isMoving(){
	return (position != target);
}

void Joint::moveToImmediate(uint16_t aPos) {
	aPos = calibration.constrainMicros(aPos);
	write(aPos);
	position = aPos;
	target = position;
}

void Joint::moveToImmediateAngle(float aAng) {
	aAng = calibration.constrainAngle(aAng);
	position = calibration.angleToMicros(aAng);
	write(position);
	target = position;
}

uint8_t Joint::getPin(){
	return pin;
}

uint16_t Joint::getPosition(){
	return position;
}

float Joint::getAngle() {
	return calibration.microsToAngle(getPosition());
}

uint16_t Joint::getLength(){
	return length;
}

void Joint::setLength(uint16_t aLength){
	length = aLength;
}

uint16_t Joint::getOffset(){
	return offset;
}

void Joint::setOffset(uint16_t aOffset){
	offset = aOffset;
}

uint16_t Joint::setTarget(uint16_t aTarget){
	aTarget = calibration.constrainMicros(aTarget);
	target = aTarget;
	return target;
}

uint16_t Joint::getTarget(){
	return target;
}

float Joint::setTargetAngle(float aAngle){
	aAngle = calibration.constrainAngle(aAngle);
	target = calibration.angleToMicros(aAngle);
	return calibration.microsToAngle(target); // just in case setting it changes things.
}

uint16_t Joint::setTarget(uint16_t aTarget, uint16_t aSpeed){
	aTarget = calibration.constrainMicros(aTarget);
	target = aTarget;
	setSpeed(aSpeed);
	return target;
}

float Joint::setTargetAngle(float aAngle, uint16_t aSpeed){
	aAngle = calibration.constrainAngle(aAngle);
	target = calibration.angleToMicros(aAngle);
	setSpeed(aSpeed);
	return calibration.microsToAngle(target); // just in case setting it changes things.
}

void Joint::setSpeed(uint16_t aSpeed){
	speed = aSpeed;
}

uint16_t Joint::getSpeed(){
	return speed;
}

void Joint::stop() {
	target = position;
	moving = false;
}

boolean Joint::run() {

	unsigned long cur = millis();
	unsigned long deltaTime = cur - lastRunTime;

	if (position != target) {

		if(!moving){
			lastRunTime = cur; // reset our timer for the new move
			moving = true;
			return false;  // bail out until next run
		}

		//  Speed in us pulse time per s of real time
		//  What would be the max?
		unsigned long deltaPulse = deltaTime * speed / 1000;

		if (deltaPulse > 0) {
			if (target < position) {
				position -= deltaPulse;
				if(position < target){
					position = target;
				}
			} else {
				position += deltaPulse;
				if(position > target){
					position = target;
				}
			}
			position = calibration.constrainMicros(position);
			lastRunTime = cur;
			if(position == target){
				moving = false;
			}
		}

	}
	write(position);

	return (position == target);
}

ServoCalibrationStruct Joint::getCalibrationStruct(){
	ServoCalibrationStruct retval = calibration;
	return retval;
}

int Joint::saveCalibration(int aAddress){
	return calibration.saveCalibration(aAddress);
}

int Joint::loadCalibration(int aAddress){
	return calibration.readCalibration(aAddress);
}


//  Saves 3 ints for a total of 6 bytes.
int Joint::saveState(int aAddress){

	int add = 0;
	add += writeToEEPROM(aAddress + add, position);
	add += writeToEEPROM(aAddress + add, target);
	add += writeToEEPROM(aAddress + add, speed);

	return add;
}


int Joint::recallState(int aAddress){

	int add = 0;
	add += readFromEEPROM(aAddress + add, position);
	add += readFromEEPROM(aAddress + add, target);
	add += readFromEEPROM(aAddress + add, speed);

	return add;

}




void Joint::followTheStick(int aReading, int aCenterMicros) {
	if (aCenterMicros == 0) {
		setTarget(map(aReading, -32768, 32767, calibration.minimumMicros, calibration.maximumMicros));
	} else {
		if(aReading < 0){
			setTarget(map(aReading, -32768, 0, calibration.minimumMicros, aCenterMicros));
		} else {
			setTarget(map(aReading, 0, 32767, aCenterMicros, calibration.maximumMicros));
		}
	}
}


void Joint::useStick(int aReading){

	unsigned long cm = millis();

	//The delta time should be last time since a
	//move or a zero reading from the stick.

	if(aReading == 0){
		lastStickUpdate = cm;
		return;
	}

	float timeScale = (cm - lastStickUpdate) / 1000.0;  // speed is in microsecond steps per second so this needs to be in seconds
	float speedRatio = (float)aReading / 32767.0;
	int16_t step = speed * speedRatio * timeScale;

	//  Instead of moveToImmediate, we could try another version with
	//  setting the target
	if((step >= 1)||(step <= -1)){
//		moveToImmediate(position + step);
		// TODO:
		// This will still move at full speed.  We need to make this ratio
		// number persistent.
		setTarget(position + step);
		lastStickUpdate = cm;
	}
}


//  aMultiplier is the number of milliseconds forward
//  at the current speed to advance the motor
//  if you make this just slightly longer than the
//  rate at which you're calling this function it
//  will move very smoothly and stop when you let off.
void Joint::advance(int aMultiplier){

	// this is sharing a previous millis variable
	// lastStickUpdate with useStick.

	unsigned long cm = millis();

//	float timeScale = (cm - lastStickUpdate) / 1000.0;
	float timeScale = (aMultiplier / 1000.0);
	int16_t step = speed * timeScale;

	if((step >= 1)||(step <= -1)){
		//unlike useStick, here we just take a single step
		// sign on multiplier sets direction
		setTarget(position + step);
		lastStickUpdate = cm;
	}
}


/*********




For the purpose of kinematics, let's construct a line drawing
starting at the base servo and going up through the middle of
all the arm pieces.  If we want to fill things out later we can.
But for now all arm pieces are modeled as a line.

For the purposes of this kinematics we will set our coordinate frame
such that 0,0,0 is the hub of the base servo.  Positive Y will be
towards the front of the vehicle.  Positive X will be to the right.
And positive Z will be up.  Angles will be measured so 90 degrees
(pi/2) is straight up and down.  Angles less than 90 will be towards
the front of the vehicle if the base is at it's mid-position with the
arm facing forwards.





**************/


//  Let's start with some utility funcs.

// Start by treating the arm as a 2D flat object and calculate the distance from base (X) and height (Y)
// then you can combine that with data from base servo later and calculate actual xyz.

//  Find end of hypotenuse from angle at 0,0

/*
XYpoint solveTriangle (float aAngle, uint16_t aLength){
	XYpoint retval = {0,0};
	retval.x = cos(aAngle) * aLength;
	retval.y = sin(aAngle) * aLength;
	return retval;
}


XYandAngle Joint::findEndXY(XYpoint aPivot, float aAngle){

	float endAngle = aAngle + calibration.microsToAngle(position) - 1.5708;

	XYpoint solution = solveTriangle(endAngle, length);

	XYandAngle retval;

	retval.x = solution.x + aPivot.x;
	retval.y = solution.y + aPivot.y;
	retval.approachAngle = endAngle;

	return retval;
}


XYandAngle Joint::findEndXY(XYandAngle aPivot){

	float endAngle = aPivot.approachAngle + calibration.microsToAngle(position) - 1.5708;

	XYpoint solution = solveTriangle(endAngle, length);

	if(offset != 0){
		XYpoint offsetSolution = solveTriangle(endAngle + 90, offset);
		solution.x += offsetSolution.x;
		solution.y += offsetSolution.y;
	}

	XYandAngle retval;

	retval.x = solution.x + aPivot.x;
	retval.y = solution.y + aPivot.y;
	retval.approachAngle = endAngle;

	return retval;
}




XYandAngle Joint::findEndXY(XYandAngle aPivot, float aPosition){

	float endAngle = aPivot.approachAngle + calibration.constrainAngle(aPosition) - 1.5708;

		XYpoint solution = solveTriangle(endAngle, length);

		if(offset != 0){
			XYpoint offsetSolution = solveTriangle(endAngle + 90, offset);
			solution.x += offsetSolution.x;
			solution.y += offsetSolution.y;
		}

		XYandAngle retval;

		retval.x = solution.x + aPivot.x;
		retval.y = solution.y + aPivot.y;
		retval.approachAngle = endAngle;

		return retval;

}

*/
