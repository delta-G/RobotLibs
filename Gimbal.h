/*

Gimbal.h
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

#ifndef GIMBAL_H_
#define GIMBAL_H_

#include "Arduino.h"
#include "Joint.h"

class GimbalClass {

private:

	Joint* panJoint;
	Joint* tiltJoint;

	uint16_t centerPan = 0;
	uint16_t centerTilt = 0;

public:

	GimbalClass();
	GimbalClass(Joint*, Joint*);

	void init();
	void detach();

	void run();
	void stop();

	float getPanAngle();
	float getTiltAngle();

	Joint* getPanJoint();
	Joint* getTiltJoint();

	boolean isMoving();

	uint16_t getPan();
	uint16_t getTilt();
	void setPan(uint16_t);
	void setTilt(uint16_t);
	void setPanAngle(float);
	void setTiltAngle(float);

	void setPanSpeed(uint16_t);
	void setTiltSpeed(uint16_t);

	void gotoCenter();

	uint16_t getCenterPan();
	uint16_t getCenterTilt();
	void setCenter();
	void setCenter(uint16_t aPan, uint16_t aTilt);

	int saveCalibrations(int);
	int loadCalibrations(int);

};






#endif /* GIMBAL_H_ */

