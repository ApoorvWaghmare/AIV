#ifndef _COMMONUTIL_MATRIXOPERATIONSUSINGINTEL_H_
#define _COMMONUTIL_MATRIXOPERATIONSUSINGINTEL_H_

namespace CommonUtil
{
class Matrix;

// This class represents matrix operations using Intel
class MatrixOperationsUsingIntel
{
	//========================== private data members =========================

	//======================= private members functions =======================

	//************************************************************************
	
public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	MatrixOperationsUsingIntel(){};
	
	//****************************** destructor *******************************

	// Destructor
	virtual ~MatrixOperationsUsingIntel(){};

	//***************************   *****************************

	void multiply(bool bTransA, const Matrix& rMatA, 
								   bool bTransB, const Matrix& rMatB,
								   Matrix& result);
	
	//************************************************************************
	
	bool getInverseMKL(const Matrix& input, Matrix& inverseMatrix);
};

}
#endif