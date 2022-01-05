// ============================================================================
//			Copyright (c) 2008 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include <vector>
#include <math.h>
#include <float.h>


#include "CUSketcher_LUDecomposition.h"


namespace CommonUtil
{
//-----------------------------------------------------------------------------

Sketcher_LUDecomposition::Sketcher_LUDecomposition(const Matrix &coeffMatrix,
								 const Matrix &rhsMatrix) :
								 m_coeffMatrix(coeffMatrix),
								 m_rhsMatrix(rhsMatrix)
{
}

//-----------------------------------------------------------------------------

bool Sketcher_LUDecomposition::Execute(Matrix &unknownMatrix)
{
	std::vector<int> indices;

	double d;
	if(!SeparateLowerUpper(indices, d))
		return false;

	unknownMatrix.Resize(m_coeffMatrix.GetNumRows(), 1);
	backSubstitution(indices, unknownMatrix);
	return true;
}

//-----------------------------------------------------------------------------

bool Sketcher_LUDecomposition::SeparateLowerUpper(std::vector<int> &indices, double &d)
{
	size_t rowSize = m_coeffMatrix.GetNumRows();
	std::vector<double>scalingVector;
	scalingVector.resize(rowSize);
	indices.resize(rowSize);
	int i=0, j=0;

	d = 1.0;

	for (i = 1; i <= rowSize; i++)
	{
		double maxRowValue = 0.0;
		for (j = 1; j <= rowSize; j++)
		{
			double tmpVal = fabs(m_coeffMatrix(i-1, j-1));
			if (tmpVal > maxRowValue)
				maxRowValue = tmpVal;
		}

        if ( maxRowValue != 0.0 )
        {
    		scalingVector[i-1] = 1./maxRowValue;
        }
        else
        {
//          scalingVector[i-1] = DBL_MAX;
			return false;
        }
	}

	for(j = 1; j <= rowSize; j++)
	{
		for(i = 1; i < j; i++)
		{
			double sum = m_coeffMatrix(i-1, j-1);

			for(int k = 1; k < i; k++)
			{
				sum -= m_coeffMatrix(i-1, k-1) * m_coeffMatrix(k-1, j-1);
			}

			m_coeffMatrix(i-1, j-1) = sum;
		}

		double maxValue = 0.0;
		int    iMax     = 0;

		for(i = j; i <= rowSize; i++)
		{
			double sum = m_coeffMatrix(i-1, j-1);

			for(int k = 1; k < j; k++)
			{
				sum -= m_coeffMatrix(i-1, k-1) * m_coeffMatrix(k-1, j-1);
			}

			m_coeffMatrix(i-1, j-1) = sum;

			double tmpVal = scalingVector[i-1] * fabs(sum);

			if (tmpVal >= maxValue)
			{
				maxValue = tmpVal;
				iMax = i;
			}
		}

		if ( ( j != iMax ) && ( iMax != 0 ) )
		{
			for(int k = 1; k <= rowSize; k++)
			{
				double tmpVal = m_coeffMatrix(iMax-1, k-1);
				m_coeffMatrix(iMax-1, k-1) = m_coeffMatrix(j-1, k-1);
				m_coeffMatrix(j-1, k-1) = tmpVal;
			}
			d = - d;
			scalingVector[iMax-1] = scalingVector[j-1];
		}

		indices[j-1] = iMax;

		if(fabs(m_coeffMatrix(j-1, j-1)) < 1e-3)
			m_coeffMatrix(j-1, j-1) = 1e-3;

		if(j != rowSize)
		{
			double tmpVal = 1.0/m_coeffMatrix(j-1, j-1);

			for( i = j + 1; i <= rowSize; i++)
				m_coeffMatrix(i-1, j-1) *= tmpVal;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------

void Sketcher_LUDecomposition::backSubstitution(const std::vector<int> &indices,
									   Matrix &unknownMatrix)
{
	unknownMatrix = m_rhsMatrix;

	size_t rowSize = m_coeffMatrix.GetNumRows();

	size_t index = 0;
	size_t i=0;
	for(i = 1; i <= rowSize; i++)
	{
		size_t tmpIndex = indices[i-1];
		
		double sum = unknownMatrix(tmpIndex-1, 0);
		
		unknownMatrix(tmpIndex-1, 0) = unknownMatrix(i-1, 0);

		if(index)
		{
			for(size_t j = index; j <= i-1; j++)
			{
				sum -= m_coeffMatrix(i-1, j-1) * unknownMatrix(j-1, 0);
			}
		}
		else if(fabs(sum) > 1e-3)
		{
			index = i;
		}

		unknownMatrix(i-1, 0) = sum;
	}

	for(i = rowSize; i >= 1; i--)
	{
		double sum = unknownMatrix(i-1, 0);
		for(size_t j = i+1; j <= rowSize; j++)
		{
			sum -= m_coeffMatrix(i-1, j-1) * unknownMatrix(j-1, 0);
		}
		unknownMatrix(i-1, 0) = sum/ m_coeffMatrix(i-1, i-1);
	}
}

//-----------------------------------------------------------------------------

bool Sketcher_LUDecomposition::GetInverse(Matrix &inverseMatrix)
{
	Matrix unknownMatrix(1, m_coeffMatrix.GetNumRows());
	std::vector<int> indices;

	double dpara;
	if(!SeparateLowerUpper(indices, dpara))
		return false;

	inverseMatrix.Resize(m_coeffMatrix.GetNumCols(), m_coeffMatrix.GetNumRows());

	size_t rowSize = m_coeffMatrix.GetNumRows();
	size_t i=0, j=0;
	for(j = 0; j < rowSize; j++)
	{
		for(i = 0; i < rowSize; i++)
		{
			m_rhsMatrix(i,0) = 0;
		}
		m_rhsMatrix(j,0) = 1;
		backSubstitution(indices, unknownMatrix);
		for(i = 0; i < rowSize; i++)
		{
			inverseMatrix(i,j) = unknownMatrix(i,0);
		}
	}
	return true;
}



}




