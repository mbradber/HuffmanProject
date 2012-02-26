//#define DEBUG

#include<iostream>
#include<time.h>
#include"FileHandler.h"
#include"Huffman.h"
#include"BitHandler.h"

int main(int argc, char** argv)
{
	Huffman huff;
	BitHandler bitHandler;
	FileHandler fileHandler("C:\\Users\\Acer\\DataSets\\moby.txt", "output");
	fileHandler.loadFile();

#ifdef DEBUG
	for(int i = 0; i < fileHandler.getFileSize(); ++i)
	{
		//printf("%d\n", (int)fileHandler.getBuffer()[i]);
		short x = (short)fileHandler.getBuffer()[i];
		printf("Integer:%d\n", x);
		printf("Char:%c\n", (unsigned char)x);
	}
	printf("File size: %d\n", sizeof(*fileHandler.getBuffer()));
#endif
	unsigned t0 = clock(), t1;

	huff.countBytes(fileHandler.getBuffer(), fileHandler.getFileSize());
	huff.generateCodes();
	std::vector<ByteData> checkList = huff.getLookupList();
	const unsigned char* const fileBuffer = fileHandler.getBuffer();
	std::string builder = "";

	for(unsigned int i = 0; i < fileHandler.getFileSize(); ++i)
		builder += checkList[(int)fileBuffer[i]].getCode();

	bitHandler.stringToBits(builder);
	const unsigned char* const bitBuff = bitHandler.getBuffer();
	fileHandler.writeToFile(bitBuff, bitHandler.getBufferSize());

	t1 = clock() - t0;

	printf("Exe Time: %d\n", t1);
	printf("Size: %d\n", fileHandler.getFileSize());

	getchar();
	return 0;
}