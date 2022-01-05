// ============================================================================
//			Copyright (c) 2008 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_MATRIX_RANK_COMPUTER_H
#define _COMMONUTIL_MATRIX_RANK_COMPUTER_H

#include <vector>

#include "CUMatrix.h"

namespace CommonUtil
{

class MatrixRankComputer
{
	const Matrix &m_coeffMatrix;

	Matrix m_reducedMatrix;

	std::vector<size_t> m_swapColumnIdMap;

	size_t m_numRowsCheckedForRank;

	void gaussJordenOperation(Matrix& lhs, 
								Matrix& rhs);

	void swapRows(Matrix& matrix, size_t index1, size_t index2);

	void swapColumns(Matrix& matrix, size_t index1, size_t index2);

	bool modifyRows(size_t currentElementIndex, Matrix& aMatrix, double tolerance);

public:

	MatrixRankComputer(const Matrix &m_coeffMatrix);

	~MatrixRankComputer();

	size_t GetRank();
	
};
}
#endif



