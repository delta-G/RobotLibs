/*

 RadioCommon  --  runs on Arduino Nano and acts as a serial to LoRa bridge
 for connecting to my robot  The common parts of RobotRadioLora and
 BaseRadioLora
 Copyright (C) 2020  David C.

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


#include "RadioCommon.h"

extern RH_RF95 radio;

extern void handleRawRadio(uint8_t *p);
extern void handleRadioCommand(char *p);


uint8_t holdingBuffer[HOLDING_BUFFER_SIZE];
uint8_t holdingSize = 0;

uint32_t lastFlushTime;
uint32_t maxFlushInterval = 10000;

uint8_t resetPin = -1;

void initRadio(uint8_t aResetPin){
	resetPin = aResetPin;
	pinMode(resetPin, OUTPUT);
	digitalWrite(resetPin, HIGH);
}

void listenToRadio() {
	if (radio.available()) {

		static uint8_t buf[MAX_MESSAGE_SIZE_RH] = { 0 };
		memset(buf, 0, MAX_MESSAGE_SIZE_RH);
		uint8_t len = MAX_MESSAGE_SIZE_RH;

		if (radio.recv(buf, &len)) {
			processRadioBuffer(buf, len);
		}
	}
}

void handleOutput(){
	if (holdingSize == 0) {
		lastFlushTime = millis(); // don't start timer if we don't have anything to send.
	}
	if (millis() - lastFlushTime >= maxFlushInterval) {
		flush();
	}
}

void processRadioBuffer(uint8_t *aBuf, uint8_t aLen) {

	static boolean receiving = false;
	static boolean receivingRaw = false;
	static char commandBuffer[64];
	static int index;

//	flushOnNextRaw = true;
	uint8_t len = aLen;
	if (len > MAX_MESSAGE_SIZE_RH) {
		len = MAX_MESSAGE_SIZE_RH;
	}

	// radio.racv doesn't put any null terminator, so we can't use
	// string functions, have to scroll through and pick stuff out.
	for (int i = 0; i < len; i++) {
		char c = aBuf[i];

		if (c == START_OF_PACKET && !receivingRaw){
			receiving = true;
			index = 0;
			commandBuffer[0] = 0;
		}
		if (receiving) {
			commandBuffer[index++] = c;
			if(receivingRaw){
				if(index == (commandBuffer[2])){
					// index 2 is the length of the raw command
					// so we've received a whole raw command
					handleRawRadio((byte*)commandBuffer);
					receivingRaw = false;
					receiving = false;
				}
				continue; // skip the rest of the for loop, it's for ascii commands.
			}
			if((index == 2)&&((commandBuffer[1]>=0x11) && (commandBuffer[1]<=0x14))){
				receivingRaw = true;
			}
			commandBuffer[index] = 0;
			if (index >= HOLDING_BUFFER_SIZE) {
				index--;
			}
			if (c == END_OF_PACKET) {
				receiving = false;
				handleRadioCommand(commandBuffer);
			}
		}
	}
}


void addToHolding(uint8_t *p, uint8_t aSize) {
	if (HOLDING_BUFFER_SIZE - holdingSize <= aSize) {
		//  Not enough room so clear the buffer now
		flush();
	}
	memcpy(holdingBuffer + holdingSize, p, aSize);
	holdingSize += aSize;
}

void addToHolding(char *p) {
	addToHolding((uint8_t*) p, strlen(p));
}

void sendToRadio(char *p) {
	sendToRadio((uint8_t*) p, strlen(p));
}

void sendToRadio(uint8_t *p, uint8_t aSize) {
	radio.send(p, aSize);
	radio.waitPacketSent();
}

void flush() {
	sendToRadio(holdingBuffer, holdingSize);
	holdingSize = 0;
	lastFlushTime = millis();
}


void handleConfigString(char *p) {
	switch (p[2]) {
	case 'I': {
		uint32_t val = strtoul(p+3, NULL, 10);
		maxFlushInterval = val;
		break;
	}
	case 'M': {
		switch (p[3]) {
		case '0':
			radio.setModemConfig(RH_RF95::Bw125Cr45Sf128);
			break;
		case '1':
			radio.setModemConfig(RH_RF95::Bw500Cr45Sf128);
			break;
		case '2':
			radio.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);
			break;
		case '3':
			radio.setModemConfig(RH_RF95::Bw125Cr48Sf4096);
			break;
		default:
			radio.setModemConfig(RH_RF95::Bw125Cr45Sf128);
			break;

		}
		break;
	}
	case 'B': {
		// set bandwidth to  option: 7800,10400,15600,20800,31250,41700,62500,125000,250000,500000
		//  sketchy if below 62500 although I hear 31250 works ok sometimes
		//  higher bandwidth is faster and more efficient but shorter range and more congestion
		//  Each doubling of bandwidth is 3dB reduction in link budget
		uint32_t entry = atoi((const char*) (p + 3));
		radio.setSignalBandwidth(entry);
		break;
	}
	case 'S': {
		// set spreading factor 6 - 12
		// Each step up doubles time per character
		// and gives 2.5dB of extra link budget
		uint8_t entry = atoi((const char*) (p + 3));
		if (entry < 6) {
			entry = 6;
		} else if (entry > 12) {
			entry = 12;
		}
		radio.setSpreadingFactor(entry);
		break;
	}
	case 'C': {
		// set coding Rate Denominator 5 - 8
		// larger is slower but more reliable.
		uint8_t entry = atoi((const char*) (p + 3));
		if (entry < 5) {
			entry = 5;
		} else if (entry > 8) {
			entry = 8;
		}
		radio.setCodingRate4(entry);
		break;
	}
	case 'R': {
		//reset the radio
		resetRadio();
		break;
	}
	default:
		break;
	} // end switch
}



void resetRadio() {

	// manual reset
	digitalWrite(resetPin, LOW);
	delay(10);
	digitalWrite(resetPin, HIGH);
	delay(10);

	while (!radio.init()) {
//		DEBUG("LoRa radio init failed");
		while (1)
			;
	}
//	DEBUG("LoRa radio init OK!");

	// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
	if (!radio.setFrequency(RF95_FREQ)) {
//		DEBUG("setFrequency failed");
		while (1)
			;
	}
//	DEBUG("Set Freq to: ");
//	DEBUG(RF95_FREQ);

	radio.setTxPower(23, false);

}









////  END

