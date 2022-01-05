#pragma once

#include "MKLWrapperDLLAPI.h"
// class 'xxx' needs to have dll-interface to be used by clients of class 'yyy'
//#   pragma warning( disable: 4251 )

//Define Data types
#ifdef PMK_64
  #define PMK_INT _int64
  #define PMK_INTEGER_t long long int
#else
  #define PMK_INT int
  #define PMK_INTEGER_t int
#endif

// These APIs represents wrapper API functions of the Intel MKL functions

// This function returns rank of matrix
MKLWRAPPER_API size_t __cdecl GetRankOfMatrix(PMK_INT numRows, PMK_INT numColumns, double* matrix);

// This function multiplies two matrices and returns output matrix
// matANumRows: number of rows of first matrix
// matANumCols: number of columns of first matrix
// matBNumRows: number of rows of second matrix
// matBNumCols: number of columns of second matrix 
// bTransA: Flag indicates whether to transpose first matrix before multiplication
// bTransB: Flag indicates whether to transpose second matrix before multiplication
// matA: first matrix
// matB: second matrix
// resultMatrix: output matrix
MKLWRAPPER_API void __cdecl MultiplyMatrices(PMK_INT matANumRows, PMK_INT matANumCols,
											 PMK_INT matBNumRows, PMK_INT matBNumCols,
											 bool bTransA, const double* matA, 
											 bool bTransB, const double* matB,
											 double* resultMatrix);

// This function do matrix inverse operation
MKLWRAPPER_API bool __cdecl GetInverseMatrix(PMK_INT numRows, double* inverseMatrix);

// This function returns Intel MKL maximum threads
MKLWRAPPER_API PMK_INT __cdecl GetIntelMklMaxThreads();

// This function solves linear equations
MKLWRAPPER_API void __cdecl SolveLinearSystem(void* pt, PMK_INTEGER_t* maxfct, PMK_INTEGER_t* mnum, PMK_INTEGER_t* mtype, PMK_INTEGER_t* phase,
											  PMK_INTEGER_t* n, double* a, PMK_INTEGER_t* ia, PMK_INTEGER_t* ja, PMK_INTEGER_t* idum, 
											  PMK_INTEGER_t* nrhs, PMK_INTEGER_t* iparam, PMK_INTEGER_t* msglvl, double* ddum1, double* ddum2, 
											  PMK_INTEGER_t* error);

// This function gives eigen values and eigen vectors for Square Matrix
MKLWRAPPER_API bool __cdecl GetEigenInfo(char jobvl, char jobvr,
										 PMK_INTEGER_t order, double* matrix, PMK_INTEGER_t lda, double* wr,
										 double* wi, double* vl, PMK_INTEGER_t ldvl, double* vr,
										 PMK_INTEGER_t ldvr);
