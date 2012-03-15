#pragma once

#include<string>
#include<fstream>

class FileHandler
{
public:
	FileHandler();
	FileHandler(std::string fileIn, std::string fileOut);
	~FileHandler(void);

	void loadFile();
	const unsigned char* const getBuffer() const { return buffer; }
	const unsigned int getFileSize() const { return fileSize; }
	void writeToFile(const unsigned char* const byteBuffer, int bufferSize);
	void pushToBuffer(const unsigned char c) { oStream.put(c); }
	void flushBuffer() { oStream.flush(); }
	void openOutStream(bool append);
	void openInStream();
	void setOutputFile(const std::string s) { fileNameOut = s; }
	void setInputFile(const std::string s) { fileNameIn = s; }
	void closeOutputStream() { if(oStream.is_open()) oStream.close(); }
	void closeInputStream() { if(iStream.is_open()) iStream.close(); }

private:
	std::string fileNameIn;
	std::string fileNameOut;
	std::ofstream oStream;
	std::ifstream iStream;
	unsigned char* buffer;
	unsigned long fileSize;
	bool opened;
};