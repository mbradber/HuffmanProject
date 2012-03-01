#pragma once

#include<string>
#include<fstream>

class FileHandler
{
public:
	FileHandler(std::string fileIn, std::string fileOut);
	~FileHandler(void);

	void loadFile();
	const unsigned char* const getBuffer() const { return buffer; }
	const unsigned int getFileSize() const { return fileSize; }
	void writeToFile(const unsigned char* const byteBuffer, int bufferSize);

private:
	std::string filenameIn;
	std::string fileNameOut;
	std::ofstream oStream;
	std::ifstream iStream;
	unsigned char* buffer;
	unsigned long fileSize;
	bool opened;
};