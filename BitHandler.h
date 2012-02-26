#pragma once
#include<list>

class BitHandler
{
public:
	BitHandler();
	~BitHandler();

	void setPos();
	void clearPos();
	const unsigned char* const getBuffer();
	void stringToBits(const std::string& bits);
	const int getBufferSize() const { return byteBuffer.size(); }

private:
	void checkPos();

	unsigned char currentByte;
	short position;
	mutable unsigned char* bufferHandle;
	std::list<unsigned char> byteBuffer;
};

