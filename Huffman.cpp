#include "Huffman.h"
#include<algorithm>
#include<sstream>

Huffman::Huffman()
	:dataList(NUM_BYTES), countedOnce(false), root(NULL)
{
	for(int i = 0; i < NUM_BYTES; ++i)
		dataList[i].setValue(ByteFrequency((unsigned char)i, 0));
}

void Huffman::countBytes(const unsigned char* const buffer, long bufferSize)
{
	if(!countedOnce)
	{
		for(int i = 0; i < bufferSize; ++i)
			dataList[(int)buffer[i]].getValue().Increment();

		countedOnce = true;
	}
	//sort the results
	std::sort(dataList.begin(), dataList.end());
	//purge the zeros from the data list
	for(std::vector<ByteData>::iterator itr = dataList.begin(); itr != dataList.end(); ++itr)
		if(itr->getValue().getFrequency() == 0)
		{
			dataList.erase(itr, dataList.end());
			break;
		}
	//build the Huffman tree
	buildTree();
}

void Huffman::buildTree()
{
	while(dataList.size() > 1)
	{
		ByteData* a = new ByteData(dataList.back());
		dataList.pop_back();
		ByteData* b = new ByteData(dataList.back());
		dataList.pop_back();

		ByteData newNode(ByteFrequency(0, a->getValue().getFrequency() + b->getValue().getFrequency()));
		newNode.setLeftNode(a);
		newNode.setRightNode(b);

		dataList.push_back(newNode);
		std::sort(dataList.begin(), dataList.end());
	}
	root = &dataList.front();

	if(root == NULL){
		printf("Empty tree, exiting program");
		exit(-1);
	}
}

void Huffman::generateCodes(const ByteData* node, std::string prefix) const
{
	if(node->getLeftNode() != 0)
		generateCodes(node->getLeftNode(), prefix + "0");

	node->setCode(prefix);
	printf("%c : %s\n", node->getValue().getData(), node->getCode().c_str());

	if(node->getRightNode() != 0)
		generateCodes(node->getRightNode(), prefix + "1");
}

void Huffman::display(const ByteData* const node) const
{
	if(node->getLeftNode() != 0)
		display(node->getLeftNode());

	printf("%c : %d\n", (char)node->getValue().getData(), node->getValue().getFrequency());

	if(node->getRightNode() != 0)
		display(node->getRightNode());
}

void Huffman::printFrequency() const
{
	std::for_each(dataList.begin(), dataList.end(), [](ByteData b){
		if(b.getValue().getFrequency() > 0)
			printf("Byte:%c, Frequency:%d\n", b.getValue().getData(), b.getValue().getFrequency());
	});
}

Huffman::~Huffman()
{
}
