// ============================================================================
//			Copyright (c) 2008 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include <vector>
#include <math.h>
#include <float.h>
#include "CUMatrixRankComputer.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

MatrixRankComputer::MatrixRankComputer(const Matrix &coeffMatrix) :
				m_coeffMatrix(coeffMatrix)
{
	m_numRowsCheckedForRank = 0;
}

//-----------------------------------------------------------------------------

MatrixRankComputer::~MatrixRankComputer()
{}

//-----------------------------------------------------------------------------

size_t MatrixRankComputer::GetRank()
{
	size_t rank = 0;
	if(	!m_coeffMatrix.GetNumRows() || !m_coeffMatrix.GetNumCols())
		return rank;

	//if(m_numRowsCheckedForRank != m_coeffMatrix.GetNumRows()-1)
	{
		Matrix copyLhs(m_coeffMatrix);
		Matrix copyRhs(1, m_coeffMatrix.GetNumRows());
		gaussJordenOperation(copyLhs, copyRhs);
		rank = copyLhs.GetNumRows();
		m_reducedMatrix = copyLhs;
		m_numRowsCheckedForRank = m_reducedMatrix.GetNumRows();
	}
	//else
	{
		//Special Rank
		//add the last row to m_reducedMatrix


		//reduce the last row of m_reducedMatrix

	}

	return rank;
}

//-----------------------------------------------------------------------------

void MatrixRankComputer::gaussJordenOperation(Matrix& lhs, 
												   Matrix& rhs)
{
	size_t i, j;

	Matrix& aMatrix = lhs, orgAMat = lhs, orgBMat = rhs;
	size_t numRows = lhs.GetNumRows();
	size_t numCols = lhs.GetNumCols();
	
	double tolerance = lhs.GetAbsMaxValue() * DBL_EPSILON;
	size_t maxCount = (numRows < numCols) ? numRows : numCols;
	for(i = 0;i < maxCount; i++)
	{
		size_t nRow = -1, nCol = -1;
		if(aMatrix.FindNextMaxElement(i, i, nRow, nCol, tolerance))
		{
			if(i != nRow)
			{
				swapRows(aMatrix, i, nRow);
				swapRows(orgAMat, i, nRow);
				swapRows(orgBMat, i, nRow);
			}
			if(i != nCol)
			{
				swapColumns(aMatrix, i, nCol);
			}
			modifyRows(i, aMatrix, tolerance);
		}
		else
			break;
	}
	if(i < numRows)
	{
		lhs.Resize(i, numCols);
		rhs.Resize(i, 1);
		for(j=0; j<i; j++)
		{
			Matrix curRow;
			orgAMat.GetRowMatrix(j, curRow);
			lhs.SetAsRow(j, curRow);
			rhs(j, 0L) = orgBMat(j,0L);
		}
	}
}

//-----------------------------------------------------------------------------

void MatrixRankComputer::swapRows(Matrix& matrix, size_t index1, size_t index2)
{
	Matrix rowWithLessIndexValue, rowWithHightIndexValue;
	matrix.GetRowMatrix(index1, rowWithLessIndexValue);
	matrix.GetRowMatrix(index2, rowWithHightIndexValue);
	matrix.SetAsRow(index1, rowWithHightIndexValue);
	matrix.SetAsRow(index2, rowWithLessIndexValue);
}

//-------------------------------------------------------------------------------------

void MatrixRankComputer::swapColumns(Matrix& matrix, size_t index1, size_t index2)
{
	Matrix columnWithLessIndexValue, columnWithHighIndexValue;
	matrix.GetColumn(index1, columnWithLessIndexValue);
	matrix.GetColumn(index2, columnWithHighIndexValue);
	matrix.SetColumnElements(index1, columnWithHighIndexValue);
	matrix.SetColumnElements(index2, columnWithLessIndexValue);
}

//-------------------------------------------------------------------------------------

bool MatrixRankComputer::modifyRows(size_t currentElementIndex, Matrix& aMatrix, double tolerance)
{
	size_t numRows = aMatrix.GetNumRows();
	assert(currentElementIndex < numRows);
	aMatrix.GetNumCols();

	Matrix pivotRow;
	aMatrix.GetRowMatrix(currentElementIndex, pivotRow);

	//pivotRow.MultiplyScalar(); // Need to check/Reviewed
	Matrix pivotRow1;
	pivotRow.ScalarMultiply(1.0/pivotRow(0L,currentElementIndex), pivotRow1);
	pivotRow = pivotRow1; // Need to check/Reviewed

	for(size_t i=currentElementIndex+1; i < numRows; i++)
	{
		Matrix curRow, tmpRow = pivotRow;

		double currentVal = aMatrix(i,currentElementIndex);

		if(fabs(currentVal) > DBL_MIN)
		{
			//tmpRow.MultiplyScalar(aMatrix(i,currentElementIndex)); // Need to check/Reviewed
			Matrix tmpRow1;
			tmpRow.ScalarMultiply(currentVal, tmpRow1);
			tmpRow = tmpRow1; // Need to check/Reviewed

			aMatrix.GetRowMatrix(i, curRow);

			Matrix curRow1; // Need to check/Reviewed
			curRow.Subtract(tmpRow, curRow1);
			curRow = curRow1; // Need to check/Reviewed

			aMatrix.SetAsRow(i, curRow);
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------



}




