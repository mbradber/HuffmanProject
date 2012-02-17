#include "Huffman.h"
#include<algorithm>

Huffman::Huffman()
	:dataList(NUM_BYTES), countedOnce(false)
{
	for(int i = 0; i < NUM_BYTES; ++i)
		dataList[i].setValue(ByteFrequency((unsigned char)i, 0));
}

void Huffman::countBytes(const unsigned char* const buffer, long bufferSize)
{
	if(!countedOnce)
	{
		for(int i = 0; i < bufferSize; ++i)
			dataList[(int)buffer[i]].getValue().Increment();

		countedOnce = true;
	}

	std::sort(dataList.begin(), dataList.end());
}



void Huffman::printFrequency() const
{
	std::for_each(dataList.begin(), dataList.end(), [](ByteData b){
		if(b.getValue().getFrequency() > 0)
			printf("Byte:%c, Frequency:%d\n", b.getValue().getData(), b.getValue().getFrequency());
	});
}

Huffman::~Huffman()
{
}
