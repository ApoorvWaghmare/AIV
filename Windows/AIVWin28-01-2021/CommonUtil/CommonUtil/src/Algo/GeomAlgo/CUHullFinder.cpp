#pragma once

#include "CUHullFinder.h"

//Includes Header Files
#include "CUSegment2D.h"

//==================================================================================================================================================================
namespace CommonUtil
{
	//Constructor
	HullFinder::HullFinder(void)
	{

	}
	//==================================================================================================================================================================
	//destructor
	HullFinder::~HullFinder(void)
	{

	}
	//==================================================================================================================================================================
	//
	double HullFinder::isLeft(CommonUtil::Point2D p0, CommonUtil::Point2D p1, CommonUtil::Point2D p2)
	{
		return (p1.GetX() - p0.GetX()) * (p2.GetY() - p0.GetY()) - (p2.GetX() - p0.GetX()) * (p1.GetY() - p0.GetY());
	}

	//==================================================================================================================================================================

	bool HullFinder::checkEdgeIntersection(const CommonUtil::Point2D &p0, const CommonUtil::Point2D &p1, const CommonUtil::Point2D &p2, const CommonUtil::Point2D &p3)
	{
		double s1_x = p1.GetX() - p0.GetX();
		double s1_y = p1.GetY() - p0.GetY();
		double s2_x = p3.GetX() - p2.GetX();
		double s2_y = p3.GetY() - p2.GetY();
		double s = (-s1_y * (p0.GetX() - p2.GetX()) + s1_x * (p0.GetY() - p2.GetY())) / (-s2_x * s1_y + s1_x * s2_y);
		double t = (s2_x * (p0.GetY() - p2.GetY()) - s2_y * (p0.GetX() - p2.GetX())) / (-s2_x * s1_y + s1_x * s2_y);
		return (s > 0 && s < 1 && t > 0 && t < 1);
	}

	//==================================================================================================================================================================

	bool HullFinder::checkEdgeIntersection(const std::vector<CommonUtil::Point2D> &hull, CommonUtil::Point2D curEdgeStart, CommonUtil::Point2D curEdgeEnd,
		CommonUtil::Point2D checkEdgeStart, CommonUtil::Point2D checkEdgeEnd)
	{
		size_t numHullPoints = hull.size();
		bool status = false;

		if (numHullPoints < 3)
			return status;

		for (size_t i = 0; i < (numHullPoints - 1); ++i)
		{
			CommonUtil::Point2D p1 = hull[i];
			CommonUtil::Point2D p2 = hull[i + 1];

			//Skip Current Segment
			if ((curEdgeStart.IsEqual(p1) && curEdgeEnd.IsEqual(p2)) || (curEdgeStart.IsEqual(p2) && curEdgeEnd.IsEqual(p1)))
				continue;

			if (checkEdgeIntersection(checkEdgeStart, checkEdgeEnd, p1, p2))
			{
				status = true;
				break;
			}
		}

		return status;
	}
	//==================================================================================================================================================================
	// returns nearest inner point form edge
	CommonUtil::Point2D HullFinder::nearestInnerPoint(const CommonUtil::Point2D edgeStart, const CommonUtil::Point2D edgeEnd, const std::vector<CommonUtil::Point2D> &points,
		const std::vector<CommonUtil::Point2D> &hull, bool &found)
	{
		CommonUtil::Point2D nearestPoint;
		double distance = DBL_MAX;
		found = false;

		for (size_t iPnt = 0; iPnt < points.size(); ++iPnt)
		{
			bool isPointInsideHull = false;

			// Skip points that are already in he hull	
			CommonUtil::Point2D tempPoint = points[iPnt];

			for (size_t iHullPnt = 0; iHullPnt < (hull.size() - 1); ++iHullPnt)
			{
				if (hull[iHullPnt].IsEqual(tempPoint))
				{
					isPointInsideHull = true;
					break;
				}
			}

			if (isPointInsideHull)
			{
				continue;
			}

			//Calculate distance of point from edge
			CommonUtil::Segment2D edge(edgeStart, edgeEnd);
			double d = GetNearestPointDistance(edge, tempPoint);

			//double d = tempPoint.Distance(edgeStart, edgeEnd);
			bool skip = false;

			for (size_t iPoint = 0; ((!skip) && (iPoint < (hull.size() - 1))); ++iPoint)
			{
				CommonUtil::Segment2D edgeTmp(hull[iPoint], hull[iPoint + 1]);
				double dTmp = GetNearestPointDistance(edgeTmp, tempPoint);
				//double dTmp = tempPoint.Distance(hull[iPoint], hull[iPoint + 1]);
				skip |= dTmp < d;
			}

			if (skip)
			{
				continue;
			}

			if (!(found) || distance > d)
			{
				nearestPoint = tempPoint;
				distance = d;
				found = true;
			}
		}

		return nearestPoint;
	}
	//==================================================================================================================================================================
	// returns decision distance from a point
	double HullFinder::GetDecisionDistance(const CommonUtil::Point2D point, const std::vector<CommonUtil::Point2D> &myPoints) const
	{
		double miDistance = DBL_MAX;

		for (size_t i = 0; i < myPoints.size(); ++i)
		{
			CommonUtil::Point2D tmpPoint = myPoints[i];
			double tmpDistance = point.Distance(tmpPoint);

			if (tmpDistance < miDistance)
			{
				miDistance = tmpDistance;
			}
		}

		return miDistance;
	}
	//==================================================================================================================================================================
	// returns vector of points of convex Hull
	std::vector<CommonUtil::Point2D> HullFinder::FindConvexHull(const std::vector<CommonUtil::Point2D> & points)
	{
		std::vector<CommonUtil::Point2D> allPoints(points);
		std::vector<CommonUtil::Point2D> convexHullPoints;

		size_t numPoints = points.size();

		if (numPoints < 3)
			return convexHullPoints;

		// Sort P by x and y
		for (size_t i = 0; i < numPoints; ++i)
		{
			for (size_t j = i + 1; j < numPoints; ++j)
			{
				if ((allPoints[j].GetX() < allPoints[i].GetX()) ||
					((fabs(allPoints[j].GetX() - allPoints[i].GetX()) < CommonUtil::CommonConstants::ZERO) && (allPoints[j].GetY() < allPoints[i].GetY())))
				{
					CommonUtil::Point2D tmpPoint = allPoints[i];
					allPoints[i] = allPoints[j];
					allPoints[j] = tmpPoint;
				}
			}
		}

		// Get the indices of points with min x-coord and min|max y-coord
		int minmin = 0, minmax = 0, index = 0;
		double xmin = allPoints[0].GetX();

		for (index = 1; index < numPoints; ++index)
		{
			if (allPoints[index].GetX() != xmin)
				break;
		}


		minmax = index - 1;

		if (minmax == (allPoints.size() - 1)) // all x-coords == xmin
		{
			convexHullPoints.push_back(allPoints[minmin]);

			if (allPoints[minmax].GetY() != allPoints[minmin].GetY())
				convexHullPoints.push_back(allPoints[minmax]);

			//Add polygon endpoint
			convexHullPoints.push_back(allPoints[minmin]);
			return convexHullPoints;
		}

		// Get the indices of points with max x-coord and min|max y-coord
		int maxmin = 0, maxmax = (int)allPoints.size() - 1;
		double xmax = allPoints[allPoints.size() - 1].GetX();

		for (index = ((int)allPoints.size() - 2); index >= 0; --index)
		{
			if (allPoints[index].GetX() != xmax)
				break;
		}

		maxmin = index + 1;

		// Compute the lower hull
		convexHullPoints.push_back(allPoints[minmin]);
		index = minmax;

		while (++index <= maxmin)
		{
			// the lower line joins allPoints[minmin]  with allPoints[maxmin]
			if (isLeft(allPoints[minmin], allPoints[maxmin], allPoints[index]) >= 0 && index < maxmin)
				continue;           // ignore allPoints[i] above or on the lower line

			while (convexHullPoints.size() > 1)         // there are at least 2 points on the stack
			{
				// test if  allPoints[i] is left of the line at the stack top
				if (isLeft(convexHullPoints[convexHullPoints.size() - 2], convexHullPoints[convexHullPoints.size() - 1], allPoints[index]) > 0)
					break;         // allPoints[i] is a new hull  vertex
				convexHullPoints.pop_back();         // pop top point off  stack
			}

			convexHullPoints.push_back(allPoints[index]);
		}

		//compute the upper hull above  the bottom hull
		// if  distinct xmax points
		if (maxmax != maxmin)
			convexHullPoints.push_back(allPoints[maxmax]);

		//Bottom point of the upper hull stack
		int bot = (int)convexHullPoints.size();
		index = maxmin;

		while (--index >= minmax)
		{
			// the upper line joins P[maxmax]  with P[minmax]
			// ignore P[i] below or on the upper line
			if (isLeft(allPoints[maxmax], allPoints[minmax], allPoints[index]) >= 0 && index > minmax)
				continue;

			while (convexHullPoints.size() > bot)     // at least 2 points on the upper stack
			{
				// test if  allPoints[i] is left of the line at the stack top
				// allPoints[i] is a new hull  vertex
				if (isLeft(convexHullPoints[convexHullPoints.size() - 2], convexHullPoints[convexHullPoints.size() - 1], allPoints[index]) > 0)
					break;

				// pop top point off stack
				convexHullPoints.pop_back();
			}

			convexHullPoints.push_back(allPoints[index]);
		}

		// push  joining endpoint onto stack
		if (minmax != minmin)
			convexHullPoints.push_back(allPoints[minmin]);

		return convexHullPoints;
	}
	//==================================================================================================================================================================
	// returns vector of points of concave Hull
	std::vector<CommonUtil::Point2D> HullFinder::FindConcaveHull(const std::vector<CommonUtil::Point2D> & points, double N)
	{
		std::vector<CommonUtil::Point2D> convexHullPoints = FindConvexHull(points);

		if (convexHullPoints.size() < 3)
			return convexHullPoints;

		for (int i = 0; i < (convexHullPoints.size() - 1); ++i)
		{
			// Find the nearest inner point from the edge (ci1, ci2);
			CommonUtil::Point2D edgeStart = convexHullPoints[i];
			CommonUtil::Point2D edgeEnd = convexHullPoints[i + 1];

			bool found = false;
			CommonUtil::Point2D nearestPoint = nearestInnerPoint(edgeStart, edgeEnd, points, convexHullPoints, found);

			if (!found)
			{
				continue;
			}

			//Calculate the length of edge
			double edgeLength = edgeStart.Distance(edgeEnd);

			std::vector<CommonUtil::Point2D> templist;
			templist.push_back(edgeStart);
			templist.push_back(edgeEnd);

			double decisionDistance = GetDecisionDistance(nearestPoint, templist);

			if ((edgeLength / decisionDistance) > N)
			{
				// Check that new edge will not intersect existing edges.
				bool intersects = checkEdgeIntersection(convexHullPoints, edgeStart, edgeEnd, edgeStart, nearestPoint);
				intersects |= checkEdgeIntersection(convexHullPoints, edgeStart, edgeEnd, nearestPoint, edgeEnd);
				if (!intersects)
				{
					convexHullPoints.insert(convexHullPoints.begin() + i + 1, nearestPoint);
					--i;
				}
			}
		}

		return convexHullPoints;
	}

	//==================================================================================================================================================================

	double HullFinder::GetAvgNearestPointDistOfGivenPoints(const std::vector<CommonUtil::Point2D> & points, double &maxNearestDist)
	{
		double avgNearestDist = 0.0;
		maxNearestDist = DBL_MIN;
		size_t numPoints = points.size();

		for (size_t iCount = 0; iCount < numPoints; ++iCount)
		{
			double minDist = DBL_MAX;
			CommonUtil::Point2D point = points[iCount];

			//Calculate nearest point distance
			for (size_t jCount = 0; jCount < numPoints; ++jCount)
			{
				if (iCount == jCount)
					continue;

				CommonUtil::Point2D tmpPoint = points[jCount];

				double tmpDist = point.Distance(tmpPoint);

				if (tmpDist < minDist)
					minDist = tmpDist;
			}

			if (minDist > maxNearestDist)
				maxNearestDist = minDist;

			avgNearestDist += minDist;
		}

		avgNearestDist /= numPoints;

		return avgNearestDist;
	}
	//==================================================================================================================================================================

	double HullFinder::GetNearestPointDistance(const CommonUtil::Segment2D seg, const CommonUtil::Point2D &point)const
	{
		double snappingTol = DBL_MIN;
		double uVal = 0.0;

		seg.Project(point, snappingTol, uVal);

		if (uVal < 0.0)
			uVal = 0.0;
		else if (uVal > 1.0)
			uVal = 1.0;

		CommonUtil::Point2D nearPoint = seg.GetPoint(uVal);

		double dist = nearPoint.Distance(point);

		return dist;
	}
	//==================================================================================================================================================================
}