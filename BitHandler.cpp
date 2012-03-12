#include "BitHandler.h"
#include <iostream>
#include <algorithm>
/*--------------------------------------------------------------------
  Constructor
  -------------------------------------------------------------------*/
BitHandler::BitHandler()
	:currentByte(0), position(7), bufferHandle(NULL)
{
	byteBuffer.clear();
}
/*--------------------------------------------------------------------
  Destructor
  --------------------------------------------------------------------*/
BitHandler::~BitHandler()
{
}
/*--------------------------------------------------------------------
  Sets the current bit to a '1'
  --------------------------------------------------------------------*/
void BitHandler::setPos()
{
	short op = 1;
	op = op << position--;
	currentByte |= op;

	checkPos();
}
/*--------------------------------------------------------------------
  Sets the current bit position to '0'
  --------------------------------------------------------------------*/
void BitHandler::clearPos()
{
	--position;
	checkPos();
}
/*---------------------------------------------------------------------
  A check function that writes out a byte if it is full and resets the bit position
  ---------------------------------------------------------------------*/
void BitHandler::checkPos()
{
	if(position < 0)
	{
		byteBuffer.push_back(currentByte);
		position = 7;
		currentByte = 0;
	}
}
/*---------------------------------------------------------------------
  Converts strings of 1's and 0's to binary data
  ---------------------------------------------------------------------*/
void BitHandler::stringToBits(const std::string& bits)
{
	for(unsigned int i = 0; i < bits.length(); ++i)
	{
		if(bits[i] != '0' && bits[i] != '1')
			printf("Invalid string\n");
	}

	for(unsigned int i = 0; i < bits.length(); ++i)
	{
		if(bits[i] == '0')
			clearPos();
		else
			setPos();
	}
}
/*---------------------------------------------------------------------
  Returns an immutable representation of the binary buffer associated with this object
  ---------------------------------------------------------------------*/
const unsigned char* const BitHandler::getBuffer()
{
	/*If the writer bit position is not at the first bit of the next byte, 
	  write the bits in the currentByte to the buffer, it will be padded*/
	if(position != 7)
		byteBuffer.push_back(currentByte);

	/*Save the meta data about number of padded bits in the last byte as a byte*/
	if(position == 7)
		byteBuffer.push_back((unsigned char)(0));
	else
		byteBuffer.push_back((unsigned char)(position + 1));

	if(bufferHandle != NULL)
		delete(bufferHandle);

	bufferHandle = (unsigned char*)new char[byteBuffer.size()];//(unsigned char*)malloc(byteBuffer.size());
	int counter = 0;

	for(std::list<unsigned char>::const_iterator itr = byteBuffer.begin(); itr != byteBuffer.end(); ++itr)
	{
		bufferHandle[counter] = *itr;
		++counter;
	}

	bufferHandle[byteBuffer.size()] = '\0';
	return bufferHandle;
}