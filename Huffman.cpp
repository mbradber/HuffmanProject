#include "Huffman.h"
#include<algorithm>
#include<sstream>
#include<list>
#include<bitset>
#include<cstring>
#include<cstdlib>

#define ENCODE_BUFFER_SIZE 104857600 /*100 MB*/
#define DECODE_BUFFER_SIZE 104857600 /*100 MB*/

/*---------------------------------------------------------------------
  Constructor
  ---------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------
  Iterate through the contents of the 'buffer' argument and record the 
  frequency of each byte, then sort the dataList of bytes by their 
  frequencies and remove the bytes with 0 occurrences from the list.
  ---------------------------------------------------------------------*/
void Huffman::countBytes(const unsigned char* const buffer, long bufferSize)
{
	if(!countedOnce)
	{
		for(int i = 0; i < bufferSize; ++i)
			dataList[(int)buffer[i]].getValue().Increment();

		countedOnce = true;
	}
	/*sort the results*/
	std::sort(dataList.begin(), dataList.end());
	/*purge the zeros from the data list*/
	for(std::vector<ByteData>::iterator itr = dataList.begin(); itr != dataList.end(); ++itr)
		if(itr->getValue().getFrequency() == 0)
		{
			dataList.erase(itr, dataList.end());
			break;
		}
	/*build the Huffman tree*/
	buildTree();
}
/*---------------------------------------------------------------------
  Cycle through the list and repeatedly take the two nodes of the lowest
  frequency, remove them from the list, then create a new node with these
  two nodes as its children with the frequency being the sum of its two children.
  ---------------------------------------------------------------------*/
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
	
	if(root == NULL)
	{
		printf("Empty tree, exiting program");
		getchar();
		exit(-1);
	}
}
/*---------------------------------------------------------------------
  Recurse through the tree in depth first order, and collect the list of bits
  as a path from the root (where 0 means left branch, and 1 means right). 
  Whenever a leaf is hit, store the path of bits as its code.  
  ---------------------------------------------------------------------*/
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
		lookupList[node->getValue().getData()].setCode(node->getCode().c_str());

	if(node->getRightNode() != 0)
		generateCodes(node->getRightNode(), prefix + "1");
}
/*---------------------------------------------------------------------
  Return a list of comma separated values containing the Huffman tree
  in depth first, post order.
  ---------------------------------------------------------------------*/
const std::string Huffman::getSerializedTree()
{
	serializeTree(root);
	std::stringstream builder;
	int counter = 0;

	for(std::list<int>::const_iterator itr = charList.begin(); itr != charList.end(); ++itr)
		builder << *itr << ",";

	return builder.str();
}
/*---------------------------------------------------------------------
  Store the values of the Huffman tree in depth first, post order into a 
  list.
  ---------------------------------------------------------------------*/
void Huffman::serializeTree(ByteData* node)
{
	if(node->getLeftNode() != 0)
		serializeTree(node->getLeftNode());

	if(node->getRightNode() != 0)
		serializeTree(node->getRightNode());

	if(node->getValue().getData() != NIL) ++numSerialized;

	charList.push_back(node->getValue().getData());
}

void Huffman::encodeFile(const unsigned char* const fileBuffer, unsigned int fileSize, std::string outputFile)
{
	BitHandler bitHandler(outputFile);
	std::string builder = "";

	for(unsigned int i = 0; i < fileSize; ++i)
	{
		builder.append(lookupList[(int)fileBuffer[i]].getCode());

		if(builder.size() >= ENCODE_BUFFER_SIZE)
		{
			bitHandler.stringToBits(builder);
			builder = "";
		}
	}

	bitHandler.stringToBits(builder);
	bitHandler.cleanUp();
}

/*---------------------------------------------------------------------
  Used for decoding: Read a serialized file and load the Huffman tree into
  memory.
  ---------------------------------------------------------------------*/
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

	if(root == NULL)
	{
		printf("Error reading encoded file, check input file path.\n");
		getchar();
		exit(-1);
	}
}
/*---------------------------------------------------------------------
  Cycle through the encoded bytes of the buffer argument and store their
  binary values into a string. Then cycle through that string to traverse
  the constructed Huffman Tree. When you stop at a leaf, store that leaf
  in the return string.
  ---------------------------------------------------------------------*/
void Huffman::decodeFile(const unsigned char* const fileBuffer, unsigned int fileSize, std::string outputFile)
{
	/*The meta char is the last byte of the encoded file. This data 
	  represents the number of 'padded' bits of the last byte (meaning
	  they should not be used to traverse.)*/
	FileHandler fileHandler;
	fileHandler.setOutputFile(outputFile);
	fileHandler.openOutStream(false);

	int meta = (int)fileBuffer[fileSize - 1];
	std::string lastByte = std::bitset<CHAR_BIT>(fileBuffer[fileSize - 2]).to_string();

	std::string builder = "";
	std::string returnString = "";
	const ByteData* trav = root;

	for(unsigned int i = dataIndex; i < fileSize - 2; ++i)
	{
		builder += std::bitset<CHAR_BIT>(fileBuffer[i]).to_string();

		if(builder.size() >= DECODE_BUFFER_SIZE)
		{
			for(unsigned int j = 0; j < builder.length(); ++j)
			{
				if(builder[j] == '0')
				{
					trav = trav->getLeftNode();

					if(trav->getValue().getData() != NIL)
					{
						returnString += (char)trav->getValue().getData();
						trav = root;
					}
				}
				if(builder[j] == '1')
				{
					trav = trav->getRightNode();

					if(trav->getValue().getData() != NIL)
					{
						returnString += (char)trav->getValue().getData();
						trav = root;
					}
				}
			}

			fileHandler.writeToFile((unsigned char*)returnString.c_str(), returnString.length());
			builder = "";
			returnString = "";
		}
	}

	/*Clean up*/
	builder += lastByte;
	int endIndex = builder.length();
	if(meta > 0)
		endIndex = builder.length() - meta;
	
	for(int i = 0; i < endIndex; ++i)
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

	fileHandler.writeToFile((unsigned char*)returnString.c_str(), returnString.length());
}

/*---------------------------------------------------------------------
  Destructor
  ---------------------------------------------------------------------*/
Huffman::~Huffman()
{
}