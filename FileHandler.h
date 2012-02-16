#pragma once

#include<string>
#include<fstream>

class FileHandler
{
public:
	FileHandler(std::string file);
	~FileHandler(void);

	void loadFile();
	const unsigned char* const getBuffer() const { return buffer; }
	const unsigned int getFileSize() const { return fileSize; }

private:
	std::string filename;
	std::ifstream iStream;
	unsigned char* buffer;
	unsigned long fileSize;
};




