#pragma once
#include<cstdio>
#include<vector>
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

private:
	void buildTree();
	void generateCodes(const ByteData* node, std::string prefix) const;
	void display(const ByteData* const node) const;

	std::vector<ByteData> dataList;
	bool countedOnce;
	ByteData* root;
};



