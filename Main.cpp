#include<iostream>
#include<bitset>
#include<time.h>
#include"FileHandler.h"
#include"Huffman.h"
#include"BitHandler.h"

#define ENCODE 0
#define DECODE 1

int main(int argc, char** argv)
{
	/*Handle interactive prompt options*/

	bool branch;
	printf("Encode or Decode(e or d)?");
	std::string mode = "";
	std::cin >> mode;

	if(!mode.compare("Encode") || !mode.compare("encode") || !mode.compare("e"))
		branch = ENCODE;
	else if(!mode.compare("Decode") || !mode.compare("decode") || !mode.compare("d"))
		branch = DECODE;
	else
	{
		printf("Invalid mode specified, exiting program\n");
		return -1;
	}

	printf("Enter an input file path:");
	std::string inputFile = "";
	std::cin >> inputFile;

	printf("Enter an output file path:");
	std::string outputFile = "";
	std::cin >> outputFile;

	/*Execute the encode/decode branch given by user*/

	if(branch == ENCODE)
	{
		printf("Encoding...\n");
		Huffman huff;
		FileHandler fileHandler(inputFile, outputFile);
		/*Load file into memory*/
		fileHandler.loadFile();

		unsigned t0 = clock(), t1;

		/*Count the frequency of bytes in order to then generate Huffman codes for each byte*/
		huff.countBytes(fileHandler.getBuffer(), fileHandler.getFileSize());
		huff.generateCodes();

		/*Acquire the binary representation of the Huffman tree in order to write it to a file*/
		const std::string treeBuff = huff.getSerializedTree();

		/*Also store the number of unique characters that appeared in the file*/
		std::string numCharString = "";
		numCharString += (unsigned char)huff.getSerializedSize();

		/*Write data to output file*/
		fileHandler.writeToFile((unsigned char*)numCharString.c_str(), 1);
		fileHandler.writeToFile((unsigned char*)treeBuff.c_str(), treeBuff.size());

		/*Iterate through the bytes of the file and output their Huffman codes as bytes
		  to the output file*/
		huff.encodeFile(fileHandler.getBuffer(), fileHandler.getFileSize(), outputFile);

		/*Prompt completion*/
		printf("File encoded.\n");
		t1 = clock() - t0;
		printf("Exe Time: %d\n", t1);
	}

	else if(branch == DECODE)
	{
		printf("Decoding...\n");
		Huffman huff;
		FileHandler fileHandler;
		fileHandler.setInputFile(inputFile);
		fileHandler.openInStream();

		/*Load file into memory*/
		fileHandler.loadFile();
		fileHandler.closeInputStream();

		unsigned t0 = clock(), t1;

		/*Load the serialized Huffman tree into memory and use it to decode the rest of the
		  serialized file*/
		huff.loadTree(fileHandler.getBuffer(), fileHandler.getFileSize());
		huff.decodeFile(fileHandler.getBuffer(), fileHandler.getFileSize(), outputFile);

		/*Prompt completion*/
		printf("File decoded.\n");
		t1 = clock() - t0;
		printf("Exe Time: %d\n", t1);
	}

	getchar();
	getchar();
	return 0;
}