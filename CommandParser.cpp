/*

CommandParser  --  allows for easy setup of single letter text commands to
                   call functions at runtime. 
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

#include "CommandParser.h"

void CommandParser::parseCommandString(char *aCommand) {
	char *c = aCommand;
	if(hasStartMarker){
		c++;
	}
	for (int i = 0; i < numCommands; i++) {
		if (commands[i].match(c)) {
			commands[i].run(aCommand);
			return;
		}
	}

}



