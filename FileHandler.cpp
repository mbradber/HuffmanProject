#include "FileHandler.h"
/*---------------------------------------------------------------------
  Constructors
  ---------------------------------------------------------------------*/
FileHandler::FileHandler()
	:buffer(NULL), fileSize(0), opened(false)
{
}

FileHandler::FileHandler(std::string fileIn, std::string fileOut)
	:fileNameIn(fileIn), fileNameOut(fileOut), iStream(fileIn, std::ios::in | std::ios::binary | std::ios::ate), 
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
/*---------------------------------------------------------------------
  Load a file on disk into memory and record the file size
  ---------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------
  Write the buffer argument to the output file associated with this object
  ---------------------------------------------------------------------*/
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

void FileHandler::openOutStream(bool append)
{
	if(append)
		oStream.open(fileNameOut, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	else
		oStream.open(fileNameOut, std::ios::out | std::ios::binary);

	if(!oStream.is_open())
	{
		printf("Could not open file for writing\n");
		getchar();
		exit(-1);
	}
}

void FileHandler::openInStream()
{
	iStream.open(fileNameIn, std::ios::in | std::ios::binary | std::ios::ate);

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

/*---------------------------------------------------------------------
  Destructor
  ---------------------------------------------------------------------*/
FileHandler::~FileHandler()
{
	if(iStream.is_open())
		iStream.close();
	if(oStream.is_open())
		oStream.close();
}
