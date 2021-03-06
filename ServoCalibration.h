/*

ServoCalibration.h   Some helper functions for turning servo positions into angles.  
     Copyright (C) 2019  David C.

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

#ifndef SERVOCALIBRATION_H_
#define SERVOCALIBRATION_H_

#include "Arduino.h"

#include <EepromFuncs.h>


struct ServoCalibrationStruct {

	float minimumAngle;
	float maximumAngle;
	uint16_t minimumMicros;
	uint16_t maximumMicros;

	uint16_t angleToMicros(float aAngle);
	float microsToAngle(uint16_t aMicros);
	uint16_t constrainMicros(uint16_t aMicros);
	float constrainAngle(float aAngle);

	int saveCalibration(int);
	int readCalibration(int);

	void calibrate(uint16_t aMinMicros, float aMinAngle, uint16_t aMaxMicros, float aMaxAngle);

};



#endif /* SERVOCALIBRATION_H_ */

