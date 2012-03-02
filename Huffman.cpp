#include "Huffman.h"
#include<algorithm>
#include<sstream>
#include<list>
#include<bitset>

Huffman::Huffman()
	:dataList(NUM_BYTES), lookupList(NUM_BYTES), countedOnce(false), root(NULL), dataIndex(0)
{
	for(int i = 0; i < NUM_BYTES; ++i)
	{
		dataList[i].setValue(ByteFrequency((unsigned char)i, 0));
		lookupList[i].setValue(ByteFrequency((unsigned char)i, 0));
	}

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
		ByteData* const a = new ByteData(dataList.back());
		dataList.pop_back();
		ByteData* const b = new ByteData(dataList.back());
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
	if(node->getValue().getData() != 0)
	{
		lookupList[(int)node->getValue().getData()].setCode(node->getCode().c_str());
		printf("%c : %s\n", node->getValue().getData(), node->getCode().c_str());
	}

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

const unsigned char* const Huffman::getSerializedTree() const
{
	serializeTree(root);

	unsigned char* bufferHandle = (unsigned char*)malloc(charList.size());
	int counter = 0;

	for(std::list<unsigned char>::const_iterator itr = charList.begin(); itr != charList.end(); ++itr)
		bufferHandle[counter++] = *itr;

	//append null terminator to c string
	bufferHandle[charList.size()] = '\0';

	//for(int i = 0; i < charList.size(); ++i)
	//{
	//	if(bufferHandle[i] == '\r')
	//		printf("Return at %d\n", i);
	//	if(bufferHandle[i] == 't')
	//		printf("t at %d\n", i);
	//	if(bufferHandle[i] == '\0')
	//		printf("NULL at %d\n", i);
	//	if(bufferHandle[i] == 'h')
	//		printf("h at %d\n", i);
	//	if(bufferHandle[i] == '\n')
	//		printf("newline at %d\n", i);
	//}

	return bufferHandle;
}

void Huffman::serializeTree(const ByteData* const node) const
{
	if(node->getLeftNode() != 0)
		serializeTree(node->getLeftNode());

	if(node->getRightNode() != 0)
		serializeTree(node->getRightNode());

	charList.push_back(node->getValue().getData());
}

void Huffman::loadTree(const unsigned char* const fileBuffer, long bufferSize)
{
	bool init = false;
	int i = 0;
	for(i = 0; i < bufferSize; ++i)
	{
		if(fileBuffer[i] != NULL){
			charStack.push(ByteData(fileBuffer[i]));
			//printf("Pushed char %c at index %i and stack size is now %d\n", fileBuffer[i], i, charStack.size());
		}
		else if(charStack.size() >= 2)
		{
			//printf("Stack size before pops:%d\n", charStack.size());
			ByteData* a = new ByteData(charStack.top());
			charStack.pop();
			ByteData* b = new ByteData(charStack.top());
			charStack.pop();

			//printf("Popped %c\n", a->getValue().getData());
			//printf("Popped %c\n", b->getValue().getData());

			ByteData* newNode = new ByteData(NULL);
			newNode->setRightNode(a);
			newNode->setLeftNode(b);

			charStack.push(*newNode);

			//printf("Top of stack now has children %c and %c\n", charStack.top().getLeftNode()->getValue().getData(), charStack.top().getRightNode()->getLeftNode()->getValue().getData());
			//printf("Stack size after pops and push:%d\n", charStack.size());

			if(charStack.size() == 1){
				if(!init)
					init = true;
				else
				{
					root = &charStack.top();
					break;
				}

				//root = &charStack.top();

				//printf("Top of stack now has children %c and %c\n", charStack.top().getLeftNode()->getValue().getData(), charStack.top().getRightNode()->getLeftNode()->getValue().getData());

				//break;
			}
		}

		//printf("Got here\n");
		//printf("Top of stack now has children %c and %c\n", charStack.top().getLeftNode()->getValue().getData(), charStack.top().getRightNode()->getLeftNode()->getValue().getData());
	}

	//printf("Node:%c\n", root->getRightNode()->getLeftNode()->getValue().getData());
	//std::string builder = "";
	//for(i += 1; i < bufferSize; ++i)
	//{
	//	int meta = 0;
	//	if(i == bufferSize - 2)
	//	{
	//		meta = (int)fileBuffer[bufferSize - 1];

	//		//....
	//	}
	//	else
	//	{
	//		int decValue = (int)fileBuffer[i];
	//		std::string asBinary = std::bitset<CHAR_BIT>(decValue).to_string();
	//		
	//		builder += asBinary;
	//	}

	//}
	//printf("Char num:%d\n", root->getLeftNode()->getLeftNode()->getRightNode()->getValue().getData());
	//return (const unsigned char* const)builder.c_str();
	dataIndex = i + 1;
}

std::string Huffman::decodeFile(const unsigned char* const fileBuffer, long bufferSize)
{
	int meta = (int)fileBuffer[bufferSize - 1];
	printf("Meta:%d\n", meta);
	std::string builder = "";
	std::string returnString = "";

	for(int i = dataIndex; i < bufferSize - 1; ++i)
		builder += std::bitset<CHAR_BIT>(fileBuffer[i]).to_string().c_str();

	printf("Builder:%s\n", builder.c_str());

	int endIndex = builder.length();
	if(meta > 0)
		endIndex = builder.length() - (meta - 1);
	const ByteData* trav = root;

	//for(int i = 0; i < endIndex; ++i)
	//{
	//	if(trav->getValue().getData() != NULL)
	//	{
	//		returnString += trav->getValue().getData();
	//		//printf("Added char:%c\n", trav->getValue().getData());
	//		trav = root;

	//		if(builder[i] == '0')
	//		{
	//			trav = trav->getLeftNode();
	//		}
	//		else if(builder[i] == '1')
	//		{
	//			trav = trav->getRightNode();
	//		}
	//	}
	//	else if(builder[i] == '0')
	//	{
	//		trav = trav->getLeftNode();
	//	}
	//	else if(builder[i] == '1')
	//	{
	//		trav = trav->getRightNode();
	//	}
	//}

	for(int i = 0; i < builder.length(); ++i)
	{
		if(root->getValue().getData() != NULL)
			returnString += root->getValue().getData();
		else
		{
			if(builder[i] == '0')
			{
				trav = trav->getLeftNode();

				if(trav->getValue().getData() != NULL)
				{
					returnString += trav->getValue().getData();
					trav = root;
				}
			}
			if(builder[i] == '1')
			{
				trav = trav->getRightNode();

				if(trav->getValue().getData() != NULL)
				{
					returnString += trav->getValue().getData();
					trav = root;
				}
			}
		}
	}

	return returnString;
}

Huffman::~Huffman()
{
}