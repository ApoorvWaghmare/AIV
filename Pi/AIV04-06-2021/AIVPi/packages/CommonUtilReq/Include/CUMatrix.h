// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_MATRIX_H_
#define _COMMONUTIL_MATRIX_H_

// standard include files
#include <vector>
#include <assert.h>
#include <stdlib.h>


namespace CommonUtil
{

// Forward declarations
class Report;

// This class represents mathematical matrix
class Matrix
{
	//========================== private data members =========================

	// Two dimenstional array of matrix elements
	// we are not using two dimensional std::vector here because we have found
	// a large performance penalty
	double *m_matrixElements;

	// Rows of matrix
	size_t m_numRows;

	// Columns of matrix
	size_t m_numColumns;


	//======================= private members functions =======================

	// The function to make copy of given matrix
	void copy(const Matrix &matrix);

	//************************************
	//These methods are copied from OptimizerMatrix
	size_t getMemoryOfMatrix() const
	{
		return(m_numRows*m_numColumns*sizeof(double));
	}

	const double* getRowElements(size_t rowIndex)const
	{
		assert(rowIndex < m_numRows);
		return ( m_matrixElements + rowIndex*m_numColumns);
	}

	double* getRowElements(size_t rowIndex)
	{
		assert(rowIndex < m_numRows);
		return ( m_matrixElements + rowIndex*m_numColumns);
	}

	void scalarMultiplicationForRow(size_t rowIdx, double scalarMultiplier);

	void scalarMultiplicationForRow(size_t rowIdx, double scalarMultiplier,
						double limitingValue);
	//************************************

	int GetRowIdForIndex(size_t rowIndex);

	size_t GetRowIndexForGivenRowID(int rowID);

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Matrix();

	// Creates an empty matrix of given number of rows and columns
	// all the matrix elements are set to zero
	Matrix(size_t colSize, size_t rowSize);

	// Construct matrix using single dimensional array and column size
	Matrix(size_t colSize, const std::vector<double>& values);

	// Copy constructor
	Matrix (const Matrix &matrix);

	//****************************** destructor *******************************

	// Destructor
	~Matrix();

	//*************************** get/set methods *****************************

	// Returns the number of rows
	size_t GetNumRows()const
	{
		return m_numRows;
	}

	// Returns the number of columns
	size_t GetNumCols()const
	{
		return m_numColumns;
	}

	// Returns an element in ith row and jth column
	// this method is called for lvalue
	double& operator()(size_t i, size_t j)
	{
		assert(i < m_numRows && j < m_numColumns);
		return m_matrixElements[i*m_numColumns + j];
	}

	// Returns the value of an element in ith row and jth column
	// this method is called as rvalue
	double operator()(size_t i, size_t j)const
	{
		assert(i < m_numRows && j < m_numColumns);
		return m_matrixElements[i*m_numColumns + j];
	}

	// Returns the transpose
	void GetTranspose(Matrix &transpose)const;

	//**************************general methods********************************

	// Eequal to operator
	const Matrix &operator = (const Matrix &matrix);

	// Sets the size of matrix
	void Resize(size_t newNumRows, size_t newNumCols);

	// Adds a matrix to the current matrix and returns the result
	void Add (const Matrix& m1, Matrix& result)const;

	// Subtracts a matrix from the current matrix and returns the result
	void Subtract(const Matrix& m1, Matrix& result)const;

	// Multiplies a matrix to the current matrix and returns the result
	void Multiply(const Matrix& m1, Matrix& result)const;

	// Multiplies a scalar to the current matrix and returns the result
	void ScalarMultiply(double value, Matrix& result)const;

	//Multiplies a scalar to the current matrix and modifies current matrix
	void ScalarMultiply(double scal);

	// Adds a matrix to current matrix
	void AddTo(const Matrix& matrix);

	// Dumps the matrix information
	void Dump(Report &report, int tabIndent)const;

	// Returns a bool value true, if matrix is unit matrix
	bool IsUnitMatrix()const;

	// Multiplies a matrix to current matrix
	void MultiplyTo(const Matrix& matrix);

	// Removes a particular column indicated by column number
	void RemoveColumn(size_t columnIndex);

	// Removes a particular row indicated by row number
	void RemoveRow(size_t rowIndex);

	// Add row to matrix
	bool AddRow(size_t nRows);

	size_t GetMemSize() const
	{
		return(m_numRows*m_numColumns*sizeof(double));
	}

	double GetAbsMaxValue()const;

	bool FindNextMaxElement(size_t curRow, size_t curCol,
				 size_t& newRow, size_t& newCol, double tolerance);

	void GetRowMatrix(size_t rowIndex, Matrix& rowMatrix)const;

	void SetAsRow(size_t rowIdx, const Matrix& hmatToAdd );

	inline double* GetRow(size_t i) const
	{
		assert(i < m_numRows);
		return ( m_matrixElements + i*m_numColumns);
	}

	void GetColumn(size_t colIndex, Matrix& columnMatrix)const;

	bool SetColumnElements(size_t colIndex,
						const Matrix& colMatrix);

	//************************************
	//Following methods are copied from OptimizerMatrix
	// Deletes a particular row indicated by row number
	double GetAbsoluteMaximumValue()const;

	//Place submtrix if rowIndex and colIndex is known
	//		  |A D G|
	//e.g M = |B E H| let Matrix M created from Submatrices A,B,C,D..
	//		  |C F I|  so these matrices are placed if rowIndex and colIndex are known
	void PlaceSubMatrix(size_t rowIndex, size_t colIndex, const Matrix& subMatrix);

	double GetAbsoluteMaximumValueInRow(size_t rowIdx) const;

	//Cecks matrix is valid or not
	//If for a matrix numRows or numCols = 0 then matrix is inValid
	bool IsValidMatrix()const
	{
		return ( ( (m_numRows == 0) || (m_numColumns == 0) ) ? false : true );
	}

	//deletes row faving all elements zero(zero is value which is less tahn tolerance given)
	void DeleteZeroValueColumns(std::vector<long >& zeroValueColumnIndices, double tolerance);

	// checks whether NULL matrix or NON-NULL
	bool IsNullMatrix(double tolerance, double *maxDeviation = nullptr)const;

	bool CheckSignVariationInMatrix()const;

	bool GetNextMaxElement(long currentRow, long currentCol, long& newRow, long& newCol, double tolerance);

	//Sets row matrix is rowIndex is known
	void SetRowMatrix(size_t rowIndex, const Matrix& rowMatrix );

	//Sets col matrix is colIndex is known
	bool SetColumnMatrix(long colIndex, const Matrix& colMatrix);

	void GetColumnMatrix(size_t colIndex, Matrix& columnMatrix)const;

	void operator-=(const Matrix& rMatrix);

	void ZeroRoundOff(double dZeroTol);

	// gives ith row elements
	inline double* operator[](long i)const
	{
		//assert( i >= 0 && i < m_numRows);
		return ( m_matrixElements + i*m_numColumns );
	}
	void MakeRowMaxAsUnity(Matrix& matrix, const double limitingTol);

	// returns maximum value of the matrix
	double GetMaximumValueInMatrix()const;

	bool IsPositiveMatrix(double tolerance, double *maxDeviation, int* rowIndex = 0)const;

	// Use with care. because it gives all matrix elements outside
	// there are chances of modifing or deleting outside
	operator double*(void) { return m_matrixElements; }

	long GetNextMaxElementIndexInColumn(size_t rowIndex, size_t colIndex, double tolerance)const;

	long GetNextMaxElementIndexInRow(size_t rowIndex, size_t colIndex, double tolerance)const;

	const double* GetPointerToElements()const
	{
		return m_matrixElements;
	}

	// Use with care. because it gives all matrix elements outside
	// there are chances of modifing or deleting outside
	double* GetPointerToElements()
	{
		return m_matrixElements;
	}
};

}
#endif
