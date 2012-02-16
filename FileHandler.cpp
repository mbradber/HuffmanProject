#include "FileHandler.h"


FileHandler::FileHandler(std::string file)
	:filename(file), iStream(file, std::ios::in | std::ios::binary | std::ios::ate),
	buffer(NULL), fileSize(0)
{
	if(iStream.is_open())
	{
		fileSize = (unsigned int)iStream.tellg();
		buffer = new unsigned char[fileSize];
	}
	else
	{
		printf("Could not open the file, exiting program\n");
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

FileHandler::~FileHandler()
{
	if(iStream.is_open())
		iStream.close();
}
