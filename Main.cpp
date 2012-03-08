#include<iostream>
#include<time.h>
#include"FileHandler.h"
#include"Huffman.h"
#include"BitHandler.h"
#include<bitset>

#define d

int main(int argc, char** argv)
{
	//FileHandler fileHandler("C:\\Users\\Acer\\DataSets\\ws15.docx", "C:\\Users\\Acer\\DataSets\\wsout.out");
	//FileHandler fileHandler2("C:\\Users\\Acer\\DataSets\\ws15_d.docx", "C:\\Users\\Acer\\DataSets\\results.txt");

	//fileHandler.loadFile();
	//fileHandler2.loadFile();

	//const unsigned char* buff1 = fileHandler.getBuffer();
	//const unsigned char* buff2 = fileHandler2.getBuffer();

	//for(int i = 0; i < fileHandler2.getFileSize(); ++i){
	//	if((int)buff2[i] == 197 && (int)buff2[i+1] == 208 && (int)buff2[i+2] == 237){
	//		printf("Sequence found at index %d\n", i);
	//		//printf("Int version of char:%d", (int)buff1[fileHandler.getFileSize() / 2]);
	//		//printf("Int version of char:%d", (int)buff1[fileHandler.getFileSize() / 2 + 1]);
	//		//printf("Int version of char:%d", (int)buff1[fileHandler.getFileSize() / 2 + 2]);
	//	}
	//}

	//FileHandler fileHandler("C:\\Users\\Acer\\DataSets\\ws15.docx", "C:\\Users\\Acer\\DataSets\\nullTest.txt");
	//BitHandler bitHandler;
	//bitHandler.stringToBits("00000000000000000110010100000000");
	//const unsigned char* const bitBuff = bitHandler.getBuffer();
	//fileHandler.writeToFile(bitBuff, bitHandler.getBufferSize());

#ifdef e
	//if(!strcmp(argv[1], "encode")){
		Huffman huff;
		BitHandler bitHandler;
		FileHandler fileHandler("C:\\Users\\Acer\\DataSets\\artist.jpg", "C:\\Users\\Acer\\DataSets\\artist.out");
		fileHandler.loadFile();

		unsigned t0 = clock(), t1;

		huff.countBytes(fileHandler.getBuffer(), fileHandler.getFileSize());
		huff.generateCodes();
		std::vector<ByteData> checkList = huff.getLookupList();
		const unsigned char* const fileBuffer = fileHandler.getBuffer();
		printf("File buffer:%d\n", strlen((const char*)fileBuffer));
		std::string builder = "";

		for(unsigned int i = 0; i < fileHandler.getFileSize(); ++i){
			builder.append(checkList[(int)fileBuffer[i]].getCode());
			if(fileBuffer[i] == NULL)
				//printf("Byte is NULL\n");
			//printf("Appending code:%s\n", checkList[(int)fileBuffer[i]].getCode().c_str());
		}
		bitHandler.stringToBits(builder);

		//printf("Builder:%s\n", builder.c_str());

		const unsigned char* const bitBuff = bitHandler.getBuffer();
		const std::string const treeBuff = huff.getSerializedTree();
		//int charCount = 0;
		//for(int i = 0; i < huff.getSerializedSize(); ++i)
		//	if(treeBuff[i] != NULL)
		//		++charCount;
		printf("Count %d\n", huff.getSerializedSize());
		std::string numCharString = "";
		numCharString += (unsigned char)huff.getSerializedSize();

		printf("Serialized tree:%s\n", (unsigned char*)treeBuff.c_str());
		
		fileHandler.writeToFile((unsigned char*)numCharString.c_str(), 1);
		fileHandler.writeToFile((unsigned char*)treeBuff.c_str(), treeBuff.size());
		fileHandler.writeToFile(bitBuff, bitHandler.getBufferSize());

		t1 = clock() - t0;
		printf("Exe Time: %d\n", t1);
		printf("File encoded\n");
#endif
//	}
//	else if(!strcmp(argv[1], "decode")){
#ifdef d
		Huffman huff;
		FileHandler fileHandler("C:\\Users\\Acer\\DataSets\\artist.out", "C:\\Users\\Acer\\DataSets\\artist_d.jpg");
		fileHandler.loadFile();
		huff.loadTree(fileHandler.getBuffer(), fileHandler.getFileSize());
		std::string decodedFile =
			huff.decodeFile(fileHandler.getBuffer(), fileHandler.getFileSize());
		fileHandler.writeToFile((unsigned char*)decodedFile.c_str(), decodedFile.size());
		printf("File decoded.\n");
		
//	}
#endif

	getchar();
	return 0;
}