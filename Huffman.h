#pragma once
#include<cstdio>
#include<vector>
#include"ByteData.h"

#define NUM_BYTES 256

class Huffman
{
public:
	Huffman(void);
	~Huffman(void);

	void countBytes(const unsigned char* const buffer, long bufferSize);
	void printFrequency() const;

private:
	std::vector<ByteData> dataList;
	bool countedOnce;
};



