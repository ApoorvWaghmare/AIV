#pragma once

//standard include files
#include <vector>

//Includes Header Files
#include "CUPoint2D.h"

//forward declarations
namespace CommonUtil
{
	class Segment2D;

	class HullFinder
	{
	private:

		//****************************************************General methods****************************************************

		double isLeft(CommonUtil::Point2D p0, CommonUtil::Point2D p1, CommonUtil::Point2D p2);

		bool checkEdgeIntersection(const CommonUtil::Point2D &p0, const CommonUtil::Point2D &p1, const CommonUtil::Point2D &p2, const CommonUtil::Point2D &p3);

		bool checkEdgeIntersection(const std::vector<CommonUtil::Point2D> &hull, CommonUtil::Point2D curEdgeStart, CommonUtil::Point2D curEdgeEnd, CommonUtil::Point2D checkEdgeStart, CommonUtil::Point2D checkEdgeEnd);

		CommonUtil::Point2D nearestInnerPoint(const CommonUtil::Point2D edgeStart, const CommonUtil::Point2D edgeEnd, const std::vector<CommonUtil::Point2D> &points,
			const std::vector<CommonUtil::Point2D> &hull, bool &found);

		double GetDecisionDistance(const CommonUtil::Point2D point, const std::vector<CommonUtil::Point2D> &myPoints) const;

		double GetNearestPointDistance(const CommonUtil::Segment2D seg, const CommonUtil::Point2D &point)const;
	public:

		//************************** Constructor and Destructor**************************

		HullFinder(void);

		~HullFinder(void);

		//**************************General methods**************************

		//Finds Convex Hull
		std::vector<CommonUtil::Point2D> FindConvexHull(const std::vector<CommonUtil::Point2D> & points);

		//Finds Concave Hull
		std::vector<CommonUtil::Point2D> FindConcaveHull(const std::vector<CommonUtil::Point2D> & points, double N);

		//Returns Average minimum distance between Points
		double GetAvgNearestPointDistOfGivenPoints(const std::vector<CommonUtil::Point2D> & points, double &maxNearestDist);
	};
}
