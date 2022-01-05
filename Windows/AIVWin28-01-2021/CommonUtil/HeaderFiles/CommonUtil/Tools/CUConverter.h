//// ============================================================================
////			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
////								All Rights Reserved
//// ============================================================================
#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include <string>

using namespace std;

namespace CommonUtil
{

class Converter
{

	void getString(__int64 number, string &numString);

	void correctDecimalPart(double secondPart, string& numString);

public:
	
	Converter();

	~Converter();

	bool DoubleToString(double number, string &numString);
};

}//Converter	

#endif