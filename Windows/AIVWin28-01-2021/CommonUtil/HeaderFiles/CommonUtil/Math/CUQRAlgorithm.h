// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _ESSENCE_QRALGORITHM_H_
#define _ESSENCE_QRALGORITHM_H_

//standard include files
#include<vector>

namespace CommonUtil
{
// Forward declarations
class SqMatrix;

//This class implements the method for solving linear equations by QR decomposition
//R is upper triangular martix and Q is orthogonal matrix
class QRAlgorithm
{
	//======================= private members functions =======================

	//
	void balance(CommonUtil::SqMatrix &givenMatrix, size_t n);

	//for CCI
	void reduceHessenberg(CommonUtil::SqMatrix &givenMatrix, size_t n);

	//Finds all eigenvalues of an upper Hessenberg matrix a[1..n][1..n].
	//On input a can be exactly as output from elmhes x11.5
	//on output it is destroyed. The real and imaginary parts of the 
	//eigenvalues are returned in wr[1..n] and wi[1..n], respectively.
	void qrForHessenberg(CommonUtil::SqMatrix &givenMatrix, size_t n,
						std::vector<double> &realEigenValues,
							 std::vector<double> &imaginaryEigenValues);

	double sign(double a,double b)
	{
		return ((b) >0.0 ? fabs(a) : -fabs(a));
	}

public :

	//======================== public member functions ========================


	//**************************general methods********************************

	//execute the methode
	void Execute(CommonUtil::SqMatrix &matrix, std::vector<double> &realEigenValues,
					 std::vector<double> &imaginaryEigenValues);

	//displays the matrix on dos screen
	void DisplayMatrix(const CommonUtil::SqMatrix &m);
	

};
}
#endif
