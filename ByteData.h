#pragma once
#include<iostream>

#define NIL 256

class ByteFrequency
{
public:
	ByteFrequency();
	ByteFrequency(const ByteFrequency& copyMe);
	ByteFrequency(const short& pValue);
	ByteFrequency(const short& pValue, const unsigned int& pFrequency);

	void Increment() const { ++frequency; }
	const short getData() const { return data; }
	const unsigned int getFrequency() const { return frequency; }

	ByteFrequency& operator=(const ByteFrequency& rhs);
	friend bool operator < (const ByteFrequency& lhs, const ByteFrequency& rhs);

private:
	short data;
	mutable unsigned int frequency;
};

class ByteData
{
public:
	ByteData();
	ByteData(ByteFrequency bf);
	~ByteData();

	void setValue(const ByteFrequency& b) { value = b; }
	void setLeftNode(ByteData* const ln);
	void setRightNode(ByteData* const rn);
	void setParentNode(ByteData* const pn);
	void setCode(const std::string& bitString) const { code = bitString; }

	const ByteFrequency& getValue() const { return value; }
	ByteData* getLeftNode() const { return leftNode; }
	ByteData* getRightNode() const { return rightNode; }
	ByteData* getParentNode() const { return parentNode; }
	const std::string getCode() const { return code; }

	friend bool operator < (const ByteData& lhs, const ByteData& rhs);

private:
	ByteFrequency value;
	ByteData* leftNode;
	ByteData* rightNode;
	ByteData* parentNode;
	mutable std::string code;
};