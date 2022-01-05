// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _POLYLINE3D_H_
#define _POLYLINE3D_H_

// Include Files
#include "CUpoint3d.h"

namespace CommonUtil
{
// Forward Declaration
class Segment3D;

class Polyline3D
{
	std::vector<CommonUtil::Point3D> m_points;

	bool checkSegment(double u1, double u2, double)const;

public:
	Polyline3D(std::vector<CommonUtil::Point3D>& points) : m_points(points)
	{}

	Polyline3D()
	{}

	~Polyline3D()
	{}

	void AddPoint(const Point3D *point)
	{
		m_points.push_back(*point);
	}

	void InsertPoint(size_t index, const Point3D &point)
	{
		m_points.insert(m_points.begin()+index, point);
	}

	void SetPoint(size_t index, const Point3D &point)
	{
		m_points[index] = point;
	}

	Point3D GetPoint(size_t index)const
	{
		return m_points[index];
	}

	Point3D GetPoint(double uVal)const;

	std::vector<CommonUtil::Point3D> getPoints()const
	{
		return m_points;
	}

	void Smoothen(double smoothenTol, std::vector<size_t> &uVals)const;

	size_t getNumPoints()const
	{
		return m_points.size();
	}

	Segment3D GetSegment(size_t index)const;

	void Clear()
	{
		m_points.clear();
	}
};

}
#endif