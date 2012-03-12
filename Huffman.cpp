#include "Huffman.h"
#include<algorithm>
#include<sstream>
#include<list>
#include<bitset>
#include<cstring>
#include<cstdlib>
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
	
	if(root == NULL){
		printf("Empty tree, exiting program");
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
/*---------------------------------------------------------------------*/
void Huffman::display(const ByteData* const node) const
{
	if(node->getLeftNode() != 0)
		display(node->getLeftNode());

	printf("%c : %d\n", (char)node->getValue().getData(), node->getValue().getFrequency());

	if(node->getRightNode() != 0)
		display(node->getRightNode());
}
/*---------------------------------------------------------------------*/
void Huffman::printFrequency() const
{
	std::for_each(dataList.begin(), dataList.end(), [](ByteData b){
		if(b.getValue().getFrequency() > 0)
			printf("Byte:%c, Frequency:%d\n", b.getValue().getData(), b.getValue().getFrequency());
	});
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

	/*Clean up*/
	//if(node->getParentNode() != NULL)
	//{
	//	if(node->getParentNode()->getLeftNode() == node)
	//		node->getParentNode()->setLeftNode(NULL);
	//	else if(node->getParentNode()->getRightNode() == node)
	//		node->getParentNode()->setRightNode(NULL);
	//}

	//delete node;
	//node = NULL;
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
		exit(-1);
	}
}
/*---------------------------------------------------------------------
  Cycle through the encoded bytes of the buffer argument and store their
  binary values into a string. Then cycle through that string to traverse
  the constructed Huffman Tree. When you stop at a leaf, store that leaf
  in the return string.
  ---------------------------------------------------------------------*/
std::string Huffman::decodeFile(const unsigned char* const fileBuffer, long bufferSize)
{
	/*The meta char is the last byte of the encoded file. This data 
	  represents the number of 'padded' bits of the last byte (meaning
	  they should not be used to traverse.)*/
	int meta = (int)fileBuffer[bufferSize - 1];
	std::string builder = "";
	std::string returnString = "";

	for(int i = dataIndex; i < bufferSize - 1; ++i)
		builder += std::bitset<CHAR_BIT>(fileBuffer[i]).to_string().c_str();

	/*The endIndex will be the position in the file buffer to stop reading
	  characters to traverse the Huffman tree*/
	int endIndex = builder.length();
	if(meta > 0)
		endIndex = builder.length() - meta;

	const ByteData* trav = root;

	for(int i = 0; i < endIndex; ++i)
	{
		if(root->getValue().getData() != NIL)
			returnString += (char)root->getValue().getData();
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
/*---------------------------------------------------------------------
  Destructor
  ---------------------------------------------------------------------*/
Huffman::~Huffman()
{
}