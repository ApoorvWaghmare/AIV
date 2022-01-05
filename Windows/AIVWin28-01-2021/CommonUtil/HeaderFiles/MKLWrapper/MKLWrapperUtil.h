#pragma once

#include "mkl_types.h"

double* allocMem(MKL_INT allocSize);

void freeMem(double*& pMem);
