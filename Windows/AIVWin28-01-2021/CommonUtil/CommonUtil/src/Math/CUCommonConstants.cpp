// ============================================================================
//			Copyright (c) 2016 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//math include files
#include "CUCommonConstants.h"

namespace CommonUtil
{

//---------------.------------------------------------------------------
const bool CommonConstants::INCORE = true;

//---------------.------------------------------------------------------
const size_t CommonConstants::MAX_NUM_VERTICES_IN_CELL = 20;

//---------------.------------------------------------------------------
const size_t CommonConstants::MAX_NUM_POINTS_IN_CELL = 20;

//---------------.------------------------------------------------------
const size_t CommonConstants::MAX_NUM_CELLS_IN_MEMORY = 2000000;

//---------------.------------------------------------------------------
const size_t CommonConstants::MAX_DEPTH_LEVEL = 8;

//---------------.------------------------------------------------------
const size_t CommonConstants::MAX_VISIBLE_CELLS = 6000000;

//---------------.------------------------------------------------------
const size_t CommonConstants::MAX_NEIGHBOURS = 25;

//---------------.------------------------------------------------------

const double CommonConstants::ZERO = 1e-8;

//---------------------------------------------------------------------

const double CommonConstants::PNT_TOL = 1e-6;


//---------------------------------------------------------------------

const double CommonConstants::SQR_PNT_TOL = PNT_TOL *PNT_TOL;

//---------------------------------------------------------------------

const double CommonConstants::PI= 3.14159265358979323846;

//---------------------------------------------------------------------
	
const double CommonConstants::COLLINEARITY_TOL = 1e-4;

//---------------------------------------------------------------------

const double CommonConstants::LENGTH_TOL = 1e-3;

//---------------------------------------------------------------------
const Units CommonConstants::INTERNAL_UNIT = MILLIMETERS;
}