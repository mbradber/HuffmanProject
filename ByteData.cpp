#include "ByteData.h"

/*ByteFrequency implementation*/

ByteFrequency::ByteFrequency()
	:data(NIL), frequency(0)
{}

ByteFrequency::ByteFrequency(const ByteFrequency& copyMe)
	:data(copyMe.getData()), frequency(copyMe.getFrequency())
{}

ByteFrequency::ByteFrequency(const short& pValue)
	:data(pValue), frequency(1)
{}

ByteFrequency::ByteFrequency(const short& pValue, const unsigned int& pFrequency)
	:data(pValue), frequency(pFrequency)
{}

ByteFrequency& ByteFrequency::operator=(const ByteFrequency& rhs)
{
	data = rhs.getData();
	frequency = rhs.getFrequency();

	return *this;
}

bool operator < (const ByteFrequency& lhs, const ByteFrequency& rhs)
{
	if(lhs.getData() < rhs.getData())
		return true;
	else
		return false;
}

/*ByteData implementation*/

ByteData::ByteData()
	:value(), leftNode(0), rightNode(0), parentNode(0), code("")
{}

ByteData::ByteData(ByteFrequency bf):
value(bf), leftNode(0), rightNode(0), parentNode(0), code("")
{}

ByteData::~ByteData(void)
{}

void ByteData::setLeftNode(ByteData* const ln)
{
	ln->setParentNode(this);
	leftNode = ln;
}

void ByteData::setRightNode(ByteData* const rn)
{
	rn->setParentNode(this);
	rightNode = rn;
}

void ByteData::setParentNode(ByteData* const pn)
{
	parentNode = pn;
}

bool operator < (const ByteData& lhs, const ByteData& rhs)
{ 
	if(lhs.getValue().getFrequency() > rhs.getValue().getFrequency())
		return true;
	else
		return false;
}