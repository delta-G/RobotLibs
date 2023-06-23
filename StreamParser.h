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

#ifndef STREAMPARSER_H_
#define STREAMPARSER_H_

#include "Arduino.h"

#ifndef STREAMPARSER_BUFFER_SIZE
#define STREAMPARSER_BUFFER_SIZE 64
#endif


class StreamParser {

private:
	char _SPbuffer[STREAMPARSER_BUFFER_SIZE];
	int index;

	Stream* in;
	char sop;
	char eop;

	void (*callback)(char*);
	void (*rawCallback)(char*);

	boolean receiving = false;
	boolean receivingRaw = false;

	boolean greedy = false;


public:

	StreamParser(Stream* aIn, char aSop, char aEop, void(*aCallback)(char*)):index(0), in(aIn), sop(aSop), eop(aEop), callback(aCallback), rawCallback(aCallback){};
	StreamParser(Stream* aIn, void(*aCallback)(char*)):index(0), in(aIn), sop('<'), eop('>'), callback(aCallback), rawCallback(aCallback){};
	void run();
	void handleChar(char c);

	void handleRawData(char C);

	void setCallback(void (*aCall)(char*));
	void setRawCallback(void (*aCall)(char*));

	void setGreedy(bool);
	bool getGreedy();

};



#endif /* STREAMPARSER_H_ */
