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
		BitHandler bitHandler;
		FileHandler fileHandler(inputFile, outputFile);
		/*Load file into memory*/
		fileHandler.loadFile();

		/*Count the frequency of bytes in order to then generate Huffman codes for each byte*/
		huff.countBytes(fileHandler.getBuffer(), fileHandler.getFileSize());
		huff.generateCodes();

		std::vector<ByteData> checkList = huff.getLookupList();
		const unsigned char* const fileBuffer = fileHandler.getBuffer();

		/*Iterate through the bytes of the file and output its Huffman code*/
		std::string builder = "";
		for(unsigned int i = 0; i < fileHandler.getFileSize(); ++i)
			builder.append(checkList[(int)fileBuffer[i]].getCode());
		bitHandler.stringToBits(builder);

		/*Acquire the binary representation of the Huffman encoding and the 
		  Huffman tree in order to write them to file*/
		const unsigned char* const bitBuff = bitHandler.getBuffer();
		const std::string treeBuff = huff.getSerializedTree();

		/*Also store the number of unique characters that appeared in the file*/
		std::string numCharString = "";
		numCharString += (unsigned char)huff.getSerializedSize();

		/*Write data to output file*/
		fileHandler.writeToFile((unsigned char*)numCharString.c_str(), 1);
		fileHandler.writeToFile((unsigned char*)treeBuff.c_str(), treeBuff.size());
		fileHandler.writeToFile(bitBuff, bitHandler.getBufferSize());

		/*Prompt completion*/
		printf("File encoded.\n");
	}

	else if(branch == DECODE)
	{
		printf("Decoding...\n");
		Huffman huff;
		FileHandler fileHandler(inputFile, outputFile);

		/*Load file into memory*/
		fileHandler.loadFile();

		/*Load the serialized Huffman tree into memory and use it to decode the rest of the
		  serialized file*/
		huff.loadTree(fileHandler.getBuffer(), fileHandler.getFileSize());
		std::string decodedFile =
			huff.decodeFile(fileHandler.getBuffer(), fileHandler.getFileSize());

		/*Write the decoded string out to a file*/
		fileHandler.writeToFile((unsigned char*)decodedFile.c_str(), decodedFile.size());

		/*Prompt completion*/
		printf("File decoded.\n");
	}

	//getchar();
	//getchar();
	return 0;
}