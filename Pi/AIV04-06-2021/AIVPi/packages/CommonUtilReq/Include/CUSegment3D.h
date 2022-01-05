// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_SEGMENT3D_H_
#define _COMMONUTIL_SEGMENT3D_H_

//math include files
#include "CUPoint3D.h"
#include "CUPoint3DF.h"
#include "CUBoundingBox3d.h"

namespace CommonUtil
{

//Forward declaration
class Transform3D;
class SqMatrix;

//This class represents segment bounded by two 3d points
class Segment3D
{
	//========================== private data members =========================

	Point3D m_p1;
	Point3D m_p2;

public :

	//======================== public member functions ========================

	// this data structure is used to store information
	// about the relative positions of two parallel segments
	struct ParallelInterSectionInfo
	{
		double m_Edge1p1ProjectionOnEdge2;
		double m_Edge1p2ProjectionOnEdge2;

		double m_Edge2p1ProjectionOnEdge1;
		double m_Edge2p2ProjectionOnEdge1;

		bool m_parallel;
		bool m_collinear;

		void Initialize()
		{
			m_parallel = false;
			m_collinear = false;
			m_Edge1p1ProjectionOnEdge2 = -1;
			m_Edge1p2ProjectionOnEdge2 = -1;

			m_Edge2p1ProjectionOnEdge1 = -1;
			m_Edge2p2ProjectionOnEdge1 = -1;
		}

		// checks if there is an overlap between the two segments
		bool IsOverlap()const;
	};


	//************************** list of constructors *************************

	//defualt constructor
	Segment3D()
	{}

	//constructor
	Segment3D(const Point3D &p1, const Point3D &p2)
		: m_p1(p1),
		  m_p2(p2)
	{}

	Segment3D(const Point3DF &p1, const Point3DF &p2)
	{
		m_p1.Set(p1.GetX(), p1.GetY(), p1.GetZ());
		m_p2.Set(p2.GetX(), p2.GetY(), p2.GetZ());
	}

	//****************************** destructor *******************************

	// Destructor
	~Segment3D()
	{}

	//*************************** get/set methods *****************************

	//skew intersection between two segments
	bool GetSkewIntersection(const Segment3D &segment, double &u, double &v,
							 double skewDistTol, double uSnappingFactor,
							 double vSnappingFactor,
							 double angTol,
							 ParallelInterSectionInfo *parallelInfo)const;

	//returns the first point of the segment
	Point3D GetStartPoint()const;

	//returns the second point of the segment
	Point3D GetEndPoint()const;

	const Point3D* GetPoint1()const
	{
		return &m_p1;
	}

	const Point3D* GetPoint2()const
	{
		return &m_p2;
	}

	// returns the point on the segment with the given parametric value
	Point3D GetPoint(double uVal)const;

	// returns the point on the segment with the given parametric value
	void GetPoint(double val, Point3D& point)const;

	//returns the perpendicular distance from the point
	double GetDistance(const Point3D &point)const;

	double GetSquareDistance(const Point3D &point)const;

	//returns the length of the segment
	double GetLength()const
	{
		return m_p1.Distance(m_p2);
	}

	double GetSqrLength()const
	{
		return m_p1.SqrDistance(m_p2);
	}

	//set the first and second point of the segment
	void Set (const Point3D &p1, const Point3D &p2)
	{
		m_p1 = p1;
		m_p2 = p2;
	}

	//set the first and second point of the segment
	void SetPoint1(const Point3D &p1)
	{
		m_p1 = p1;
	}

	//set the first and second point of the segment
	void SetPoint2(const Point3D &p2)
	{
		m_p2 = p2;
	}

	// Sets points of the segment
	void Set (const Point3D *p1, const Point3D *p2)
	{
		m_p1 = *p1;
		m_p2 = *p2;
	}

	//**************************general methods********************************

	// checks parallelity  within given angular tolearance
	// bool checkParallelism(const Segment3D & segment, double angTol)const;
	bool CheckParallelism(const Segment3D & segment, double angTol,
						  double skewTol)const;

	// checks if the given point lies on the segment
	bool CheckOnSegment(const Point3D &projPoint)const;

	//tessellates Segment3D
	void Tessellate(const std::vector<CommonUtil::Point3D> &points,
							double tessellationTol)const;

	//returns bounding box
	BoundingBox3D GetBoundingBox(double expansionTol)const;

	//apply given transformation and rotation
	void Transform(const SqMatrix &Rotation,
				   const std::vector<double> &translation);

	// checks if the given point has a projection on the segment
	// if the projection is nearer to one of the ends of segment, then
	// it is snapped as per the snapping tolerance
	// uVal is a parametric value of projected point on the segment
	// uVal is returned even if it is found < 0 or > 1.
	bool Project(const Point3D &projPoint, double snappingTol,
				 double &uVal)const;

	bool Project(const Point3DF &projPoint, double snappingTol,
		double &uVal)const;

	//dumps the data
	void Dump(Report &report, int tabIndent)const;

	//apply given transformation
	void Transform(const Transform3D &transform);

	//checks if given point is between -ZERO to 1+ZERO
	bool IsPointWithinParamVals(const Point3D& point,
								double snappingTol = 0.0)const;


	double GetMaxDeviation(const std::vector<CommonUtil::Point3D> &dataPoints)const;
	//temp added for sprite

	bool ProjectPoint(const Point3D& pointToBeProjected,
					  double snappingTol, Point3D &projectionPoint, bool pointOnsegement = true)const;


	bool Project1(const Point3D &projPoint, double snappingTol,
				 double &uVal)const;

	double ComputeAngleMadeByTwoPoints(const Point3D &point1, const Point3D &point2)const;

	bool IsCollinear(const Point3D &point)const;

	bool IsCollinear(const Segment3D &segment)const;

	bool IsCollinearAndOverlapping(const Segment3D &segment, Point3D intersectionPoints[2])const;

	bool CheckOnSeg(const Point3D *projPoint)const;
};

}

#endif
