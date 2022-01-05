//=============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
//=============================================================================

#ifndef _COMMON_CONSTANTS_H_
#define _COMMON_CONSTANTS_H_

/*
This file contains common constants required for Settings of Cache data structure
*/

namespace CommonUtil
{

enum Orientation
{
	AntiClockWise,
	ClockWise,
	UnKnown
};

enum CellStatus
{
	None,
	Inside,
	Outside,
	Intersecting
};

enum Units
{
	MICRONS = 0,
	MILLIMETERS,
	CENTIMETERS,
	METERS,
	INCHES,
	FEET,
	KILOMETERS,
	INVALID,
};

enum TxtReaderTemplate
{
	INVALID_TEMPLATE,
	POINTS_ONLY,
	POINTS_PLUS_RGB_COLORS,
	POINTS_PLUS_VECTORS
};

enum PolylineType
{
	INVALID_TYPE,
	OPEN_TYPE,
	CLOSED_TYPE,
};
class CommonConstants
{
public:

	static const bool INCORE;

	static const size_t MAX_NUM_VERTICES_IN_CELL;

	static const size_t MAX_NUM_POINTS_IN_CELL;

	static const size_t MAX_NUM_CELLS_IN_MEMORY;

	static const size_t MAX_DEPTH_LEVEL;

	static const size_t MAX_VISIBLE_CELLS;

	static const size_t MAX_NEIGHBOURS;

	
	//A double value less than this value can be treated at 0.
	static const double ZERO;

	//If the distance between two points is less than this distance, then
	//the two points can be treated as coincident
	static const double PNT_TOL;
	
	static const double SQR_PNT_TOL;

	//Value of PI
	static const double PI;

	//Tolerance to check if the points are collinear
	//It is the tolerance on angle in radian between two vectors.
	//If the angle is less than this tolerance, the vectors are said to be
	//collinear
	static const double COLLINEARITY_TOL;

	// Tolerance for minimum length of a segment
	static const double LENGTH_TOL;

	static const Units INTERNAL_UNIT;
};
}
#endif
