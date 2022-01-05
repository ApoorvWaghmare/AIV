#include "CUCostElement.h"


namespace CommonUtil
{

//-----------------------------------------------------------------------------

void CostElement::Multiply(double scaler)
{
	size_t numRows = m_QMatrix.GetNumRows();
	size_t numCols = m_QMatrix.GetNumCols();

	// scalar multiply to Q
	int i = 0;
	for(i = 0; i < numRows; i++ )
		for(int j = 0; j < numCols; j++)
			m_QMatrix(i, j) = scaler * m_QMatrix(i, j);

	// scalar multiply to C
	numRows = m_CMatrix.GetNumRows();
	numCols = m_CMatrix.GetNumCols();

	for(i = 0; i < numRows; i++ )
		for(int j = 0; j < numCols; j++)
			m_CMatrix(i, j) = scaler * m_CMatrix(i, j);

	// scalar multiply to constant
	m_constant = scaler* m_constant;
}

//-----------------------------------------------------------------------------

}