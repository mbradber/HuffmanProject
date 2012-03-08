#pragma once
#include<cstdio>
#include<vector>
#include<list>
#include<stack>
#include"ByteData.h"

#define NUM_BYTES 256

class Huffman
{
public:
	Huffman(void);
	~Huffman(void);

	void countBytes(const unsigned char* const buffer, long bufferSize);
	void generateCodes() const { generateCodes(root, ""); }
	void display() const { display(root); }
	void printFrequency() const;
	const std::vector<ByteData>& getLookupList() const { return lookupList; }
	const std::string const getSerializedTree();
	const int getSerializedSize() const { return numSerialized; }
	void loadTree(const unsigned char* const fileBuffer, long bufferSize);
	std::string decodeFile(const unsigned char* const fileBuffer, long bufferSize);

private:
	void buildTree();
	void generateCodes(const ByteData* node, std::string prefix) const;
	void display(const ByteData* const node) const;
	void serializeTree(const ByteData* const node);

	std::vector<ByteData> dataList;
	std::vector<ByteData> lookupList;
	mutable std::list<int> charList;
	std::stack<ByteData> charStack;
	bool countedOnce;
	ByteData* root;
	int dataIndex;
	int numSerialized;
};



