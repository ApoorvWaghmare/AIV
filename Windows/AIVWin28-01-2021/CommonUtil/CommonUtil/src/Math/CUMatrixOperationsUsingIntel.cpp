// math include files
#include "CUMatrixOperationsUsingIntel.h"
#include "CUMatrix.h"

#include "MKLWrapper.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------
void MatrixOperationsUsingIntel::multiply(bool bTransA, const Matrix& rMatA, 
								   bool bTransB, const Matrix& rMatB,
								   Matrix& result)
{
	PMK_INT matANumRows = (PMK_INT)rMatA.GetNumRows();
	PMK_INT matANumCols = (PMK_INT)rMatA.GetNumCols();
	PMK_INT matBNumRows = (PMK_INT)rMatB.GetNumRows();
	PMK_INT matBNumCols = (PMK_INT)rMatB.GetNumCols();
		
	if(bTransA && bTransB)
		result.Resize((size_t)matANumCols, (size_t)matBNumRows);
	else if(bTransA)
		result.Resize((size_t)matANumCols, (size_t)matBNumCols);
	else if(bTransB)
		result.Resize((size_t)matANumRows, (size_t)matBNumRows);
	else
		result.Resize((size_t)matANumRows, (size_t)matBNumCols);

	const double* matA = rMatA.GetPointerToElements();
	const double* matB = rMatB.GetPointerToElements();

	double* resultElements = result.GetPointerToElements();

	MultiplyMatrices(matANumRows, matANumCols, matBNumRows, matBNumCols, 
						bTransA, matA, bTransB, matB, resultElements);
}

//-----------------------------------------------------------------------------

bool MatrixOperationsUsingIntel::getInverseMKL(const Matrix& input, Matrix& inverseMatrix) 
{
	inverseMatrix = input;
	PMK_INT numRowsm = (PMK_INT)inverseMatrix.GetNumRows();
	double* LU = inverseMatrix.GetPointerToElements();
	
	return GetInverseMatrix(numRowsm, LU);
}

//-----------------------------------------------------------------------------

}