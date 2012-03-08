#include "Huffman.h"
#include<algorithm>
#include<sstream>
#include<list>
#include<bitset>
#include<cstring>
#include<cstdlib>

Huffman::Huffman()
	:dataList(NUM_BYTES), lookupList(NUM_BYTES), countedOnce(false), 
	root(NULL), dataIndex(0), numSerialized(0)
{
	for(short i = 0; i < NUM_BYTES; ++i)
	{
		dataList[i].setValue(ByteFrequency(i, 0));
		lookupList[i].setValue(ByteFrequency(i, 0));
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

		ByteData newNode(ByteFrequency(NIL, a->getValue().getFrequency() + b->getValue().getFrequency()));
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
	if(root->getValue().getData() != NIL)
	{
		root->setCode("0");
		lookupList[0].setCode("0");
		return;
	}

	if(node->getLeftNode() != 0)
		generateCodes(node->getLeftNode(), prefix + "0");

	node->setCode(prefix);
	if(node->getValue().getData() != NIL)
	{
		lookupList[node->getValue().getData()].setCode(node->getCode().c_str());
		printf("%d : %s\n", (unsigned char)node->getValue().getData(), node->getCode().c_str());
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

const std::string const Huffman::getSerializedTree()
{
	serializeTree(root);
	//unsigned char* bufferHandle = (unsigned char*)malloc(charList.size());
	std::stringstream builder;
	int counter = 0;

	for(std::list<int>::const_iterator itr = charList.begin(); itr != charList.end(); ++itr)
		builder << *itr << ",";
		//bufferHandle[counter++] = (unsigned char)(*itr);

	//append null terminator to c string
	//bufferHandle[charList.size()] = '\0';

	//return bufferHandle;
	//std::cout << builder.str() << std::endl;
	return builder.str();
}

void Huffman::serializeTree(const ByteData* const node)
{
	if(node->getLeftNode() != 0)
		serializeTree(node->getLeftNode());

	if(node->getRightNode() != 0)
		serializeTree(node->getRightNode());

	if(node->getValue().getData() != NIL) ++numSerialized;

	charList.push_back(node->getValue().getData());
}

void Huffman::loadTree(const unsigned char* const fileBuffer, long bufferSize)
{
	short numChars = (short)fileBuffer[0];
	if(numChars == 0) numChars = 256;

	short numCharsOnStack = 0;
	char* tokenize = (char*)&(fileBuffer[1]);
	dataIndex = 1;

	char* token = strtok(tokenize, ",");

	while(token != NULL)
	{
		dataIndex += strlen(token) + 1;
		int stackable = atoi(token);

		if(stackable != NIL)
		{
			charStack.push(ByteData(stackable));
			++numCharsOnStack;
		}
		else if(charStack.size() >= 2)
		{
			ByteData* a = new ByteData(charStack.top());
			charStack.pop();
			ByteData* b = new ByteData(charStack.top());
			charStack.pop();

			ByteData* newNode = new ByteData(NIL);
			newNode->setRightNode(a);
			newNode->setLeftNode(b);

			charStack.push(*newNode);
		}


		if(charStack.size() == 1 && numCharsOnStack == numChars)
		{
			root = &charStack.top();
			break;
		}

		token = strtok(NULL, ",");
	}


	//bool init = false;
	//int i = 1;
	//for(; i < bufferSize; ++i)
	//{
	//	if(fileBuffer[i] != NULL)
	//	{
	//		charStack.push(ByteData(fileBuffer[i]));
	//		++numCharsOnStack;
	//		//printf("Pushed char %c at index %i and stack size is now %d\n", fileBuffer[i], i, charStack.size());
	//	}
	//	else if(charStack.size() >= 2)
	//	{
	//		//printf("Stack size before pops:%d\n", charStack.size());
	//		ByteData* a = new ByteData(charStack.top());
	//		charStack.pop();
	//		ByteData* b = new ByteData(charStack.top());
	//		charStack.pop();

	//		//printf("Popped %c\n", a->getValue().getData());
	//		//printf("Popped %c\n", b->getValue().getData());

	//		ByteData* newNode = new ByteData(NULL);
	//		newNode->setRightNode(a);
	//		newNode->setLeftNode(b);

	//		charStack.push(*newNode);

	//		//printf("Top of stack now has children %c and %c\n", charStack.top().getLeftNode()->getValue().getData(), charStack.top().getRightNode()->getLeftNode()->getValue().getData());
	//		//printf("Stack size after pops and push:%d\n", charStack.size());

	//		if(charStack.size() == 1 && numCharsOnStack == numChars)
	//		{
	//			root = &charStack.top();
	//			break;
	//		}
	//	}

		//printf("Got here\n");
		//printf("Top of stack now has children %c and %c\n", charStack.top().getLeftNode()->getValue().getData(), charStack.top().getRightNode()->getLeftNode()->getValue().getData());
	//}
	//dataIndex += 2;
	printf("NumChars:%d, NumCharsOnStack:%d\n", numChars, numCharsOnStack);
	//printf("Root:%d\n", root->getRightNode()->getRightNode()->getRightNode()->getRightNode()->getValue().getData());
	//printf("Char num:%c\n", root->getRightNode()->getRightNode()->getRightNode()->getRightNode()->getValue().getData());
	//return (const unsigned char* const)builder.c_str();
	//dataIndex = (int)token[strlen(token)] - (int)fileBuffer[0];
	printf("Data index:%d\n", dataIndex);
}

std::string Huffman::decodeFile(const unsigned char* const fileBuffer, long bufferSize)
{
	int meta = (int)fileBuffer[bufferSize - 1];
	printf("Meta:%d\n", meta);
	std::string builder = "";
	std::string returnString = "";

	for(int i = dataIndex; i < bufferSize - 1; ++i)
		builder += std::bitset<CHAR_BIT>(fileBuffer[i]).to_string().c_str();

	//printf("Builder:%s\n", builder.c_str());

	int endIndex = builder.length();
	if(meta > 0)
		endIndex = builder.length() - meta;
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

	for(int i = 0; i < endIndex; ++i)
	{
		if(root->getValue().getData() != NIL)
			returnString += root->getValue().getData();
		else
		{
			if(builder[i] == '0')
			{
				trav = trav->getLeftNode();

				if(trav->getValue().getData() != NIL)
				{
					returnString += (char)trav->getValue().getData();
					trav = root;
				}
			}
			if(builder[i] == '1')
			{
				trav = trav->getRightNode();

				if(trav->getValue().getData() != NIL)
				{
					returnString += (char)trav->getValue().getData();
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