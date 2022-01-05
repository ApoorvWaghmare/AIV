// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_HOUSEHOLDER_H_
#define _COMMONUTIL_HOUSEHOLDER_H_

//standard include files
#include<vector>

//math include files
#include"CUSqMatrix.h"

namespace CommonUtil
{

//This class implements the Householder method to compute the eigen values and 
// eigen vectors of a square matrix
class Householder
{
	//======================== private member functions =======================

	
	//**************************general methods********************************

	//compute (a^2 + b^2) ^ (1/2)) without destructive underflow or overflow
	//i.e. returns zero if fabs(a) = fabs(b) = 0
	double pythag(double a, double b);

	double sign(double a,double b)
	{
		return ((b) >0.0 ? fabs(a) : -fabs(a));
	}

	//
	void triad( SqMatrix &a , size_t n , std::vector<double> &d , std::vector<double> &e);

	//
	void triadQLI(std::vector<double> &d,std::vector<double> &e, size_t n, SqMatrix &z);


	/*--------------------------- Methods In STEPReader --------------------------------
	void triad( SqMatrix &a , int n , std::vector<double> &d , std::vector<double> &e);
	void triadQLI(std::vector<double> &d,std::vector<double> &e, int n, SqMatrix &z);
	----------------------------------------------------------------------------------*/

public:
	
	//======================== public member functions ========================


	//************************** list of constructors *************************

	Householder()
	{}

	//****************************** destructor *******************************

	~Householder()
	{}

	//**************************general methods********************************

	//Executes the Householder method and computes the eigen values of square matrix
	//The matrix is also modified and contains correspoding eigen vectors
	void Execute(SqMatrix &matrix, std::vector<double> &eigenValues);

	void Execute1(const SqMatrix &matrix, std::vector<double> &eigenValues,
		SqMatrix &eigenVectorMatrix);
};

}

#endif