// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_SPARSEMATRIX_H_
#define _COMMONUTIL_SPARSEMATRIX_H_

// standard include files
#include <vector>
#include <algorithm>

namespace CommonUtil
{

// Forward declarations
class SqMatrix;
class Matrix;
//class EProgressAnalyser;
class Report;

// This class represents row indexed sparse matrix
class SparseMatrix
{

	public:
	struct Element
	{
		int m_row;
		int m_col;
		double m_val;
	};

	//========================== private data members =========================

	//This is vector of Non-Zero Elements
	std::vector<Element > m_matElements;

	// This is the row indexed sparse matrix index
	std::vector<size_t> m_sparseIndices;

	// This is non-zero element if offdiagonal element 
	// This elemnt may be zero in case of diagonal element
	std::vector<double> m_sparseElements;

	//EProgressAnalyser* m_progressAnalyser;

	// This method creates sparse matrix from given square matrix
	//
	//
	//	Input Square Matrix
	//  --				 --
	// | 3	0	1	0	0 |
	// |				  |
	// | 0	4	0	0	0 |
	// |				  |
	// | 0	7	5	9	0 |
	// |				  |
	// | 0	0	0	0	2 |
	// |				  |
	// | 0	0	0	6	5 |
	//  --				 --
	//
	//
	//Output(sparse matrix)( by row indexing)
	//================================================================
	// index k				0	1	2	3	4	5	6	7	8	9	10	
	//================================================================
	// m_sparseIndices[k]	6	7	7	9	10	11	2	1	3	4	3	
	//================================================================
	// m_sparseElements[k]	3	4	5	0	5	x	1	7	9	2	6
	//================================================================
	

	//======================= private members functions =======================

	//This method converts given square matrix to sparse matrix
	void convertToSparseMatrix(const SqMatrix &matrix);

	//This method computes one of two norms for minimizer vector
	double computeNormForMinimizerVector(const std::vector<double> &minimizers,
										 const int iterationTol)const;
	
	//This method computes the vector of residuals  for given array of minimizers
	void computeProdOfAOrATrans(const std::vector<double> &minimizers, 
								bool isSparseTranMultiply, 
								std::vector<double > &residuals)const;

	void preconditionerSolver(const std::vector<double > &rhsVals, 
							  std::vector<double > &minimizers,
							  bool isPreconditionerMatrix)const;

	//This method computes the preconditioner matrix from given sparse matrix
	void computePreconditionerMatrix(SparseMatrix &preconditionerMatrix)const;

	int getNumLocations(const SqMatrix& matrix, double zero)const;

	void convertToSparseMatrix(int numRows,
										const std::vector<Element > &matElements);

	void getNumLocations(int &numLocations, 
						const std::vector<Element > &matElements, double zero)const;

	void getStartRowIndices(int numRows,
				                      const std::vector<Element > &matElements,
									  std::vector<int> &startRowIds)const;
    

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//Default constructor
	SparseMatrix()
	{
		//m_progressAnalyser = 0;
	}

	// Constructor with square matrix
	SparseMatrix(const SqMatrix &matrix);

	SparseMatrix(int numRows, const std::vector<Element > &matElements);
       


	//****************************** destructor *******************************

	~SparseMatrix()
	{}

	//*************************** get/set methods *****************************

	//Sets sparse indices
	void SetSparseIndices(const std::vector<size_t> &sparseIndices)
	{
		m_sparseIndices = sparseIndices;	
	}

	void SetSparseElements(const std::vector<double> &sparseElements)
	{
		m_sparseElements = sparseElements;
	}

	void Create(int numRows, const std::vector<Element > &matElements)
	{
		convertToSparseMatrix(numRows, matElements);
	}
	//**************************general methods********************************

	// This method multiplies the column matrix ((m_sparseIndices[0] - 1) x 1)
	// [Sparse Matrix][columnMat] = [outputMat]
	void MultiplyByColumnNatrix(const Matrix &inputColMatrix, 
								Matrix &outputMatrix)const;

	// This method multiplies sparse matrix transpose 
	// with the column matrix ((m_sparseIndices[0] - 1) x 1)
	// [Sparse MatrixTrans][columnMat] = [outputMat]
	void MultiplySparseMatrixTransposeByColumnNatrix(
											const Matrix &inputColMatrix, 
											Matrix &outputMatrix)const;

	//  This method computes the transpose of the row indexed sparse matrix
	//[aij]		= [aji]
	//	   nxn         nxn  
	void ComputeTransposeMatrix(SparseMatrix &transpose)const;

	//This method computes the elements at given sparse locations
	//Sparse locations are fixed for multMatrix
	void MultipllicationForSpecifiedSparseLocations(
											const SparseMatrix &otherSparseMat, 
											SparseMatrix &multMatrix)const; 

	//[A]	*	[B] = [C]
	//   nxn	   nxn	 nxn
	// This method computes all the elements of sparse multiplication
	void Multiply(const SparseMatrix &otherSparseMatrix,
				  SparseMatrix &multMatrix)const; 

	//This method solves linear system having sparsity.
	void SolveSparseLinearSystem(const std::vector<double > &rhsVals, 
								 const int iterTol, const double tol, 
								 const int iterMax, int &iter, 
								 std::vector<double > &variableVals,
								 bool* isProcessBreak = 0)const;

	// dumps sparse mjatrix
	void Dump(Report &report, int tabIndent)const;

	void DumpInSqrMatrixForm(Report &report, int tabIndent)const;

	void SetProgressAnalyser(/*EProgressAnalyser* progressAnalyser*/)
	{
		//m_progressAnalyser = progressAnalyser;
	}
	
	//Added for Morphing-II
	//This method returns the value from sparse matrix which 
	//is logicaly belongs to square matrix
	double GetValueFromSqureMatrixIndices(int sqrMatRow, int sqrMatCol)const;
	//This function returns the value of determinant of the sparse matrix
//	double computeDeterminant()const;

	//This method computes the inverse of the sparse matrix
//	bool getInverseMatrix(SparseMatrix &inverseMatrix)const;

	//This method computes the eigen values of the sparse matrix
//	void computeEigenValues(std::vector<double > &eigenValues)const;

	//This method computes the eigen vector for the sparse matrix 
	//at given eigen value
//	void computeEigenVector(const double eigenValue, Vector &eigenVector)const;

	// Returns an element in ith row and jth column
	// this method is called for lvalue
//	double operator()(int i, int j)const;

	//This method creates square matrix from sparse matrix
//	void createSqMatrix(SqMatrix &sqMatrix)const;

	void GetSqrMatrix(SqMatrix &sqrMatrix)const;

};

}
#endif