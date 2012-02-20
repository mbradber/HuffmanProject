#pragma once
#include<iostream>

class ByteFrequency
{
public:
	ByteFrequency();
	ByteFrequency(const ByteFrequency& copyMe);
	ByteFrequency(const unsigned char& pValue);
	ByteFrequency(const unsigned char& pValue, const unsigned int& pFrequency);

	void Increment() const { ++frequency; }
	const unsigned char getData() const { return data; }
	const unsigned int getFrequency() const { return frequency; }

	ByteFrequency& operator=(const ByteFrequency& rhs);
	friend bool operator < (const ByteFrequency& lhs, const ByteFrequency& rhs);

private:
	unsigned char data;
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
	const ByteData* const getLeftNode() const { return leftNode; }
	const ByteData* const getRightNode() const { return rightNode; }
	const ByteData* const getParentNode() const { return parentNode; }
	const std::string getCode() const { return code; }

	friend bool operator < (const ByteData& lhs, const ByteData& rhs);

private:
	ByteFrequency value;
	ByteData* leftNode;
	ByteData* rightNode;
	ByteData* parentNode;
	mutable std::string code;
};