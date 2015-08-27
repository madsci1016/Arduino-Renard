/******************************************************************
*  Renard Communications Arduino Library 
*		details and example sketch: 
*			tbd
*
*		Brought to you by the makers of EasyTransfer:
*              Bill Porter
*              www.billporter.info
*
*		See Readme for other info and version history
*	
*  
*This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>
*
*This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License. 
*To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or
*send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
******************************************************************/

#ifndef Arduino-Renard_h
#define Arduino-Renard_h


#if ARDUINO > 22
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "HardwareSerial.h"

//some defines for human readability
#define WAITING 0
#define ADDRESS 1
#define STRIPPING 2
#define ESCAPE 3
#define FORWARDING 4

class Renard {
	public:

		//function to start library
		//begin(pointer to buffer, number of channels, serial port object)
		void begin(uint8_t *, uint8_t, HardwareSerial *theSerial, uint32_t);

		//function the services the serial port and parses the Renard data
		//returns true if a full packet of data has come in
		boolean receive();

	private:
		HardwareSerial *_serial;

		uint8_t * address;  //address of buffer
		uint8_t size;       //size of buffer
		uint8_t rx_state;   //state machine mode
		uint8_t rx_channel; //number of channels received

};

class RenardTX {
	public:
		void begin(uint16_t, HardwareSerial *theSerial, uint32_t);  //declare size and serial port
		void startPacket();  							  //allocate memory
		void setValue(uint16_t, uint8_t);                 //write to memory, applying escapes
		void sendPacket(); 								  //Output data, clean up memory
	
	private:
		HardwareSerial *_serial;
		uint16_t _size;
		uint8_t * _ptr;
		
};

#endif