#include "Arduino-Renard.h"

//Captures address and size of buffer
void Renard::begin(uint8_t * ptr, uint8_t length, HardwareSerial *theSerial, uint32_t baud){
	address = ptr;
	size = length;
	_serial = theSerial;
	rx_state = 0;
	rx_channel =0;
	
	_serial->begin(baud);
}

boolean Renard::receive(){

	//create a return flag
	uint8_t rtn = false;
	uint8_t ren_temp = 0;

	//for now let's block till we empty the serial buffer
	while(_serial->available() > 0){
	
		
		switch(rx_state){
			case WAITING: //look for sync
			
				if (_serial->read() == 0x7E){
					rx_state=ADDRESS;
					//send sync						
					_serial->write(0x7E);
					
				}			
			break;
			case ADDRESS: //look for my address
			
				ren_temp = _serial->read();
				
				if (ren_temp == 0x80){
					rx_state = STRIPPING;
					rx_channel = 0;
					
				}
				else if (ren_temp > 0x80){ //not my address, subtract 1 and forward rest
					rx_state = FORWARDING;							
					_serial->write(--ren_temp);
				}
			break;
			case STRIPPING: //peel off my data
			
				ren_temp = _serial->read();
			
				if (ren_temp == 0x7D) //pad byte, drop
					break;
				if (ren_temp == 0x7F)//escape byte
					rx_state = ESCAPE;
				else {
					//write brightness value into memory
					*(address+rx_channel++) = ren_temp;
					
					//check if last value	
					if(rx_channel == size){
						//set Serial state machine to forward rest
						rx_state = FORWARDING;							
						_serial->write(0x80);	
						rtn = true;
					}			
				}							
			break;
			case ESCAPE: //peel off escaped byte			
			
				*(address+rx_channel++) = _serial->read() + 0x4E;
				
				if(rx_channel == size){
					rx_state = FORWARDING;						
					_serial->write(0x80);
					rtn = true;
				}			
				else
					rx_state = STRIPPING;
			break;
			case FORWARDING: //forward rest
			
				ren_temp = _serial->read();
			
				if(ren_temp == 0x7E) 
					rx_state = ADDRESS;						
				_serial->write(ren_temp);
				
			break;
		}    
	}
	
	return rtn;
}

void RenardTX::begin(uint16_t length, HardwareSerial *theSerial, uint32_t baud){
	
	_size = length;
	_serial = theSerial;
	_serial->begin(baud);
}

void RenardTX::startPacket(){
	
	_ptr = (uint8_t*) malloc(_size+2);
	_ptr[0] = 0x7E;
	_ptr[1] = 0x80;
	
	
}

void RenardTX::setValue(uint16_t address, uint8_t value){
	
	switch (value){
		case 0x7d:
		_ptr[address+2] = 0x7c;
		break;
		case 0x7e:
		case 0x7f:
		_ptr[address+2] = 0x80;
		break;
		default:
		_ptr[address+2] = value;
		break;
	}
}

void RenardTX::sendPacket(){
	
	_serial->write(_ptr, _size+2);
	_serial->flush();
	
	free(_ptr);
}