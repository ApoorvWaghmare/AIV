// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _ESSENCE_POLYGON_H_
#define _ESSENCE_POLYGON_H_

//standard include files
#include <assert.h>

// math include files
#include "CUPoint2D.h"

namespace CommonUtil
{

//forward declarations
class BoundingBox2D;
class Segment2D;

//It represents a closed polygon in XY plane
class Polygon
{

	struct VertexAngle;

	struct PolygonVertex
	{
		Point2D m_point;
		size_t m_index;
		VertexAngle* m_vertexAngle;
	};
	
	struct VertexAngle
	{
		PolygonVertex* m_vertex;
		double m_angle;
	};

	class CmpAngles
	{
	public:
		bool operator() (const VertexAngle* a1, const VertexAngle* a2)
		{
			return a1->m_angle < a2->m_angle;
		}
	};

	//========================== private data members =========================

	std::vector<Point2D> m_polygonPoints;
	
	//======================= private members functions =======================

	// gets the maximum bounds of the polygon
	void getExtents(double &minX, double &maxX, double &minY, double &maxY)const;

	// Douglases the peucker reduction
	bool douglasPeuckerPointReduction(const std::vector<Point2D>& points, int firstPointIndex, 
							int lastPointIndex,float tolerance, std::vector<int>& pointIndexsToKeep);
	//Vertex Reduction within tolerance of  prior vertex cluster
	bool vertexReductionwithinTol( float tolerance,std::vector<Point2D>& points);

	void updateAngleInVertexAnglesArray(PolygonVertex* vertex, double angle);

	struct Triplet
	{
		size_t m_indices[3];
	};
	
	std::vector <Triplet*> m_triplets;

	std::vector<PolygonVertex*> m_vertices;
	std::vector<VertexAngle*> m_vertexAngles;

	//sequence of m_cachedExtents is minX, maxX, minY, maxY;
	mutable std::vector<double> m_cachedExtents;

	// Arranges the points in an anticlockwise order
	void SetAnticlockwise();
	
	//Calculates internal angle at the given point
	double CalculateAngle(size_t currentIndex)const;
	
	// Sorts the points in order of increasing internal angles
	void SortAngles();
	
	void CreateTriplet(size_t current, size_t next, size_t previous, size_t currentVertexAngleIndex);

	bool SplitAngleAroundPI();

	size_t GetVertexIndex(const PolygonVertex *polyVertex)const;

	size_t GetAngleIndex(size_t polyVertexIndex)const;

	bool CheckBadTriangle(size_t current, size_t next, size_t previous)const;

	// gets the maximum bounds of the polygon
	void createCachedExtents()const;
	void clearCachedExtents()const;

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Polygon()
	{}
	
	//constructor with points as input
	Polygon(const std::vector<Point2D> &points) 
		: m_polygonPoints(points)
	{}

	//copy constructor
	Polygon(const Polygon &polygon)
	{
		size_t numPoints = polygon.GetNumPoints();
		m_polygonPoints.resize(numPoints);
		for(int i = 0; i < numPoints; i++)
			m_polygonPoints[i] = polygon.GetPoint(i);
	}

	//****************************** destructor *******************************

	// Destructor
	~Polygon();

	//*************************** get/set methods *****************************

	//set the points to polygon
	void Set(const std::vector<Point2D> &points)
	{
		m_polygonPoints = points;
	}

	// adds a point to the polygon
	void AddPoint(const Point2D &point, size_t index);

	// returns the number of points in the polygon
	size_t GetNumPoints()const
	{
		return m_polygonPoints.size();
	}

	// returns the point at the given index
	Point2D GetPoint(int index)const
	{
		assert(index < GetNumPoints());
		return m_polygonPoints[index];
	}

	//returns the array of points
	std::vector<Point2D> GetPointArray()const
	{
		return m_polygonPoints;
	}

	// gives bounding box
	BoundingBox2D GetBoundingBox(double expansionTol)const;

	//**************************general methods********************************

	//checks if the point is in the bounding box
	bool CheckPointInBoundingBox(const Point2D &point)const;

	//check  whether the given point is on the polygon
	bool CheckPointOnPolygon(const Point2D &point)const; 
 
	// checks whether point is inside or outside the polygon
	bool IsPointInPolygon(const Point2D &point)const;
	
	// Checks whether point is inside or outside the polygon
	bool IsPointOutsideOfPolygon(const Point2D &point)const;

	bool IsAntiClockwise()const;
	
	//triangulates the polygon
	bool Triangulate();
	
	size_t GetNumTriangles()const
	{
		return m_triplets.size();
	}
	bool GetTriangleIndices(size_t index, size_t& x, size_t& y, size_t& z)const
	{
		bool stat = false;

		if (index < m_triplets.size())
		{
			x = m_triplets[index]->m_indices[0];
			y = m_triplets[index]->m_indices[1];
			z = m_triplets[index]->m_indices[2];
			stat = true;
		}
		return stat;
	}
	size_t GetPreviousIndex(size_t current)const
	{
		size_t previous;
		if(current == 0)
			previous = ( m_vertices.size()-1);
		else
			previous = current - 1;
		
		return previous;
	}
	
	size_t GetNextIndex(size_t current)const
	{
		size_t next;
		if(current == ( m_vertices.size()-1))
			next = 0;
		else
			next = current + 1;
		
		return next;
	}

	PolygonVertex* GetMaxXvertex(size_t &index)const;

	// get tesselated points
	void Tessellate(std::vector<Point2D> &points, double tessellationTol)const;

	//gives the area of polygon
	double GetArea();

	//offset a polygon by given offset values
	Polygon GetNonUniformOffset(std::vector<double>&offsetValues)const;
	
	//offset a polygon by given offset value
	Polygon GetOuterOffset(double offsetValue)const;

	//offset a side of polygon by given offset value
	Polygon GetOneSideOffset(int index, double offsetValue)const;

	const Point2D &GetNextPoint(int index)const;

	const Point2D &GetPreviousPoint(int index)const;

	//gives the array of all segments of polygon
	void GetSegmentArray(std::vector<Segment2D>& allSegments)const;

	void CleanSmallEdges(double cleanlinessFactor);

	//DEBUG FUNCTION
	void PrintToFile(FILE* fp);

	//dump the data
	void Dump(Report &report, int tabIndent)const;

	//using Douglas Peucker algorithim to reduce the 
	//number of points in polygon
	/*http://geomalgorithms.com/a16-_decimate-1.html*/
	bool ReducePoints(float tolerance, std::vector<Point2D>& reducedPoints);

	void ReverseIndices();

	bool IsAnticlockwise(bool uMinBound = true, bool vMinBound = true,
		bool uMaxBound = false, bool vMaxBound = true,
		bool* intersectingPolygon = 0)const;

	//orients the polygon in anticlockwise sense
	bool Orient(bool antiClockWise);

	bool IsSelfIntersecting(const double tol = CommonConstants::PNT_TOL);
};

}
#endif