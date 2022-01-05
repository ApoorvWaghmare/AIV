// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include "CUConverter.h"
#include "CUCommonConstants.h"

using namespace std;

//-----------------------------------------------------------------------------

namespace CommonUtil
{

//-----------------------------------------------------------------------------

Converter::Converter()
{

}

//-----------------------------------------------------------------------------

Converter::~Converter()
{
	
}

//-----------------------------------------------------------------------------

bool Converter::DoubleToString(double number, string &numString)
{
	bool success = false;
	numString = "";
	if(0 > number)
	{
		numString += "-";
		number *= -1;
	}

	__int64 firstPart = (__int64)number;
	string numPart1, numPart2;
	getString(firstPart, numPart1);
	numString += numPart1 + "." ;

	double secondPartTemp = number - firstPart;

	correctDecimalPart(secondPartTemp, numString);
	
	int secondPart = (int) (secondPartTemp * 1000000); //as max precision is 6 for unit 'm'
	getString(secondPart, numPart2);

	numString += numPart2;

	return success;
}

//-----------------------------------------------------------------------------

void Converter::getString(__int64 number, string &numString)
{
	numString = "";
	string numStringTemp;
	int remainder = 0;
	__int64 quotient = number;
	bool revertString = false;
	
	if(0 == quotient)//if input string is 0.000
	{
		numString = "0";
		return;
	}

	for( ;quotient > 0; )
	{
		remainder = quotient % 10;
		quotient = (__int64) (quotient / 10);
		numStringTemp += (remainder + 48);
		revertString = true;
	}

	if(revertString)
	{
		for(int iCount = (numStringTemp.length() - 1); iCount >= 0; --iCount)
		{
			numString += numStringTemp[iCount];
		}
	}
}

void Converter::correctDecimalPart(double secondPart, string& numString)
{
	secondPart = (int) (secondPart * 1000000);
	if(100000 > secondPart && 10000 <= secondPart)
	{
		numString += "0";
	}
	else if(10000 > secondPart && 1000 <= secondPart)
	{
		numString += "00";
	}
	else if(1000 > secondPart && 100 <= secondPart)
	{
		numString += "000";
	}
	else if(100 > secondPart && 10 <= secondPart)
	{
		numString += "0000";
	}
	else if(10 > secondPart && 1 <= secondPart)
	{
		numString += "00000";
	}
	else if (1 > secondPart)
	{
		numString += "000000";
	}
}

//void Converter::getSecondString(double number, string &numPart2)
//{
//
//}

}
