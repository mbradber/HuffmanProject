#pragma once
#include<cstdio>
#define MAX_BYTES 256

class Huffman
{
public:
	Huffman(void);
	~Huffman(void);

	void countBytes(const unsigned char* const buffer, long bufferSize);
	void printFrequency() const;

private:
	short frequencyTable[MAX_BYTES];
	bool countedOnce;
};



