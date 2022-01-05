// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//standard include files
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <deque>
#include <memory.h>

//util include files
#include "CUReport.h"

//math include files
#include "CUMatrix.h"
#include <float.h>
//#include "CUMatrixOperationsUsingIntel.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

Matrix::Matrix()
{
	m_matrixElements = 0;
	m_numRows = 0;
	m_numColumns = 0;
}

//-----------------------------------------------------------------------------

Matrix::Matrix(size_t colSize, size_t rowSize)
{
	m_matrixElements = 0;
	Resize (rowSize, colSize);
}

//-----------------------------------------------------------------------------

Matrix::Matrix(size_t colSize, const std::vector<double> &values)
{
	size_t numValues = values.size();

	assert(colSize > 0);
	assert(!(numValues % colSize));

	size_t rowSize = (numValues/colSize);

	m_matrixElements = 0;
	Resize(rowSize, colSize);

	//put the input values into the matrix elements
	for (size_t i = 0; i < rowSize * colSize; ++i)
	{
		m_matrixElements[i] = values[i];
	}
}

//-----------------------------------------------------------------------------

Matrix::Matrix (const Matrix &matrix)
{
	m_matrixElements = 0;
	copy(matrix);
}

//-----------------------------------------------------------------------------

Matrix::~Matrix()
{
	if (m_matrixElements)
		delete[] m_matrixElements;
	m_matrixElements = 0;
}

//-----------------------------------------------------------------------------

void Matrix::Resize(size_t rowSize, size_t colSize)
{
	if(m_matrixElements != NULL && (m_numRows != rowSize || m_numColumns != colSize))
	{
		delete [] m_matrixElements;
		m_matrixElements = NULL;
	}

	m_numRows = rowSize;
	m_numColumns = colSize;

	if(rowSize == 0 || colSize == 0)
	{
		m_matrixElements = NULL;
		return;
	}

	if(m_matrixElements == NULL )
		m_matrixElements = new double [m_numRows * m_numColumns];

	//set all the elements of matrix to zero
	memset (m_matrixElements, 0, m_numRows * m_numColumns * sizeof(double));
}

//-----------------------------------------------------------------------------

void Matrix::copy(const Matrix& matrix)
{
	size_t newRowSize = matrix.m_numRows;
	size_t newColSize = matrix.m_numColumns;

	Resize (newRowSize, newColSize);

	//copies all elements from matrix
	for(size_t i = 0; i < newRowSize; ++i)
	{
        size_t iMatrix = i*matrix.m_numColumns;
        size_t iThis   = i*m_numColumns;
		for(size_t j = 0; j < newColSize; ++j)
		{
             m_matrixElements[iThis + j] = matrix.m_matrixElements[iMatrix + j];
		}
	}
}

//-----------------------------------------------------------------------------

const Matrix& Matrix::operator = (const Matrix &matrix)
{
	if( this != &matrix )
    {
	    copy(matrix);
    }
	return *this;
}

//-----------------------------------------------------------------------------

void Matrix::GetTranspose(Matrix &transpose)const
{
	//initialize the transpose matrix
	transpose.Resize(m_numColumns, m_numRows);

	size_t rows = transpose.m_numRows;
	size_t cols = transpose.m_numColumns;

	for(size_t i = 0; i < rows; ++i)
	{
        size_t iTranspose = i*transpose.m_numColumns;
		for(size_t j = 0; j < cols; ++j)
		{
             transpose.m_matrixElements[iTranspose + j] = m_matrixElements[j*m_numColumns + i];
		}
	}
}

//-----------------------------------------------------------------------------

void Matrix::Add(const Matrix& m1, Matrix& result)const
{
	// Number of rows and columns should be same for the current matrix
	// and input matrix
	assert(m_numRows == m1.m_numRows && m_numColumns == m1.m_numColumns);

//	if(m_numRows 1= m1.m_numRows && m_numColumns 1= m1.m_numColumns)
//		return;

	//initialize the result matrix with the rows and columns of current matrix
	result.Resize(m_numRows, m_numColumns);

	for(size_t i = 0; i < m_numRows; ++i)
	{
        size_t iResult = i*result.m_numColumns;
        size_t iThis   = i*m_numColumns;
        size_t iM1     = i*m1.m_numColumns;
		for(size_t j = 0; j < m_numColumns; ++j)
		{
            result.m_matrixElements[iResult + j] = m_matrixElements[iThis + j] + m1.m_matrixElements[iM1 + j];
		}
	}
}

//-----------------------------------------------------------------------------

void Matrix::Subtract(const Matrix& m1, Matrix& result)const
{
	//Number of rows and columns should be same for the current matrix
	//and input matrix
	assert(m_numRows == m1.m_numRows && m_numColumns == m1.m_numColumns);

	// We need to intialize the result matrix to the number of rows and columns
	//of the two matrices
	result.Resize(m_numRows, m_numColumns);

	for(size_t i = 0; i < m_numRows; ++i)
	{
		size_t iResult = i*result.m_numColumns;
        size_t iThis   = i*m_numColumns;
        size_t iM1     = i*m1.m_numColumns;
		for(size_t j = 0; j < m_numColumns; ++j)
		{
            result.m_matrixElements[iResult + j] = m_matrixElements[iThis + j] - m1.m_matrixElements[iM1 + j];
		}
	}
}

//-----------------------------------------------------------------------------

void Matrix::Multiply (const Matrix& m1, Matrix& result)const
{
	size_t numColsOfOtherMatrix = m1.m_numColumns;

	//The number of columns of current matrix should be same as
	//number of rows from other matrix
	assert(m1.m_numRows == m_numColumns);
	bool mklUse = false;
	if (mklUse)
	{
		//CommonUtil::MatrixOperationsUsingIntel intelMatSolver;
		//intelMatSolver.multiply(false, *this, false, m1, result);
	}
	else
	{
		// the number of rows of the result matrix are made equal to those of the
		// current matrix and the number of columns are made equal to those of the
		// matrix m1
		result.Resize(m_numRows, numColsOfOtherMatrix);

		const size_t m1NumColumns = m1.m_numColumns;
		for (size_t i = 0; i < m_numRows; ++i)
		{
			const size_t iResult = i*result.m_numColumns;
			const size_t iThis = i*m_numColumns;
			for (size_t j = 0; j < numColsOfOtherMatrix; ++j)
			{
				double& currentEntry = result.m_matrixElements[iResult + j];
				currentEntry = 0.0;
				//result(i, j) = 0;
				/*size_t resultIndex = i*result.m_numColumns + j;
				result.m_matrixElements[resultIndex] = 0;*/

				// Using __restrict gives an hint to the compiler to optimize the use of this array.
				// We tells that the only way we will access this array in this scope will be through this pointer.
				double* __restrict thisMatrixPtr = &(m_matrixElements[iThis]);
				double* __restrict m1MatrixPtr = &(m1.m_matrixElements[j]);
				size_t km1NumColumns = 0;
				for (size_t k = 0; k < m_numColumns; ++k, km1NumColumns += m1NumColumns)
				{
					currentEntry += thisMatrixPtr[k] * m1MatrixPtr[km1NumColumns];
					//result(i, j) += (*this)(i, k) * m1(k, j);
					/*result.m_matrixElements[resultIndex]
					+= m_matrixElements[i*m_numColumns + k] *
					m1.m_matrixElements[k*m1.m_numColumns + j];*/
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------

void Matrix::MultiplyTo(const Matrix& matrix)
{
	Matrix result;
	this->Multiply(matrix, result);
	*this = result;
}

//-----------------------------------------------------------------------------

void Matrix ::ScalarMultiply (double value, Matrix& result)const
{
	//initialize the result matrix to the same dimensions of current matrix
	result.Resize(m_numRows, m_numColumns);

	for(size_t i = 0; i < m_numRows; ++i)
	{
        size_t iResult = i*result.m_numColumns;
        size_t iThis   = i*m_numColumns;
		for(size_t j = 0; j < m_numColumns; ++j)
		{
			result.m_matrixElements[iResult + j] = value * m_matrixElements[iThis + j];
		}
	}
}

//-----------------------------------------------------------------------------

void Matrix::AddTo(const Matrix &matrix)
{
	// Number of rows and columns should be same for the current matrix
	// and input matrix
	assert(m_numRows == matrix.m_numRows && m_numColumns == matrix.m_numColumns);

	for(size_t i = 0; i < m_numRows; ++i)
	{
        size_t iMatrix = i*matrix.m_numColumns;
        size_t iThis   = i*m_numColumns;
		for(size_t j = 0; j < m_numColumns; ++j)
		{
            m_matrixElements[iThis + j] += matrix.m_matrixElements[iMatrix + j];
		}
	}
}

//-----------------------------------------------------------------------------

void Matrix::RemoveColumn(size_t columnIndex)
{
	if(m_numColumns > columnIndex)
	{
		// create temporary matrix with one less column
		Matrix temp(m_numColumns-1, m_numRows);

		// copy all elements of current matrix into the temporary matrix except
		// the elements of the column to be removed
		for(size_t i = 0; i < m_numColumns-1; ++i)
		{
			for(size_t j = 0; j < m_numRows; ++j)
			{
				if(i < columnIndex)
					temp(j,i) = (*this)(j,i);
				else
					temp(j,i) = (*this)(j,i+1);
			}
		}

		// replace the current matrix with the temporary matrix
		copy(temp);
	}
}

//-----------------------------------------------------------------------------

void Matrix::RemoveRow(size_t rowIndex)
{
	if(m_numRows > rowIndex)
	{
		// create temporary matrix with one less row
		Matrix temp(m_numColumns, m_numRows-1);

		// copy all elements of current matrix into the temporary matrix except
		// the elements of the row to be removed
		for(size_t i = 0; i < m_numRows-1; ++i)
		{
			for(size_t j = 0; j < m_numColumns; ++j)
			{
				if(i < rowIndex)
					temp(i,j) = (*this)(i,j);
				else
					temp(i,j) = (*this)(i+1,j);
			}
		}

		// replace the current matrix with the temporary matrix
		copy(temp);
	}
}

//-----------------------------------------------------------------------------

bool Matrix::IsUnitMatrix()const
{
	bool isUnit = true;

	for(size_t i = 0; i < GetNumRows(); ++i)
	{
		for(size_t j = 0; j < GetNumCols(); ++j)
		{
			double value = (*this)(i, j);

			double checkValue = 0;
			if (i == j)
				checkValue = 1;

			if(!(fabs(value-checkValue) < 1e-3))
			{
				isUnit = false;
				break;
			}
		}
	}

	return isUnit;
}

//-----------------------------------------------------------------------------

void Matrix::Dump(Report &report, int tabIndent)const
{
	for(size_t i = 0; i < GetNumRows(); ++i)
	{
		for(size_t j = 0; j < GetNumCols(); ++j)
		{
			report.Add(tabIndent, " %lf \t",(*this)(i, j));
		}
		report.Add(tabIndent, "\n\n");
	}

	report.AddMarker(0);
}
double Matrix::GetAbsoluteMaximumValue()const
{
	long nCount = (long)(m_numRows * m_numColumns);
	double maxVal = 0.0;
	long i = 0;
	for(i = 0; i < nCount; i++)
	{
		if(fabs(m_matrixElements[i]) > maxVal)
			maxVal = fabs(m_matrixElements[i]);
	}
	return maxVal;
}

//-----------------------------------------------------------------------------

void Matrix::PlaceSubMatrix(size_t rowIndex, size_t colIndex, const Matrix& subMatrix)
{
	assert(this != &subMatrix);
	assert(colIndex+subMatrix.m_numColumns <= m_numColumns);
	assert(rowIndex+subMatrix.m_numRows <= m_numRows);

	size_t numRows = subMatrix.m_numRows;
	size_t numCols = subMatrix.m_numColumns;
	for(long i=0; i < numRows; ++i)
	{
		for(long j=0; j < numCols; ++j)
		{
			(*this)(rowIndex+i, colIndex+j) = subMatrix(i, j);
		}
	}
}

//-----------------------------------------------------------------------------

double Matrix::GetAbsoluteMaximumValueInRow(size_t rowIdx) const
{
	assert(IsValidMatrix() == true);
	assert(rowIdx < m_numRows);
/*
	double *pRow = (*this).GetRow(rowIdx);

	double dAbsMaxVal = 0.0;

#ifdef INTEL_BLAS_OPERATIONS
	if( m_numColumns > 0)
	{
		CBLAS_INDEX maxValIdx = ::cblas_idamax(m_numColumns, pRow, 1);
		assert((long)(maxValIdx) >= 0L && (long)(maxValIdx) < m_numColumns);
		dAbsMaxVal  = fabs(pRow[maxValIdx]);
	}
#else

	double *pCell = NULL;
	double val = 0.0;

	long j=0;
	for(j = 0, pCell = pRow; j < m_numColumns; ++j, ++pCell)
	{
		val = fabs(*pCell);
		dAbsMaxVal = HPCAMAX(val, dAbsMaxVal);
	}
//#endif
*/
	double dAbsMaxVal = 0.0;
	double val = 0.0;
	size_t j = 0;
	for(j = 0; j < m_numColumns; j++)
	{
		val = fabs((*this)(rowIdx,j));
		dAbsMaxVal = val > dAbsMaxVal ? val : dAbsMaxVal;
	}

	return(dAbsMaxVal);
}

//-----------------------------------------------------------------------------

void Matrix::scalarMultiplicationForRow(size_t rowIndex, double scalarMultiplier)
{
	assert(IsValidMatrix() == true);
	assert(rowIndex < m_numRows);

	double *pRow = (*this).getRowElements(rowIndex);
	size_t j = 0;
	for(j = 0; j < m_numColumns; ++j)
	{
		pRow[j] *= scalarMultiplier;
	}
}

//-----------------------------------------------------------------------------

void  Matrix::DeleteZeroValueColumns(std::vector<long >& zeroValueColumnIndices, double tolerance)
{
	// Removes Columns with All zero entries....
	zeroValueColumnIndices.clear();

	size_t i = 0,j = 0,elemNo = 0;
	std::deque<bool> flagCols(m_numColumns, true);

	for(i = 0; i< m_numRows; i++)
	{
		for(j = 0; j < m_numColumns; j++)
		{
			if(flagCols[j] && (m_matrixElements[elemNo] < -tolerance || m_matrixElements[elemNo] > tolerance))
				flagCols[j] = false;
			elemNo++;
		}
	}

	size_t numZeroCols = 0;
	for(i=0; i<m_numColumns; i++)
	{
		if(flagCols[i])
		{
			numZeroCols++;
			zeroValueColumnIndices.push_back((long)i);
		}
	}

	for(i = 0; i < numZeroCols; i++)
	{
		long index = zeroValueColumnIndices[numZeroCols - i - 1];
		RemoveColumn(index);
	}
}

//-----------------------------------------------------------------------------

bool Matrix::IsNullMatrix(double tolerance, double *maxDeviation)const
{
	assert(tolerance >= 0.0);

	bool stat = true;

	if(maxDeviation)
		*maxDeviation = 0;

	long count = (long)(m_numRows * m_numColumns);
	for(long i = 0; i < count; i++)
	{
		if(m_matrixElements[i] < -tolerance || m_matrixElements[i] > tolerance)
		{
			stat = false;

			if(!maxDeviation)
				break;
			else
			{
				double val = fabs(m_matrixElements[i]);
				if(*maxDeviation < val)
					*maxDeviation = val;
			}
		}
	}
	return stat;
}

//-----------------------------------------------------------------------------

bool Matrix::CheckSignVariationInMatrix()const
{
	bool positive = false, negetive = false;

	long count = (long)(m_numRows * m_numColumns);
	for(long i = 0; i < count; i++)
	{
		if(m_matrixElements[i] > 0.0)
			positive = true;
		else
			negetive = true;

		if(positive && negetive)
			break;
	}

	return positive && negetive;
}

//-----------------------------------------------------------------------------

void Matrix::ScalarMultiply(double scal)
{
	long num = (long)(m_numRows * m_numColumns);

#ifdef INTEL_BLAS_OPERATIONS
	::cblas_dscal(num, scal, m_pdElements, 1);
#else

	for(long i = 0; i < num ; i++)
	{
		m_matrixElements[i] = scal * m_matrixElements[i];
	}
#endif

}

//-----------------------------------------------------------------------------

bool Matrix::AddRow(size_t nRows)
{
	if(m_numColumns == 0)
		return false;

	size_t nCurRows = m_numRows + nRows;
	size_t nCurCols = m_numColumns;
	//size_t newSize = nCurRows * nCurCols;

	if(this->GetNumRows() < nCurRows)
	{
		double* pElements = new double [nCurRows * nCurCols];
		memset (pElements, 0, nCurRows * nCurCols * sizeof(double));
		memcpy(pElements, m_matrixElements, GetMemSize());
		delete []m_matrixElements;

		m_matrixElements = pElements;
	}

	m_numRows = nCurRows;
	m_numColumns = nCurCols;

	return true;
}

//-----------------------------------------------------------------------------

double Matrix::GetAbsMaxValue()const
{
	size_t nCount = m_numRows * m_numColumns;
	double maxVal = 0.0;

	for(size_t i = 0; i < nCount; i++)
	{
		if(fabs(m_matrixElements[i]) > maxVal)
			maxVal = fabs(m_matrixElements[i]);
	}

	return maxVal;
}

//-----------------------------------------------------------------------------

bool Matrix::FindNextMaxElement(size_t curRow, size_t curCol,
								 size_t& newRow, size_t& newCol, double tolerance)
{
	assert( m_numRows > 0 && m_numColumns > 0);
	assert(curRow < m_numRows);
	assert(curCol < m_numColumns);

	bool bRet = false;
	size_t startIndex = curRow*m_numColumns + curCol;
	double val = fabs(m_matrixElements[startIndex]);
	newRow = newCol = -1;
	if(val > tolerance)
	{
		newRow = curRow;
		newCol = curCol;
		bRet = true;
	}
	for(size_t i = curRow; i < m_numRows; i++)
	{
		for(size_t j = curCol+1; j < m_numColumns; j++)
		{
			double tmpVal = fabs(m_matrixElements[i*m_numColumns + j]);
			if((tmpVal > tolerance) && (tmpVal > val))
			{
				val = tmpVal;
				newRow = i;
				newCol = j;
				bRet = true;
			}
		}
	}
	return bRet;
}

//-----------------------------------------------------------------------------

void Matrix::GetRowMatrix(size_t rowIndex, Matrix& rowMatrix)const
{
	assert(rowIndex < m_numRows);

	// Just allocate the memory for rowMatrix. No need to initialize the memory.
	rowMatrix.Resize(1,m_numColumns);
	::memcpy(rowMatrix.m_matrixElements, m_matrixElements + rowIndex * m_numColumns, m_numColumns*sizeof(double));
}

//-----------------------------------------------------------------------------

void Matrix::SetAsRow(size_t rowIdx, const Matrix& hmatToAdd )
{
	assert(this != &hmatToAdd);

#ifdef _DEBUG
	size_t numCols = hmatToAdd.m_numColumns;
	size_t numRows = hmatToAdd.m_numRows;

	size_t numElements = numCols * numRows;

	assert( numElements == m_numColumns);
	assert( rowIdx < m_numRows);
#endif
	double* pRow = (*this).GetRow(rowIdx);

	// Copy the elements directly into this one row.
	memcpy(pRow, hmatToAdd.m_matrixElements, hmatToAdd.GetMemSize());
}

//-----------------------------------------------------------------------------

void Matrix::GetColumn(size_t colIndex, Matrix& columnMatrix)const
{
	assert(colIndex < m_numColumns);

	columnMatrix.Resize(m_numRows, 1);
	for(size_t i = 0; i < m_numRows; i++)
	{
		columnMatrix(i, 0) = (*this)(i,colIndex);
	}
}

//-----------------------------------------------------------------------------

bool Matrix::SetColumnElements(size_t colIndex,
												const Matrix& colMatrix)
{
	size_t numRows = colMatrix.GetNumRows();
	assert(numRows == m_numRows);

	size_t i;
	for(i = 0; i < numRows; i++)
	{
		(*this)(i, colIndex) = colMatrix(i,0);
	}
	return true;
}

//-----------------------------------------------------------------------------
bool Matrix::GetNextMaxElement(long currentRow, long currentCol, long& newRow, long& newCol, double tolerance)
{
	assert( m_numRows > 0 && m_numColumns > 0);
	assert(currentRow < m_numRows && currentRow >= 0);
	assert(currentCol < m_numColumns && currentCol >= 0);

	bool bRet = false;
	long startIndex = (long)(currentRow*m_numColumns + currentCol);
	double val = fabs(m_matrixElements[startIndex]);
	newRow = newCol = -1;
	if(val > tolerance)
	{
		newRow = currentRow;
		newCol = currentCol;
		bRet = true;
	}
	for(size_t i = currentRow; i < m_numRows; i++)
	{
		for(size_t j = currentCol+1; j < m_numColumns; j++)
		{
			double tmpVal = fabs(m_matrixElements[i*m_numColumns + j]);
			if((tmpVal > tolerance) && (tmpVal > val))
			{
				val = tmpVal;
				newRow = (long)i;
				newCol = (long)j;
				bRet = true;
			}
		}
	}
	return bRet;
}

//-----------------------------------------------------------------------------

void Matrix::SetRowMatrix(size_t rowIndex, const Matrix& rowMatrix )
{
	assert(this != &rowMatrix);
	assert( ( rowMatrix.m_numColumns * rowMatrix.m_numRows ) == m_numColumns);
	assert( rowIndex < m_numRows);

	double* pRow = (*this).getRowElements(rowIndex);

	// Copy the elements directly into this one row.
	memcpy(pRow, rowMatrix.m_matrixElements, rowMatrix.getMemoryOfMatrix());
}

//-----------------------------------------------------------------------------

void Matrix::GetColumnMatrix(size_t colIndex, Matrix& columnMatrix)const
{
	assert(colIndex < m_numColumns);
	assert(colIndex >= 0);
	assert( m_numRows > 0 && m_numColumns > 0);

	columnMatrix.Resize(m_numRows, 1);
	size_t i = 0;
	for(i = 0; i < m_numRows; i++)
	{
		columnMatrix(i, 0) = (*this)(i, colIndex);
	}
}

//-----------------------------------------------------------------------------

bool Matrix::SetColumnMatrix(long colIndex, const Matrix& columnMatrix)
{
	size_t numRows = columnMatrix.GetNumRows();
	assert(numRows == m_numRows);

	size_t i;
	for(i = 0; i < numRows; i++)
	{
		(*this)(i, colIndex) = columnMatrix(i, 0);
	}
	return true;
}

//-----------------------------------------------------------------------------

void Matrix::operator-=(const Matrix& rMatrix)
{
	assert( m_numRows == rMatrix.m_numRows || m_numRows == 0 || rMatrix.m_numRows == 0);
	assert( m_numColumns == rMatrix.m_numColumns || m_numColumns == 0 || rMatrix.m_numColumns == 0);

	if( m_numRows == 0 || m_numColumns == 0)
	{
		Resize(rMatrix.m_numRows, rMatrix.m_numColumns);
	}

	if(rMatrix.IsValidMatrix() == true)
	{
		long nCount = (long)(m_numRows*m_numColumns);
		double* pResult = m_matrixElements;
		const double* pInp1   = rMatrix.m_matrixElements;

		for(; 0 < nCount ; --nCount, ++pResult, ++pInp1)
		{
			*pResult -= (*pInp1);
		}
	}
}

//-----------------------------------------------------------------------------

void Matrix::ZeroRoundOff(double dZeroTol)
{
	assert(dZeroTol >= 0.0);
	double dNegZeroTol = -dZeroTol;

	long nCount = (long)(m_numRows*m_numColumns);
	double* pElem = m_matrixElements;

	double val = 0.0;
	for(long i=0; i< nCount; ++i, ++pElem)
	{
		val = *pElem;
		if( val >= dNegZeroTol && val <= dZeroTol)
		{
			(*pElem) = 0.0;
		}
	}
}

//-----------------------------------------------------------------------------

void Matrix::MakeRowMaxAsUnity(Matrix& matrix,
												const double limitingTol)
{
	double absMaxVal = 0.0;

	size_t rowIdx = 0;
	for(rowIdx = 0; rowIdx < m_numRows; ++rowIdx)
	{
		absMaxVal = GetAbsoluteMaximumValueInRow(rowIdx);
		if( absMaxVal > 0.0)
		{
			double scalFactor = 1.0/absMaxVal;
			scalarMultiplicationForRow(rowIdx, scalFactor);
			matrix.scalarMultiplicationForRow(rowIdx, scalFactor, limitingTol);
		}
	}
}

//-----------------------------------------------------------------------------

void Matrix::scalarMultiplicationForRow(size_t rowIndex, double scalarMultiplier, double limitingValue)
{
	assert(IsValidMatrix() == true);
	assert(rowIndex < m_numRows);
	assert(limitingValue > 0.0);

	double *pRow = (*this).getRowElements(rowIndex);

	long nCount = (long)m_numColumns;

	double neglimitingValue = -limitingValue;
	double val = 0.0;

	for(; 0 < nCount ; --nCount, ++pRow)
	{
		val = *pRow;
		if(  val > neglimitingValue && val < limitingValue)
		{
			(*pRow) *= scalarMultiplier;
		}
	}
}

//-----------------------------------------------------------------------------

double Matrix::GetMaximumValueInMatrix()const
{
	double maxVal = -DBL_MAX;
	long count = (long)(m_numRows * m_numColumns);
	for(long i = 0; i < count; i++)
	{
		if(m_matrixElements[i] > maxVal)
			maxVal = m_matrixElements[i];
	}
	return maxVal;
}

//-----------------------------------------------------------------------------

bool Matrix::IsPositiveMatrix(double tolerance, double *maxDeviation,
									   int* rowIndex )const
{
	bool stat = true;

	if(maxDeviation)
		*maxDeviation = 0;

	long count = (long)(m_numRows * m_numColumns);
	for(long i = 0; i < count; i++)
	{
		if(m_matrixElements[i] < -tolerance)
		{
			stat = false;
			if(!maxDeviation)
				break;
			else
			{
				if(*maxDeviation < -m_matrixElements[i]) //the element has to be negative here..
				{
					if(rowIndex)
						*rowIndex = i;
					*maxDeviation = -m_matrixElements[i];
				}
			}
		}
	}
	return stat;
}

//-----------------------------------------------------------------------------

long Matrix::GetNextMaxElementIndexInColumn(size_t rowIndex, size_t colIndex, double tolerance)const
{
	assert( m_numRows > 0 && m_numColumns > 0);
	assert(rowIndex < m_numRows);
	assert(colIndex < m_numColumns);
	assert(tolerance > 0.0);

	long startIndex = (long)(rowIndex*m_numColumns + colIndex);
	double val = fabs(m_matrixElements[startIndex]);

	long index = (long)rowIndex;

	if( val < tolerance)
	{
		index = -1;
		val = tolerance;
	}

	double negVal = -val;

	// NOTE :  I am using posVal/negVal as seperate variables to avoid too many
	// calls to fabs()

	for(size_t i = rowIndex+1; i < m_numRows; i++)
	{
		double tmpVal = fabs(m_matrixElements[i*m_numColumns + colIndex]);

		if(tmpVal < negVal || tmpVal > val)
		{
			val = fabs(tmpVal);
			negVal = -val;
			index = (long)i;
		}
	}

	return index;
}

//-----------------------------------------------------------------------------

long Matrix::GetNextMaxElementIndexInRow(size_t rowIndex, size_t colIndex, double tolerance)const
{
	assert( m_numRows > 0 && m_numColumns > 0);
	assert(rowIndex < m_numRows);
	assert(colIndex < m_numColumns);

	long startIndex = (long)(rowIndex*m_numColumns);
	double val = fabs(m_matrixElements[startIndex+colIndex]);
	long index = (long)colIndex;

	if( val < tolerance)
	{
		index = -1;
		val = tolerance;
	}

	// NOTE :  I am using posVal/negVal as seperate variables to avoid too many
	// calls to fabs()

	double negVal = -val;

	for(size_t i = colIndex+1; i < m_numColumns; i++)
	{
		double tmpVal = fabs(m_matrixElements[startIndex+i]);

		if(tmpVal < negVal || tmpVal > val)
		{
			val = fabs(tmpVal);
			negVal = -val;
			index = (long)i;
		}
	}
	return index;
}

}
