#include "Huffman.h"

Huffman::Huffman()
	:countedOnce(false)
{
	/*init frequency array to 0*/
	for(int i = 0; i < MAX_BYTES; ++i)
		frequencyTable[i] = 0;
}

void Huffman::countBytes(const unsigned char* const buffer, long bufferSize)
{
	if(!countedOnce)
	{
		for(int i = 0; i < bufferSize; ++i)
			++frequencyTable[(int)buffer[i]];
		countedOnce = true;
	}
}

void Huffman::printFrequency() const
{
	for(int i = 0; i < MAX_BYTES - 1; ++i)
	{
		if(frequencyTable[i] > 0)
		{
			printf("Byte:%c, ", (unsigned char)i);
			printf("Frequency:%d\n", frequencyTable[i]);
		}
	}
}

Huffman::~Huffman()
{
}
