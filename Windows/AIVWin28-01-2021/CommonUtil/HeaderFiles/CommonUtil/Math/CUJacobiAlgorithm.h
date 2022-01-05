#ifndef _JACOBIALGORITHM_H_
#define _JACOBIALGORITHM_H_

//standard include files
#include<vector>

namespace CommonUtil
{
// Forward declarations
class SqMatrix;
#define FREE_ARG char*

//This class implements the method for computing eigen values and eigen vectors
//of symetric matrix by using Jacobi transformation method 
class JacobiAlgorithm
{
	bool computeEigenValues(CommonUtil::SqMatrix &a, std::vector<double> &d, CommonUtil::SqMatrix &v, int &nrot);

	void rotate(CommonUtil::SqMatrix &a, const double s, const double tau, 
				const int i, const int j, const int k, const int m);

public :

	//Constructor 
	JacobiAlgorithm(){}

	//Destructor
	~JacobiAlgorithm(){}

	bool Execute(CommonUtil::SqMatrix &mat, std::vector<double> &eigenValues,
				 CommonUtil::SqMatrix &eigenVector);

};
}

#endif