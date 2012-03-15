#pragma once
#include<cstdio>
#include<vector>
#include<list>
#include<stack>
#include"ByteData.h"
#include"BitHandler.h"
#include"FileHandler.h"

#define NUM_BYTES 256

class Huffman
{
public:
	Huffman(void);
	~Huffman(void);

	void countBytes(const unsigned char* const buffer, long bufferSize);
	void generateCodes() const { generateCodes(root, ""); }
	const std::string getSerializedTree();
	const int getSerializedSize() const { return numSerialized; }
	void loadTree(const unsigned char* const fileBuffer, long bufferSize);
	void decodeFile(const unsigned char* const fileBuffer, unsigned int fileSize, std::string outputFile);
	void encodeFile(const unsigned char* const fileBuffer, unsigned int fileSize, std::string outputFile);

private:
	void buildTree();
	void generateCodes(const ByteData* node, std::string prefix) const;
	void serializeTree(ByteData* node);

	std::vector<ByteData> dataList;
	std::vector<ByteData> lookupList;
	mutable std::list<int> charList;
	std::stack<ByteData> charStack;
	bool countedOnce;
	ByteData* root;
	int dataIndex;
	int numSerialized;
};



