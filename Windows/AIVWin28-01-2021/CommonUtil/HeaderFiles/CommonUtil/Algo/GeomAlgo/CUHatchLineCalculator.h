// ============================================================================
//			Copyright (c) 2018 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_HATCHLINE_CALCULATOR_H_
#define _COMMONUTIL_HATCHLINE_CALCULATOR_H_

#include "CUVector.h"
#include "CUPoint3D.h"
#include "CUSqMatrix.h"
namespace CommonUtil
{	
	class Segment3D;

	struct PointuValStruct
	{
		Point3D m_point;
		double m_uVal;
	};

	//This class calculates hatching lines for given 2D polyline (in X,Y directions)
	class HatchLineCalculator
	{
		std::vector<Point3D>m_polygonPoints;
		Vector m_hatchlineDirection;
		Vector m_growDirection;
		Point3D m_startPoint;
		double m_lineSpacing;

		static bool sortPointuValStructByuVal(const PointuValStruct& p1, const PointuValStruct&p2);

		void getlimitingDistanceInBothDirection(double &distanceInGrowDir, double &distanceInOppositeDir);

		void getHatchLinesInGivenDir(Vector& growDir, const std::vector<Segment3D>&polygonSegments, const double &distanceInGivenDir, std::vector<Segment3D>& hatchLines);
	public:
		HatchLineCalculator(const std::vector<Point3D>& polygonPoints, Vector& hatchLineDir, Point3D& startPoint, double lineSpacing);

		bool Execute(std::vector<Segment3D>& hatchLines);
	};

}

#endif