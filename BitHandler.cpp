#include "BitHandler.h"
#include <iostream>
#include <algorithm>

BitHandler::BitHandler()
	:currentByte(0), position(7), bufferHandle(NULL)
{
	byteBuffer.clear();
}


BitHandler::~BitHandler()
{
}

void BitHandler::setPos()
{
	/*generate a number with only a 1 turned on at the current position*/
	short op = 1;
	op = op << position--;
	currentByte |= op;

	checkPos();
}

void BitHandler::clearPos()
{
	--position;
	checkPos();
}

void BitHandler::checkPos()
{
	if(position < 0)
	{
		byteBuffer.push_back(currentByte);
		position = 7;
		currentByte = 0;
	}
}

void BitHandler::stringToBits(const std::string& bits)
{
	for(int i = 0; i < bits.length(); ++i)
	{
		if(bits[i] != '0' && bits[i] != '1')
			printf("Invalid string\n");
	}

	for(int i = 0; i < bits.length(); ++i)
	{
		if(bits[i] == '0'){
			clearPos();
		}
		else{
			setPos();
		}
	}
}

const unsigned char* const BitHandler::getBuffer()
{
	/*If the writer bit position is not at the first bit of the next byte, 
	  write the bits in the currentByte to the buffer, it will be padded*/
	if(position != 7)
		byteBuffer.push_back(currentByte);

	/*Save the meta data about number of padded bits in the last byte as a byte*/
	byteBuffer.push_back((unsigned char)(position + 1));

	if(bufferHandle != NULL)
		free(bufferHandle);

	bufferHandle = (unsigned char*)malloc(byteBuffer.size());
	int counter = 0;

	for(std::list<unsigned char>::const_iterator itr = byteBuffer.begin(); itr != byteBuffer.end(); ++itr)
	{
		bufferHandle[counter] = *itr;
		++counter;
	}
	//append null terminator to c string
	bufferHandle[byteBuffer.size()] = '\0';
	return bufferHandle;
}

