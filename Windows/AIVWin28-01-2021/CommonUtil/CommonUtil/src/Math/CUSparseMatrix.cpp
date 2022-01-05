// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//math include files
#include "CUSparseMatrix.h"
#include "CUSqMatrix.h"
#include "CUMatrix.h"
#include "CUCommonConstants.h"
//#include "EssenceProgressEvent.h"
//util include files
#include "CUReport.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

SparseMatrix::SparseMatrix(const SqMatrix &matrix)
{
	convertToSparseMatrix(matrix);
//	m_progressAnalyser = 0; 

}

//-----------------------------------------------------------------------------

void SparseMatrix::convertToSparseMatrix(const SqMatrix &matrix)
{
	size_t numCols = matrix.GetNumCols();
	double zero = 1e-14;//for fit accurate curve
	int numLocations = getNumLocations(matrix, zero);

	m_sparseIndices.resize(numLocations);
	m_sparseElements.resize(numLocations);

	m_sparseIndices[0] = numCols +1;

	size_t k = numCols;
	for(int i = 0; i < numCols; i++)
	{
		m_sparseElements[i] = matrix(i, i);
		for(int j = 0; j < numCols; j ++)
		{
			if(i == j)
				continue;
			double element = matrix(i, j);
			if(fabs(element) > zero)
			{
				++k;
				m_sparseElements[k] = element;
				m_sparseIndices[k] = j;
			}

		}
		m_sparseIndices[i+ 1] = k + 1;
	}

	//1024 is junk val
	m_sparseElements[numCols] = 1024.;

}

//-----------------------------------------------------------------------------

int SparseMatrix::getNumLocations(const SqMatrix& matrix, double zero)const
{
	int numLocations = 0;
	size_t numRows = matrix.GetNumRows();
	for(int i = 0; i < numRows; i++)
	{	
		for(int j = 0; j < numRows; j++)
		{
			if(i == j)
				numLocations++;

			else if(fabs(matrix(i,j)) > zero)
				numLocations++;
		}
	}
	
	numLocations++;

	return numLocations;
}

//-----------------------------------------------------------------------------
SparseMatrix:: SparseMatrix(int numRows,const std::vector<Element > &matElements)
{
	convertToSparseMatrix(numRows, matElements);
	//m_progressAnalyser = 0;
}
//-----------------------------------------------------------------------------
void SparseMatrix::convertToSparseMatrix(int numRows,
										const std::vector<Element > &matElements)
{
	//printf ("convertToSparseMatrix started\n");
	double zero = 1e-8;
	int numLocations = numRows;
	getNumLocations(numLocations, matElements,zero);

	m_sparseIndices.resize(numLocations, 0);
	m_sparseElements.resize(numLocations, 0.0);

	m_sparseIndices[0] = numRows +1;
	size_t k = numRows;
	std::vector<int> stRowIndices;
	getStartRowIndices(numRows,matElements,stRowIndices);
	int curRowIndex = 0;
	int nextRowIndex = 0;
	for(int i = 0; i < numRows; i++)
	{
		//if(i % 1000 == 0)
		//	printf ("GetRow %d started\n", i);

		//double val = 0.0;

		//matrix.GetRow(i, row, 0);
		//matrix.GetRow(i, curRowIndex, row);
		//matrix.GetNextRowIndex(i,curRowIndex,nextRowIndex);
		//curRowIndex += (int)row.size();
		//printf ("GetRow %d completed\n", i);
		curRowIndex = stRowIndices[i];
		if(i != (stRowIndices.size()-1))
			nextRowIndex = stRowIndices[i+1];
		else
			nextRowIndex = (int)matElements.size();
		for( ; curRowIndex < nextRowIndex; curRowIndex++)
		{
			double elementVal = matElements[curRowIndex].m_val;
			int j = matElements[curRowIndex].m_col;
			if (j == i)
			{
				m_sparseElements[i] = elementVal;
			}
			else if(fabs(elementVal) > zero)
			{
				k++;
				m_sparseElements[k] = elementVal;
				m_sparseIndices[k] = j;
			}
		}
		curRowIndex = nextRowIndex;
		//printf ("Row %d conversion 99% completed\n", i);
		m_sparseIndices[i+1] = k + 1;
		//printf ("Row %d conversion completed\n", i);
	}

	//1024 is junk val
	m_sparseElements[numRows] = 1024.;
	/*Report matrixDump("C:\\temp\\sparseMatrix.txt");
	Dump(matrixDump,0);*/
	//printf ("convertToSparseMatrix completed\n");
}
//-----------------------------------------------------------------------------
void SparseMatrix::getNumLocations(int &numLocations, 
						const std::vector<Element > &matElements,double zero)const
{
	int numElements = (int)matElements.size();
	for(int i = 0; i < numElements; i++)
	{
		const Element& element = matElements[i];
		if (element.m_col != element.m_row && fabs(element.m_val) > zero)
			numLocations++;
	}

	numLocations++;
	//return numLocations;
}
//-----------------------------------------------------------------------------
void SparseMatrix::getStartRowIndices(int numRows,
				                      const std::vector<Element > &matElements,
									  std::vector<int> &startRowIds)const
{
	//assumption: Every row has at least one element
	int stIndex = 0;

	startRowIds.resize(numRows);
	for(int i = 0; i < numRows; i++)
	{
		startRowIds[i] = stIndex;
		while (stIndex < matElements.size() &&
			matElements[stIndex].m_row == i)
		{
			stIndex++;
		}
	}
}
//-----------------------------------------------------------------------------
void SparseMatrix::MultiplyByColumnNatrix(const Matrix &inputColMatrix, 
								          Matrix &outputMatrix)const
{
	size_t n = inputColMatrix.GetNumRows();

	if((m_sparseIndices[0] != n+1) && (inputColMatrix.GetNumCols() != 1))
		assert(0);

	outputMatrix.Resize(n, 1);

	for(int i = 0; i < n; i++)
	{
		outputMatrix(i, 0) = m_sparseElements[i]*inputColMatrix(i, 0);
		for(size_t k = m_sparseIndices[i]; k < m_sparseIndices[i+1]; k++)
			outputMatrix(i, 0) += m_sparseElements[k] *
									inputColMatrix(m_sparseIndices[k], 0);
	}
}

//-----------------------------------------------------------------------------

void SparseMatrix::MultiplySparseMatrixTransposeByColumnNatrix(
						const Matrix &inputColMatrix, Matrix &outputMatrix)const
{
	size_t i, j, k;
	size_t n = inputColMatrix.GetNumRows();

	//check for num rows and cols
	if((m_sparseIndices[0] != n+1) && (inputColMatrix.GetNumCols() != 1))
		assert(0); // mismatched column matrix

	outputMatrix.Resize(n, 1);

	// loop for diagonal elements
	for(i = 0; i < n; i++)
		outputMatrix(i, 0) = m_sparseElements[i]*inputColMatrix(i, 0);

	for(i = 0; i < n; i ++)
	{
		for(k = m_sparseIndices[i]; k < m_sparseIndices[i+1]; k++)
		{ 
			j = m_sparseIndices[k];
			outputMatrix(j, 0) += m_sparseElements[k]*inputColMatrix(i, 0);
		}
	}
}

//-----------------------------------------------------------------------------

void SparseMatrix::ComputeTransposeMatrix(SparseMatrix &transpose)const
{
	std::vector<size_t > sparseIndicesB;
	std::vector<double > sparseElementsB_vec;

	sparseIndicesB.resize(m_sparseIndices.size()); 
	sparseElementsB_vec.resize(m_sparseIndices.size());
	sparseElementsB_vec[0] = m_sparseElements[0];
	size_t k = sparseIndicesB[0] = m_sparseIndices[0];
	for(int i = 1; i < sparseIndicesB[0]; i++)
	{
		int index = i-1;
		for(size_t j = m_sparseIndices[0]; j < m_sparseIndices.size(); j++)
		{
			if(m_sparseIndices[j] == index)
			{
				sparseElementsB_vec[k] = m_sparseElements[j];
				k++;
				for(int m = 0; m < m_sparseIndices[0]; m++)
				{
					if(j < m_sparseIndices[m])
					{
						sparseIndicesB[k-1] = m-1;
						break;
					}
				}
			}
		}

		sparseIndicesB[i] = k;
		sparseElementsB_vec[i] = m_sparseElements[i];

	}
	transpose.m_sparseElements = sparseElementsB_vec;
	transpose.m_sparseIndices = sparseIndicesB;
}

//-----------------------------------------------------------------------------

void SparseMatrix::MultipllicationForSpecifiedSparseLocations(
											const SparseMatrix &otherSparseMat, 
                                            SparseMatrix  &multMatrix)const
{
	multMatrix.m_sparseElements.resize(multMatrix.m_sparseIndices.size());
	SparseMatrix otherSparseMatrix;
	otherSparseMat.ComputeTransposeMatrix(otherSparseMatrix);

	size_t i, ijma, ijmb, j, m, ma, mb, mbb, mn;
	double sum;

	if(m_sparseIndices[0] != otherSparseMatrix.m_sparseIndices[0] || 
	   m_sparseIndices[0] != multMatrix.m_sparseIndices[0])
	   assert(0);

	//Loops over rows
	for(i = 0; i < multMatrix.m_sparseIndices[0]-1; i++)
	{
		//Set up so that first pass through loop does the diagonal components
		m = i + 1;
		j = m - 1;
		mn = multMatrix.m_sparseIndices[i];
		sum = m_sparseElements[i]*otherSparseMatrix.m_sparseElements[i];
		
		//Main loop over each component to be output
		for(;;)
		{
			mb = otherSparseMatrix.m_sparseIndices[j];

			//Loop through elements in A's row. Convolute
			for(ma = m_sparseIndices[i]; ma < m_sparseIndices[i+1]; ma++)
			{
				ijma = m_sparseIndices[ma];
			
				if(ijma == j )
					sum += m_sparseElements[ma]*
										otherSparseMatrix.m_sparseElements[j];
			
				else
				{
					while(mb < otherSparseMatrix.m_sparseIndices[j+1])
					{
						ijmb = otherSparseMatrix.m_sparseIndices[mb];
						if(ijmb == i)
						{
							sum += m_sparseElements[i]*
									otherSparseMatrix.m_sparseElements[mb++];
							continue;
						}
						else if(ijmb < ijma)
						{
							mb++;
							continue;
						}
						else if(ijmb == ijma)
						{
							sum += m_sparseElements[ma]*
									otherSparseMatrix.m_sparseElements[mb++];
							continue;
						}

						break;
					}
				}
			}

			//Exahust the remainder of otherSparseMatrix row
			for(mbb = mb; mbb < otherSparseMatrix.m_sparseIndices[j+1]; mbb++)
			{
				if(otherSparseMatrix.m_sparseIndices[mbb] == i)
					sum += m_sparseElements[i]*
										otherSparseMatrix.m_sparseElements[mbb];
			}

			multMatrix.m_sparseElements[m-1] = sum;

			//Reset indices for next pass through loop
			sum = 0.;

			if(mn >= multMatrix.m_sparseIndices[i+1])
				break;

			j = multMatrix.m_sparseIndices[(m = ++mn) - 1];
		}

	}
}

//-----------------------------------------------------------------------------

void SparseMatrix::Multiply(const SparseMatrix &otherSparseMatrix,
							SparseMatrix &multMatrix)const
{
	SparseMatrix transMatrix;
	otherSparseMatrix.ComputeTransposeMatrix(transMatrix);

	size_t i, ijma, ijmb, j, k, ma, mb, mbb;
	double sum;

	//checks the numcols of both sparse matrix is same or not
	if(m_sparseIndices[0] != transMatrix.m_sparseIndices[0])
		assert(0);


	size_t tmpIndex = 0;
	size_t nmax = (m_sparseIndices[0] -1)* (m_sparseIndices[0] -1);//multMatrix.m_sparseElements.size();
	multMatrix.m_sparseElements.resize(nmax);
	multMatrix.m_sparseIndices.resize(nmax);
	multMatrix.m_sparseIndices[0] = k = m_sparseIndices[0];

	//Loop over rows of (*this) matrix 
	for(i = 0; i < m_sparseIndices[0]; i++)
	{
		//Loop over rows of otherMatrix
		for(j = 0; j < transMatrix.m_sparseIndices[0] - 1; j++)
		{
			if(i == j)
				sum = m_sparseElements[i]*transMatrix.m_sparseElements[j];
			else 
				sum = 0.0e0;

			mb = transMatrix.m_sparseIndices[j];

			//Loop through elements in A's row
			//Convoluted logic, following, accounts for the various 
			//combinations of diagonal and off-diagonal elements
			for(ma = m_sparseIndices[i]; ma < m_sparseIndices[i+1]; ma++)
			{
				ijma = m_sparseIndices[ma];
				if(ijma == j)
					sum += m_sparseElements[ma]*transMatrix.m_sparseElements[j];
				else
				{
					while(mb < transMatrix.m_sparseIndices[j+1])
					{
						ijmb = transMatrix.m_sparseIndices[mb];
						if(ijmb == i)
						{
							sum += m_sparseElements[i]*
											transMatrix.m_sparseElements[mb++];
							continue;
						}

						else if(ijmb < ijma)
						{
							mb++;
							continue;
						}

						else if(ijmb == ijma)
						{
							sum += m_sparseElements[ma]*
											transMatrix.m_sparseElements[mb++];
							continue;
						}
						break;

					}
				}
			}

			for(mbb = mb; mbb < transMatrix.m_sparseIndices[j+1]; mbb++)
			{
				if(transMatrix.m_sparseIndices[mbb] == i)
					sum += m_sparseElements[i]*
											transMatrix.m_sparseElements[mbb];
			}

			if(i == j)
				multMatrix.m_sparseElements[i] = sum;
			else if(fabs(sum) > CommonConstants::ZERO)
			{
				if(k > nmax - 1)
					assert(0);
				multMatrix.m_sparseElements[k] = sum;
				multMatrix.m_sparseIndices[k++] = j;
				if(tmpIndex < k)
					tmpIndex = k;
			}

		}

			multMatrix.m_sparseIndices[i+1] = k;

	}

	multMatrix.m_sparseElements.resize(tmpIndex);
	multMatrix.m_sparseIndices.resize(tmpIndex);

}

//-----------------------------------------------------------------------------

double SparseMatrix::computeNormForMinimizerVector(
										const std::vector<double> &minimizers,
										const int iterationTol)const
{
	int i, isamax;
	double ans;

	size_t n = minimizers.size();
	if(iterationTol <= 3)
	{
		ans = 0.;

		//Vector magnitude norm
		for(i = 0; i < n; i++)
			ans += minimizers[i]*minimizers[i];
		return sqrt(ans);
	}

	else
	{
		isamax = 0;
		
		//Largest component norm
		for(i = 0; i < n; i++)
		{
			if(fabs(minimizers[i]) > fabs(minimizers[isamax]))
				isamax = i;
		}

		return (fabs(minimizers[isamax]));
	}
}

//-----------------------------------------------------------------------------

void SparseMatrix::computeProdOfAOrATrans(const std::vector<double> &minimizers, 
										  bool isSparseTranMultiply,
										  std::vector<double > &residuals)const
{
	Matrix minimizersMatrix(1, minimizers); 
	Matrix residualMatrix(1, m_sparseIndices[0] - 1);
	if(isSparseTranMultiply)
		MultiplySparseMatrixTransposeByColumnNatrix(minimizersMatrix,
													residualMatrix);

	else
		MultiplyByColumnNatrix(minimizersMatrix, residualMatrix);

	size_t numRows = m_sparseIndices[0] - 1;
	residuals.resize(numRows);

	for(int i = 0; i < numRows; i++)
		residuals[i] = residualMatrix(i, 0);
}

//-----------------------------------------------------------------------------

void SparseMatrix::preconditionerSolver(const std::vector<double > &rhsVals,
									    std::vector<double > &minimizers,
										bool isPreconditionerMatrix)const
{
	SparseMatrix sparseMatrix;
	if(!isPreconditionerMatrix)
		sparseMatrix = *this;
	else
	{
		computePreconditionerMatrix(sparseMatrix);
	}

	int i;

	size_t n = rhsVals.size();
	for(i = 0; i < n; i++)
		minimizers[i] = 
			(fabs(sparseMatrix.m_sparseElements[i]) >  1e-8 ? 
					(rhsVals[i]/sparseMatrix.m_sparseElements[i]) : rhsVals[i]);
}

//-----------------------------------------------------------------------------

void SparseMatrix::SolveSparseLinearSystem(const std::vector<double > &rhsVals,
										   const int iterTol,
										   const double tol, const int iterMax,
										   int &iter, 
										   std::vector<double > &variableVals,
										   bool* isProcessBreak)const
{
	//ProgressEvent* sparseSolverEvent = 
	//		EssenceProgressEvent::GetProgressEvent(m_progressAnalyser,SPARSE_SOLVER_EVENT);
	bool isEventEllapsed = false;

	//znrm = 0 for lint
	double ak, akden, bk, bkden = 1.0, bknum, bnrm = 0, dxnrm, xnrm, zminrm, znrm = 0;

	//Double precision is a good idea in this routine
	const double EPS = 1.0e-14;
	int j;

	size_t n = rhsVals.size(); //rhsVals = b

	std::vector<double> p(n), pp(n), r(n), rr(n), z(n), zz(n);

	iter = 0;
	//Calculates initial residual
	//Input to is variableVals[0,.....,n-1]
	//Output is r[0,......,n-1]
	variableVals.resize(n, 0.);
	computeProdOfAOrATrans(variableVals, 0, r);

	for(j = 0; j < n; j++)
	{	//1
		r[j] = rhsVals[j] - r[j];
		rr[j] = r[j];
	}	//1

	//Uncomment this line to get "minimum residual" variant of the algorithm
	//computeProdOfAOrATrans(r, 0, rr);

	if(iterTol == 1)
	{	//2
		bnrm = computeNormForMinimizerVector(rhsVals,iterTol);
		preconditionerSolver(r, z, 0);
	}	//2
	else if(iterTol == 2)
	{	//3
		preconditionerSolver(rhsVals, z, 0);
		bnrm = computeNormForMinimizerVector(z, iterTol);
		preconditionerSolver(r, z, 0);
	}	//3
	else if(iterTol == 3 || iterTol == 4)
	{	//4
		preconditionerSolver(rhsVals, z, 0);
		bnrm = computeNormForMinimizerVector(z, iterTol);
		preconditionerSolver(r, z, 0);
		znrm = computeNormForMinimizerVector(z, iterTol);
	}	//4
	else
		assert(0);

	while(iter < iterMax)
	{	
		//if(sparseSolverEvent)
			//sparseSolverEvent->Started();
		// 5
		++iter;
		preconditionerSolver(rr, zz, 1);
		for(bknum = 0.0, j = 0; j < n; j++)
			bknum += z[j]*rr[j];
		if(iter == 1)
		{	// 6
			for(j = 0; j < n; j++)
			{	//7
				p[j] = z[j];
				pp[j] = zz[j];
			}	// 7
		}	// 6

		else
		{	//8
	
			if(fabs(bkden) <= 0.)
            {
                bk = 1.0;
				break;
            }

            bk = bknum/bkden;

            for(j = 0; j < n; j++)
			{	//9
				p[j] = bk*p[j] + z[j];
				pp[j] = bk*pp[j] + zz[j];
			}	//9
		}	//8

		bkden = bknum;
		computeProdOfAOrATrans(p, 0, z);

		for(akden = 0.0, j = 0; j < n; j++)
			akden += z[j]*pp[j];
        if(fabs(akden) <= 0.) 
        { 
            ak = 1.0; 
            break;
        };
		ak = bknum/akden;

		computeProdOfAOrATrans(pp, 1, zz);
		for(j = 0; j < n; j++)
		{	//10
			variableVals[j] += ak*p[j];
			r[j] -= ak*z[j];
			rr[j] -= ak*zz[j];
		}	//10

		preconditionerSolver(r, z, 0);

		double err = 0; //intitialised for lint
		if(iterTol == 1)
			err = computeNormForMinimizerVector(r, iterTol)/bnrm;
		else if(iterTol == 2)
			err = computeNormForMinimizerVector(z, iterTol)/bnrm;
		else if(iterTol == 3 || iterTol == 4)
		{	// 11
			zminrm = znrm;
			znrm = computeNormForMinimizerVector(z, iterTol);

			if(fabs(zminrm - znrm) > EPS*znrm)
			{	//12
				dxnrm = fabs(ak)*computeNormForMinimizerVector(p, iterTol);
				err = znrm/fabs(zminrm - znrm)*dxnrm;
			}	//12
			else
			{	//13
				err = znrm/bnrm;
				continue;
			}	//13
		
			xnrm = computeNormForMinimizerVector(variableVals, iterTol);

			if(err <= 0.5*xnrm)
				err /= xnrm;
			else
			{	//14
				err = znrm/bnrm;
				continue;
			}	//14
		}	//11
		isEventEllapsed = true;
		/*if(sparseSolverEvent && !sparseSolverEvent->Ellapsed())
		{
			*isProcessBreak = true;
			 break;
		}			*/
		if(err <= tol)
			break;
	}	//5
	/*if(sparseSolverEvent && !isEventEllapsed)
	{
		if(!sparseSolverEvent->Ellapsed())
			*isProcessBreak = true;
		
	}*/
}

//-----------------------------------------------------------------------------

void SparseMatrix::computePreconditionerMatrix(
									SparseMatrix &preconditionerMatrix)const
{
	preconditionerMatrix.m_sparseIndices.resize(m_sparseIndices[0]);
	preconditionerMatrix.m_sparseElements.resize(m_sparseIndices[0]);

	for(int i = 0; i < m_sparseIndices[0] - 1; i++)
	{
		preconditionerMatrix.m_sparseElements[i] = m_sparseElements[i];
		preconditionerMatrix.m_sparseIndices[i] = m_sparseIndices[0];
	}

	//1024 is junk val
	preconditionerMatrix.m_sparseElements[m_sparseIndices[0] - 1] = 1024.;
	preconditionerMatrix.m_sparseIndices[m_sparseIndices[0] - 1] = 
															m_sparseIndices[0];	
}

//-----------------------------------------------------------------------------

double SparseMatrix::GetValueFromSqureMatrixIndices(int sqrMatRow, int sqrMatCol)const
{
	if(sqrMatRow == sqrMatCol)//Digonal element
	{
		return m_sparseElements[sqrMatRow];
	}
	else//Off-Diagonal Element
	{
		for(size_t k = m_sparseIndices[sqrMatRow]; k < m_sparseIndices[sqrMatRow + 1] ; k++)
		{
			if(sqrMatCol == m_sparseIndices[k])
				return m_sparseElements[k];
		}
	}
	//In above conditions there are non-zero element stoarage
	return 0.0;
}

//-----------------------------------------------------------------------------

void SparseMatrix::Dump(Report &report, int tabIndent)const
{
	report.AddMarker(tabIndent);
	size_t size = m_sparseIndices.size();
	for(int i = 0; i < size; ++i)
	{
		report.Add(0, "%d\t", i);
	}

//	report.Add(0, "\n");

	report.AddMarker(tabIndent);

	for(int i = 0; i < size; ++i)
	{
		report.Add(tabIndent, "%d\t", m_sparseIndices[i]);
	}

	report.AddMarker(tabIndent);

	for(int i = 0; i < size; ++i)
	{
		report.Add(tabIndent, "%f \t", m_sparseElements[i]);
	}

	report.AddMarker(tabIndent);

	report.Flush();

}

//-----------------------------------------------------------------------------

void SparseMatrix::DumpInSqrMatrixForm(Report &report, int tabIndent)const
{
	if(!m_sparseIndices.size())
		return;

	SqMatrix sqrMatrix;
	GetSqrMatrix(sqrMatrix);
	sqrMatrix.Dump(report,tabIndent);
}

//-----------------------------------------------------------------------------

void SparseMatrix::GetSqrMatrix(SqMatrix &sqrMatrix)const
{
	size_t numRows = m_sparseIndices[0] - 1;
	sqrMatrix.Resize(numRows,numRows);
	for(size_t i = 0; i < numRows; i++)
	{
		sqrMatrix(i,i) = m_sparseElements[i];
		for(size_t j = m_sparseIndices[i]; j < m_sparseIndices[i + 1] ; j++)
		{
			sqrMatrix(i,m_sparseIndices[j]) = m_sparseElements[j];
		}
	}
}

//-----------------------------------------------------------------------------

}