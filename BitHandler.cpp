#include "BitHandler.h"
#include <iostream>
#include <algorithm>
/*--------------------------------------------------------------------
  Constructor
  -------------------------------------------------------------------*/
BitHandler::BitHandler(const std::string file)
	:currentByte(0), position(7), outputFile(file)
{
	fileHandler.setOutputFile(outputFile);
	fileHandler.openOutStream(true);
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
		fileHandler.pushToBuffer(currentByte);
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
		if(bits[i] == '0')
			clearPos();
		else
			setPos();
	}

	fileHandler.flushBuffer();
}

void BitHandler::cleanUp()
{
	/*If the writer bit position is not at the first bit of the next byte, 
	write the bits in the currentByte to the buffer, it will be padded*/
	if(position != 7)
		fileHandler.pushToBuffer(currentByte);

	/*Save the meta data about number of padded bits in the last byte as a byte*/
	if(position == 7)
		fileHandler.pushToBuffer((char)0);
	else
		fileHandler.pushToBuffer((char)position + 1);

	fileHandler.flushBuffer();
	fileHandler.closeOutputStream();
}