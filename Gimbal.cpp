/*

Gimbal.cpp
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

#include "Gimbal.h"



GimbalClass::GimbalClass(){
	panJoint = NULL;
	tiltJoint = NULL;
}

GimbalClass::GimbalClass(Joint* aPanJoint, Joint* aTiltJoint){
	panJoint = aPanJoint;
	tiltJoint = aTiltJoint;
	setCenter();
}

Joint* GimbalClass::getPanJoint(){
	return panJoint;
}

Joint* GimbalClass::getTiltJoint(){
	return tiltJoint;
}

void GimbalClass::init(){
	panJoint->init();
	delay(25);
	tiltJoint->init();
}

void GimbalClass::detach(){
	panJoint->detach();
	tiltJoint->detach();
}

void GimbalClass::run(){
	panJoint->run();
	tiltJoint->run();
}


void GimbalClass::stop(){
	panJoint->stop();
	tiltJoint->stop();
}


float GimbalClass::getPanAngle(){
	return panJoint->getAngle();
}

float GimbalClass::getTiltAngle(){
	return tiltJoint->getAngle();
}

boolean GimbalClass::isMoving(){
	return (panJoint->isMoving() || tiltJoint->isMoving());
}

int GimbalClass::saveCalibrations(int aAddress){

	int add = 0;
	add += panJoint->saveCalibration(aAddress);
	add += tiltJoint->saveCalibration(aAddress + add);
	return add;
}

int GimbalClass::loadCalibrations(int aAddress) {

	int add = 0;
	add += panJoint->loadCalibration(aAddress);
	add += tiltJoint->loadCalibration(aAddress + add);
	return add;
}


void GimbalClass::setPan(uint16_t aTargetMicros) {
	panJoint->setTarget(aTargetMicros);
}

void GimbalClass::setTilt(uint16_t aTargetMicros) {
	tiltJoint->setTarget(aTargetMicros);
}

void GimbalClass::setPanAngle(float aTargetAngle) {
	panJoint->setTargetAngle(aTargetAngle);
}

void GimbalClass::setTiltAngle(float aTargetAngle) {
	tiltJoint->setTargetAngle(aTargetAngle);
}

uint16_t GimbalClass::getPan(){
	return panJoint->getPosition();
}

uint16_t GimbalClass::getTilt(){
	return tiltJoint->getPosition();
}

void GimbalClass::setPanSpeed(uint16_t aPanSpeed){
	panJoint->setSpeed(aPanSpeed);
}

void GimbalClass::setTiltSpeed(uint16_t aTiltSpeed){
	tiltJoint->setSpeed(aTiltSpeed);
}

void GimbalClass::gotoCenter(){
	setPan(centerPan);
	setTilt(centerTilt);
}

uint16_t GimbalClass::getCenterPan() {
	return centerPan;
}
uint16_t GimbalClass::getCenterTilt() {
	return centerTilt;
}
void GimbalClass::setCenter() {
	centerPan = panJoint->getPosition();
	centerTilt = tiltJoint->getPosition();
}
void GimbalClass::setCenter(uint16_t aPan, uint16_t aTilt) {
	centerPan = aPan;
	centerTilt = aTilt;
}
