#include "FileHandler.h"


FileHandler::FileHandler(std::string fileIn, std::string fileOut)
	:filenameIn(fileIn), fileNameOut(fileOut), iStream(fileIn, std::ios::in | std::ios::binary | std::ios::ate), 
	buffer(NULL), fileSize(0), opened(false)
{
	if(iStream.is_open())
	{
		fileSize = (unsigned int)iStream.tellg();
		buffer = new unsigned char[fileSize];
	}
	else
	{
		printf("Could not open the file for reading, exiting program\n");
		getchar();
		exit(-1);
	}
}

void FileHandler::loadFile()
{
	iStream.seekg(0, std::ios::beg);

	try 
	{ 
		iStream.read((char*)buffer, fileSize);
		printf("File successfully loaded into memory...\n"); 
	}
	catch(...) 
	{ 
		printf("Exception thrown when loading file into memory, exiting program\n"); 
		getchar();
		exit(-1);
	}

	buffer[fileSize] = '\0';
	
	if(iStream.is_open())
		iStream.close();
}

void FileHandler::writeToFile(const unsigned char* const byteBuffer, int bufferSize)
{
	if(!opened)
	{
		if(!oStream.is_open())
			oStream.open(fileNameOut, std::ios::out | std::ios::binary | std::ios::ate);
		opened = true;
	}
	else
		if(!oStream.is_open())
			oStream.open(fileNameOut, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);

	if(!oStream.is_open())
	{
		printf("Could not open file for writing\n");
		getchar();
		exit(-1);
	}

	oStream.write((const char*)byteBuffer, bufferSize);
	oStream.close();
}

FileHandler::~FileHandler()
{
	if(iStream.is_open())
		iStream.close();
	if(oStream.is_open())
		oStream.close();
}
