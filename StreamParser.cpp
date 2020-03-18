/*

StreamParser   --  Parses any Stream object for data with end markers.  

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

#include "StreamParser.h"


void StreamParser::run() {

//	if (receivingRaw) {
//		handleRawData();
//	} else
	if (in->available()) {
		do {
			char c = in->read();
			handleChar(c);
		} while (in->available() && greedy);
	}
}

void StreamParser::handleChar(char c) {
	if (receivingRaw) {
		handleRawData(c);
	}

	if (c == sop) {
		receiving = true;
		index = 0;
		buffer[0] = 0;
	}
	if (receiving) {
		buffer[index] = c;
		buffer[++index] = 0;
		if ((index == 3) && (buffer[1] >= 0x11) && (buffer[1] <= 0x14)) {
			receivingRaw = true;
			receiving = false;
			return;
		}
		if (index >= STREAMPARSER_BUFFER_SIZE) {
			index--;
		}
		if (c == eop) {
			receiving = false;
			callback(buffer);
		}
	}
}

void StreamParser::handleRawData(char c) {

	// To get here we already have < and the code and the number of bytes in the buffer

	int numBytes = buffer[2];

	buffer[index] = c;
	buffer[++index] = 0;  // why not null terminate

	if (index >= numBytes) {
		receivingRaw = false;
		rawCallback(buffer);
	}

}

void StreamParser::setCallback(void (*aCall)(char*)){
	callback = aCall;
}

void StreamParser::setRawCallback(void (*aCall)(char*)){
	rawCallback = aCall;
}


void StreamParser::setGreedy(bool aBoo){
	greedy = aBoo;
}

bool StreamParser::getGreedy(){
	return greedy;
}

