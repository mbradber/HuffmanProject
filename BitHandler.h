#pragma once
#include<list>
#include<fstream>

class BitHandler
{
public:
	BitHandler(const std::string outputFile);
	~BitHandler();

	void setPos();
	void clearPos();
	void stringToBits(const std::string& bits);
	void cleanUp();

private:
	void checkPos();

	unsigned char currentByte;
	short position;
	std::ofstream oStream;
	std::string outputFile;
};

