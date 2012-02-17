//#define DEBUG

#include<iostream>
#include<time.h>
#include"FileHandler.h"
#include"Huffman.h"

int main(int argc, char** argv)
{
	Huffman huff;
	FileHandler fileHandler("C:\\Users\\Acer\\DataSets\\artist.jpg");
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
	huff.printFrequency();

	t1 = clock() - t0;

	printf("Exe Time: %d\n", t1);
	printf("Size: %d\n", fileHandler.getFileSize());

	getchar();
}