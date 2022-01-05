// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//standard include files
#include <float.h>

//math include files
#include "CUSqMatrix.h"
#include "CULUDecomposition.h"
#include "CUHouseHolder.h"

//util include files
#include "CUCommonConstants.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

SqMatrix::SqMatrix(size_t colSize, SqMatrix::MatrixType type)
		 : Matrix(colSize, colSize)
{
	if(type == IDENTITY)
	{
		// assigns value 1.0 to the diagonal elements
		for(int i = 0; i < colSize; ++i)
		{
			(*this)(i, i) = 1.0; 
		}
	}
}

//-----------------------------------------------------------------------------

SqMatrix::SqMatrix(int colSize, const std::vector<double> &values)
:Matrix(colSize,values)
{
	assert (GetNumRows() == GetNumCols());
}

//-----------------------------------------------------------------------------

SqMatrix::SqMatrix(const SqMatrix &matrix) : Matrix(matrix)
{
}

//-----------------------------------------------------------------------------

void SqMatrix::getCofactorMatrix(SqMatrix &cofactor, int rowNo,
								 int columnNo)const
{
	size_t numRows = GetNumRows();
	Matrix tempCofactorMatrix(numRows, numRows);
	for(int i = 0; i < numRows; ++i)
	{
		for(int j = 0; j < numRows; ++j)
		{
			tempCofactorMatrix(i,j) = (*this)(i,j);
		}
	}

	tempCofactorMatrix.RemoveRow(rowNo);
	tempCofactorMatrix.RemoveColumn(columnNo);
	
	cofactor.Resize(numRows - 1, numRows - 1);
	for(int i = 0; i < numRows-1; ++i)
	{
		for(int j = 0; j < numRows-1; ++j)
		{
			cofactor(i,j) = tempCofactorMatrix(i,j);
		}
	}
}

//-----------------------------------------------------------------------------

const SqMatrix& SqMatrix::operator = (const SqMatrix &matrix)
{
	if( this != &matrix )
    {
		this->Matrix::operator = (matrix);
	}
	return *this;
}

//-----------------------------------------------------------------------------

void SqMatrix::operator - (SqMatrix &matrix)
{
	for(int j = 0; j < matrix.GetNumRows(); ++j)
		{
			for(int k = 0; k < matrix.GetNumCols(); ++k)
			{
				matrix(j,k) = 0 - matrix(j,k);
			}
		}
}

//-----------------------------------------------------------------------------

double SqMatrix::ComputeDeterminent()const
{
	//If the entire matrix is NULL, LUDecomposition fails.
	//so check the first row for Non-NULL elements to proceed.

	bool nullMatrix = true;
	size_t numCols = GetNumCols();
	for (int i = 0; i < numCols; ++i)
	{
		double val = (*this)(0, i);
		if (fabs(val) > CommonUtil::CommonConstants::ZERO)
		{
			nullMatrix = false;
			break;
		}
	}

	if(nullMatrix)
		return 0;

	SqMatrix coeffMatrix(*this);

	Matrix dummyUnknownMatrix(1, coeffMatrix.GetNumRows());
	LUDecomposition ludcmp(coeffMatrix, dummyUnknownMatrix);

	std::vector<int> indices;
	double d;

	ludcmp.SeparateLowerUpper(indices, d);
	Matrix changedLUCoeffMatrix;
	ludcmp.GetCoeffMatrix(changedLUCoeffMatrix);

	size_t numRowsOfCoeffMatrix = changedLUCoeffMatrix.GetNumRows();
	for( int j = 0; j < numRowsOfCoeffMatrix; ++j)
	{
		d *= changedLUCoeffMatrix(j, j);
	}
	return d;
}

//-----------------------------------------------------------------------------

void SqMatrix::GetMaxEigenVector(std::vector<double> &eigenVector)
{
	eigenVector.clear();

	std::vector<double> eigenValues;
	SqMatrix eigen_vetor_matrix(GetNumRows());

	Householder h;
	h.Execute(*(this), eigenValues);

	double max = eigenValues[0];
	size_t max_index = 0;

	for (size_t i = 0; i < GetNumRows(); i++)
	{
		if (eigenValues[i] > max)
		{
			max = eigenValues[i];
			max_index = i;
		}
	}
	for (size_t j = 0; j < GetNumRows(); j++)
	{
		eigenVector.push_back((*this)(j, max_index));
	}
}

//-----------------------------------------------------------------------------

void SqMatrix::GetMinEigenVector(std::vector<double> &eigenVector)
{
	eigenVector.clear();

	std::vector<double> eigenValues;
	SqMatrix eigen_vetor_matrix(GetNumRows());

	Householder h;
	h.Execute(*(this), eigenValues);

	double min = eigenValues[0];
	size_t min_index = 0;

	for (size_t i = 0; i < GetNumRows(); i++)
	{
		if (eigenValues[i] < min)
		{
			min = eigenValues[i];
			min_index = i;
		}
	}
	for (size_t j = 0; j < GetNumRows(); j++)
	{
		eigenVector.push_back((*this)(j, min_index));
	}
}

//-----------------------------------------------------------------------------

void SqMatrix::GetEigenVectorForMinEigenValue(std::vector<double> &minEigenVector,
	double &minEigenVal, double &cost, double* eigenValueArray)const
{
	minEigenVector.clear();
	minEigenVal = 0.0;

	std::vector<double> eigenValues;
	SqMatrix eigen_vector_matrix((*this));

	// householder method gives the eigen values and all the corresponding 
	// eigen vectors of the current matrix
	Householder h;
	h.Execute1(*this, eigenValues, eigen_vector_matrix);

	//eigen values does not have negative value
	assert(eigenValues[0] >= 0 || eigenValues[1] >= 0 || eigenValues[2] >= 0);

	double min = eigenValues[0];
	size_t min_index = 0;


	size_t numRows = GetNumRows();
	// index corresponding to the minimum eigen value is found
	for (size_t i = 0; i< numRows; i++)
	{
		if (eigenValues[i] < min)
		{
			min = eigenValues[i];
			min_index = i;
		}
	}
	minEigenVal = min;

	if (eigenValueArray)
	{
		eigenValueArray[0] = eigenValues[0];
		eigenValueArray[1] = eigenValues[1];
		eigenValueArray[2] = eigenValues[2];
	}

	/*if((fabs(eigenValues[0]) - fabs(eigenValues[1])) < RevLib::PNT_TOL)
	chkColliner = true;

	if((fabs(eigenValues[1]) - fabs(eigenValues[2])) < RevLib::PNT_TOL)
	chkColliner = true;

	if((fabs(eigenValues[2]) - fabs(eigenValues[0])) < RevLib::PNT_TOL)
	chkColliner = true;*/

	//curvature cost (minEigenValue/sum of eigen value)
	cost = minEigenVal / (eigenValues[0] + eigenValues[1] + eigenValues[2]);

	// the eigen vector corresponding to the minimum eigen value is copied into 
	// minEigenVector
	for (size_t j = 0; j < numRows; j++)
	{
		minEigenVector.push_back(eigen_vector_matrix(j, min_index));
	}
}

//-----------------------------------------------------------------------------

void SqMatrix::GetEigenValues(std::vector<double> &eigenValues)const
{
	SqMatrix eigenVector(GetNumRows());
	Householder h;
	h.Execute1(*this, eigenValues, eigenVector);	
}

//-----------------------------------------------------------------------------

void SqMatrix::GetEigenInfo(double eigenValues[], Vector eigenVectors[])const
{
	SqMatrix eigenVector(GetNumRows());
	Householder h;
	std::vector<double> tmpVals;
	h.Execute1(*this, tmpVals, eigenVector);	
	
	eigenValues[0] = tmpVals[0];
	eigenValues[1] = tmpVals[1];
	eigenValues[2] = tmpVals[2];

	eigenVectors[0].Set(eigenVector(0,0),eigenVector(1,0),eigenVector(2,0));
	eigenVectors[1].Set(eigenVector(0,1),eigenVector(1,1),eigenVector(2,1));
	eigenVectors[2].Set(eigenVector(0,2),eigenVector(1,2),eigenVector(2,2));
}

//-----------------------------------------------------------------------------

bool SqMatrix::IsSymmetric()const
{
	bool stat = true;
	size_t numRow = GetNumRows();
	SqMatrix tempMatrix(numRow);

	int num = 1;
	for(int j = 0; j < numRow - 1; ++j)
	{
		for(int i = num; i < numRow; ++i)
		{
			tempMatrix(i, j) = (*this)(i, j);
		}
		num++;
	}

	SqMatrix tempMatrix1(numRow);
	int num1 = 1;
	for(int i = 0; i < numRow - 1; ++i)
	{
		for(int j = num1; j< numRow; ++j)
		{
			tempMatrix1(i, j) = (*this)(i, j);
		}
		num1++;
	}
	
	SqMatrix transpose;
	tempMatrix.GetTranspose(transpose);

	int num2 = 1;
	for(int i = 0; i < numRow; ++i)
	{
		for(int k = num2; k < numRow; ++k)
		{
			if(tempMatrix1(i,k) != transpose(i,k))
			{
				stat = false;
				return stat;
			}
			
		}
		num2++;
	}
	return stat;
}

//------------------------------------------------------------------------

void SqMatrix::GetInverse(SqMatrix &inverseMatrix)const
{
	Matrix dummyUnknownMatrix(1, GetNumRows());
	LUDecomposition solver(*this, dummyUnknownMatrix);
	solver.GetInverse(inverseMatrix);
}
//------------------------------------------------------------------------


}