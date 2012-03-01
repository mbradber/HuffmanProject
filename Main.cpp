#include<iostream>
#include<time.h>
#include"FileHandler.h"
#include"Huffman.h"
#include"BitHandler.h"
#include<bitset>

int main(int argc, char** argv)
{
	if(!strcmp(argv[1], "encode")){
		Huffman huff;
		BitHandler bitHandler;
		FileHandler fileHandler("C:\\Users\\Acer\\DataSets\\sample3.txt", "C:\\Users\\Acer\\DataSets\\sample3.out");
		fileHandler.loadFile();

		unsigned t0 = clock(), t1;

		huff.countBytes(fileHandler.getBuffer(), fileHandler.getFileSize());
		huff.generateCodes();
		std::vector<ByteData> checkList = huff.getLookupList();
		const unsigned char* const fileBuffer = fileHandler.getBuffer();
		std::string builder = "";

		for(unsigned int i = 0; i < fileHandler.getFileSize(); ++i)
			builder.append(checkList[(int)fileBuffer[i]].getCode());

		bitHandler.stringToBits(builder);
		const unsigned char* const bitBuff = bitHandler.getBuffer();
		const unsigned char* const treeBuff = huff.getSerializedTree();
		fileHandler.writeToFile(treeBuff, huff.getSerializedSize());
		fileHandler.writeToFile(bitBuff, bitHandler.getBufferSize());

		t1 = clock() - t0;
		printf("Exe Time: %d\n", t1);
	}
	else if(!strcmp(argv[1], "decode")){
		Huffman huff;
		FileHandler fileHandler("C:\\Users\\Acer\\DataSets\\sample3.out", "C:\\Users\\Acer\\DataSets\\sample3_d.txt");
		fileHandler.loadFile();
		huff.loadTree(fileHandler.getBuffer(), fileHandler.getFileSize());
		std::string decodedFile =
			huff.decodeFile(fileHandler.getBuffer(), fileHandler.getFileSize());
		//std::cout << "Decoded:" << decodedFile.c_str() << std::endl;
		fileHandler.writeToFile((unsigned char*)decodedFile.c_str(), decodedFile.size());
		//std::cout << huff.decodeFile(fileHandler.getBuffer(), fileHandler.getFileSize()) << std::endl;
		
	}

	getchar();
	return 0;
}