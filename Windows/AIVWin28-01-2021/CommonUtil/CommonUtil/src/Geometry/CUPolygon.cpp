// ============================================================================
//			Copyright (c) 2016 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

//standard include files
#include <algorithm>

//geom include files
#include "CUpolygon.h"
#include "CUBoundingBox2D.h"
#include "CUsegment2d.h"
#include "CUtriangle2d.h"

#include "CUCommonConstants.h"
//util include files
#include "CUreport.h"
#include "CUvector.h"
#include "CUSegment2DIntersector.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------
Polygon::~Polygon()
{
	for(size_t i = 0; i < m_triplets.size(); ++i)
	{
		if(m_triplets[i])
			delete m_triplets[i];
	}
	m_triplets.clear();

	for(size_t i = 0; i < m_vertices.size(); ++i)
	{
		delete m_vertices[i]->m_vertexAngle;
		
		if(m_vertices[i])
			delete m_vertices[i];
	}
	m_vertices.clear();

	for(size_t i = 0; i < m_vertexAngles.size(); ++i)
	{
		if(m_vertexAngles[i])
			delete m_vertexAngles[i];
	}
	m_vertexAngles.clear();
}

//-----------------------------------------------------------------------------

void Polygon::getExtents(double &minX, double &maxX,
						 double &minY, double &maxY)const
{
	minX = DBL_MAX;
	minY = DBL_MAX;
	maxX = -DBL_MAX;
	maxY = -DBL_MAX;

	for(int i = 0; i < m_polygonPoints.size(); i++)
	{
		Point2D point = m_polygonPoints[i];
		double xVal = point.GetX();
		double yVal = point.GetY();
	
		if(yVal < minY)
			minY = yVal;

		if(yVal > maxY)
			maxY = yVal;
		
		if(xVal < minX)
			minX = xVal;
		
		if(xVal > maxX)
			maxX = xVal;
	}
}

//-----------------------------------------------------------------------------
// Douglases the peucker reduction
bool Polygon::douglasPeuckerPointReduction(const std::vector<Point2D>& points, int firstPointIndex, 
								int lastPointIndex,float tolerance, std::vector<int>& pointIndexsToKeep)
{
	double maxDistance = 0;
    int indexFarthest = 0;

    for (int index = firstPointIndex; index < lastPointIndex; index++)
    {
		 //The distance of a point from a line made from point1 and point2.

		Point2D Point1 = points[firstPointIndex];
		Point2D Point2 = points[lastPointIndex];
		Point2D Point = points[index];

		/*Vector adjacentSide(points[firstPointIndex],points[lastPointIndex]);
		Vector hypotenuse(points[firstPointIndex],points[index]);

		double dotProduct = adjacentSide.DotProduct(hypotenuse);
		double SqrHypotenuseDistance = hypotenuse.GetSqrMagnitude();
		double sqrProjectedDistance = (SqrHypotenuseDistance - (dotProduct * dotProduct));
        double distance = sqrt(sqrProjectedDistance);*/

		double area = fabs(.5 * (Point1.GetX() * Point2.GetY() + Point2.GetX() *
			Point.GetY() + Point.GetX() * Point1.GetY() - Point2.GetX() *
							Point1.GetY() - Point.GetX() * Point2.GetY() - Point1.GetX() * Point.GetY()));

        double bottom = sqrt(pow(Point1.GetX() - Point2.GetX(), 2) + pow(Point1.GetY() - Point2.GetY(), 2));

        double height = area / bottom * 2;

        double distance = height;

        if (distance > maxDistance)
        {
            maxDistance = distance;
            indexFarthest = index;
        }
    }

    if(maxDistance > tolerance && indexFarthest != 0)
    {
        //Add the largest point that exceeds the tolerance
		pointIndexsToKeep.push_back(indexFarthest);

        douglasPeuckerPointReduction(points, firstPointIndex, indexFarthest, tolerance, pointIndexsToKeep);
        douglasPeuckerPointReduction(points, indexFarthest, lastPointIndex, tolerance, pointIndexsToKeep);
    }

	return true;
}

//-----------------------------------------------------------------------------

bool Polygon::IsAntiClockwise()const
{
	size_t currentIndex = -1;
	
	PolygonVertex *vertex = GetMaxXvertex(currentIndex);	
	
	size_t nextIndex = GetNextIndex(currentIndex);
	size_t previousIndex = GetPreviousIndex(currentIndex);

	PolygonVertex* nextVertex = m_vertices[nextIndex];
	PolygonVertex* previousVertex = m_vertices[previousIndex];

	Point2D currentPoint = vertex->m_point;
	Point2D nextPoint = nextVertex->m_point;
	Point2D previousPoint = previousVertex ->m_point;

	Vector v1(nextPoint.GetX() - currentPoint.GetX(), 
			  nextPoint.GetY() - currentPoint.GetY(),
			  0);

	Vector v2(previousPoint.GetX() - currentPoint.GetX(),
			  previousPoint.GetY() - currentPoint.GetY(),
			  0);

	v1.Normalize();
	v2.Normalize();

	Vector product;
	v1.CrossProduct(v2,product);
	bool antiClockwise = true;

	if(v1.GetSqrMagnitude() > CommonConstants::PNT_TOL)
	{
		if(product.GetK() < 0)
			antiClockwise = false;
	}
	else
	{
		if(previousPoint.GetY() > nextPoint.GetY())
			antiClockwise = false;
	}
	return antiClockwise;
}

//-----------------------------------------------------------------------------
//Vertex Reduction within tolerance of  prior vertex cluster
bool Polygon::vertexReductionwithinTol( float tolerance,std::vector<Point2D>& points)
{
	// start at the beginning
	points.push_back(m_polygonPoints[0]);

	size_t polyPtCount = 0;
	float sqrTol = tolerance * tolerance;
	for(size_t ptCount = 1 ; ptCount < m_polygonPoints.size(); ++ptCount)
	{
		//get distance between first and second point
		Point2D firstPt = m_polygonPoints[ptCount];
		Point2D secondPt = m_polygonPoints[polyPtCount];

		//distance 
		float sqrDistance = (float)firstPt.SqrDistance(secondPt);

		if(sqrDistance < sqrTol)
			continue;

		points.push_back(firstPt);
		polyPtCount = ptCount;
    }

	// finish at the end

	if(polyPtCount < (m_polygonPoints.size() - 1))
	{
		Point2D point = m_polygonPoints[(m_polygonPoints.size() - 1)];
		points.push_back(point);
	}
	return true;
}
//-----------------------------------------------------------------------------

void Polygon::updateAngleInVertexAnglesArray(PolygonVertex* vertex, double angle)
{
	for(size_t count = 0 ; count < m_vertexAngles.size(); ++count)
	{
		if(m_vertexAngles[count]->m_vertex->m_point.SqrDistance(vertex->m_point) <= CommonUtil::CommonConstants::SQR_PNT_TOL)
		{
			m_vertexAngles[count]->m_angle = angle;
			break;
		}
	}
}

//-----------------------------------------------------------------------------

void Polygon::AddPoint(const Point2D &point, size_t index)
{
	//m_polygonPoints.resize(m_polygonPoints.size()+1);
	//m_polygonPoints[m_polygonPoints.size()-1] = point;
	m_polygonPoints.push_back(point);

	PolygonVertex* polygonVertex = new PolygonVertex;
	polygonVertex->m_index = index;
	polygonVertex->m_point = point;
	polygonVertex->m_vertexAngle = new VertexAngle;
	polygonVertex->m_vertexAngle->m_vertex = polygonVertex;

	m_vertices.push_back(polygonVertex);
}

//-----------------------------------------------------------------------------

bool Polygon::CheckPointInBoundingBox(const Point2D &point)const
{
	bool stat = false;
	size_t numPoints = m_polygonPoints.size();

	double maxX = m_polygonPoints[numPoints-1].GetX();
	double maxY = m_polygonPoints[numPoints-1].GetY();

	double minX = maxX;
	double minY = maxY;

	for(int i = 0; i < numPoints; i++)
	{
		double xnew = m_polygonPoints[i].GetX();
        double ynew = m_polygonPoints[i].GetY();

		if(xnew > maxX)
			maxX = xnew;
		if(xnew < minX)
			minX = xnew;
		if(ynew > maxY)
			maxY = ynew;
		if(ynew < minY)
			minY = ynew;
	}
	if((point.GetX() >= minX && point.GetX() <= maxX) &&
			(point.GetY() >= minY && point.GetY() <= maxY))
		stat = true;
	return stat;
}

//-----------------------------------------------------------------------------

bool Polygon::CheckPointOnPolygon(const Point2D &point)const
{
	bool stat = false;

	size_t numPoints = m_polygonPoints.size();
	for(int iSeg = 0; iSeg < numPoints; ++iSeg)
	{
		int firstPtIndex = iSeg;
		int secondPtIndex = iSeg+1;
		if(iSeg == numPoints-1)
			secondPtIndex = 0;

		Segment2D segment(m_polygonPoints[firstPtIndex], m_polygonPoints[secondPtIndex]);
		double uVal;
		double snappingTol = DBL_MAX; // to project point on segment in range 0 to 1 only
		segment.Project(point, snappingTol, uVal);
		Point2D projPoint = segment.GetPointOnCurve(uVal);
		double sqrDist = projPoint.SqrDistance(point);
		if(sqrDist < CommonConstants::ZERO*CommonConstants::ZERO)
		{
			stat = true;
			break;
		}
	}
	return stat;
}
//-----------------------------------------------------------------------------

double Polygon::CalculateAngle(size_t currentIndex)const
{
	size_t next = GetNextIndex(currentIndex);
	size_t previous = GetPreviousIndex(currentIndex);

	Point2D point = m_vertices[currentIndex]->m_point;

	Vector vec1(m_vertices[next]->m_point.GetX() - point.GetX(),
		        m_vertices[next]->m_point.GetY() - point.GetY(),0 );

	Vector vec2(m_vertices[previous]->m_point.GetX() - point.GetX(),
		        m_vertices[previous]->m_point.GetY() - point.GetY(),0 );
	
	vec1.Normalize();
	vec2.Normalize();

	Vector crossProd;
	
	double angle;
	
	vec1.CrossProduct(vec2, crossProd );

	vec1.GetAngle(vec2, angle);
	
	if (crossProd.GetK() < 0)
		angle = ( 2 * CommonConstants::PI ) - angle;

	float floatAngle = (float)angle;
	angle = (double) floatAngle;

	return angle;
}
//-----------------------------------------------------------------------------

void Polygon::CreateTriplet(size_t current, size_t next, size_t previous, size_t currentVertexAngleIndex)
{
	Triplet* trip = new Triplet;

	trip->m_indices[0] = m_vertices[previous]->m_index;
	trip->m_indices[1] = m_vertices[current]->m_index;
	trip->m_indices[2] = m_vertices[next]->m_index;
	m_triplets.push_back(trip);

	if (m_vertices.size() > 3)
	{
		//PolygonVertex* plVertex = m_vertices[current];
		//memory leak
		delete m_vertices[current];
		m_vertices[current] = nullptr;
		//VertexAngle* vertexAngle = m_vertexAngles[currentVertexAngleIndex];
		delete m_vertexAngles[currentVertexAngleIndex];
		m_vertexAngles[currentVertexAngleIndex] = nullptr;
		m_vertices.erase(m_vertices.begin() + current);
		m_vertexAngles.erase(m_vertexAngles.begin() + currentVertexAngleIndex);
		
		previous = current - 1;
		if (!current)
			previous = m_vertices.size()-1;

		next = GetNextIndex(previous);
		
		m_vertices[previous]->m_vertexAngle->m_angle = CalculateAngle(previous);
		updateAngleInVertexAnglesArray(m_vertices[previous], m_vertices[previous]->m_vertexAngle->m_angle);
		m_vertices[next]->m_vertexAngle->m_angle = CalculateAngle(next);
		updateAngleInVertexAnglesArray(m_vertices[next], m_vertices[next]->m_vertexAngle->m_angle);

		SortAngles();
	}
}
//-----------------------------------------------------------------------------

size_t Polygon::GetVertexIndex(const PolygonVertex *polyVertex)const
{
	size_t index = -1;

	size_t numVertices = m_vertices.size();
	for (int ctr = 0; ctr < numVertices; ctr++)
	{
		if (polyVertex == m_vertices[ctr])
		{
			index = ctr;
			break;
		}
	}
	return index;
}

//-----------------------------------------------------------------------------

bool Polygon::CheckBadTriangle(size_t current, size_t next, size_t previous)const
{
	bool status = false;

	PolygonVertex *currentVertex = m_vertices[current];
	PolygonVertex *nextVertex = m_vertices[next];
	PolygonVertex *previousVertex = m_vertices[previous];

	Triangle2D triangle (currentVertex->m_point, nextVertex->m_point, previousVertex->m_point);

	for(int i = 0; i < m_vertices.size(); i++)
	{
		if(i != current && i != previous && i != next)
		{
			double angle = m_vertices[i]->m_vertexAngle->m_angle;
			if(angle > CommonConstants::PI-0.001)
			{
				bool onBoundary;
				bool onVertex;
				bool isBadTringle = triangle.CheckInsideTriangle(m_vertices[i]->m_point, 
																 &onBoundary, &onVertex);

				if(isBadTringle == true)
				{
					status = true;
					break;
				}
			}
		}
	}
	return status;
}
//-----------------------------------------------------------------------------

void Polygon::clearCachedExtents()const
{
	m_cachedExtents.clear();
}

//-----------------------------------------------------------------------------

void Polygon::createCachedExtents()const
{
	if (m_cachedExtents.empty())
	{
		m_cachedExtents.resize(4);
		m_cachedExtents[0] = DBL_MAX;
		m_cachedExtents[1] = -DBL_MAX;
		m_cachedExtents[2] = DBL_MAX;
		m_cachedExtents[3] = -DBL_MAX;

		for (int i = 0; i < m_polygonPoints.size(); i++)
		{
			const Point2D &point = m_polygonPoints[i];
			double xVal = point.GetX();
			double yVal = point.GetY();

			if (xVal < m_cachedExtents[0])
				m_cachedExtents[0] = xVal;

			if (xVal > m_cachedExtents[1])
				m_cachedExtents[1] = xVal;

			if (yVal < m_cachedExtents[2])
				m_cachedExtents[2] = yVal;

			if (yVal > m_cachedExtents[3])
				m_cachedExtents[3] = yVal;
		}
	}
}

//-----------------------------------------------------------------------------

size_t Polygon::GetAngleIndex(size_t polyVertexIndex)const
{
	size_t index = -1;
	PolygonVertex *polyVertex = m_vertices[polyVertexIndex];
	
	for (int i = 0; i < m_vertexAngles.size(); i++)
	{
		//if (fabs(m_vertexAngles[i]->m_angle - polyVertex->m_vertexAngle->m_angle) <= CommonUtil::CommonConstants::PNT_TOL)
		if(polyVertex == m_vertexAngles[i]->m_vertex)
		{
			index = i;
			break;
		}
	}
	return index;
}

//-----------------------------------------------------------------------------

bool Polygon::SplitAngleAroundPI()
{
	bool angleSplit = false;

	for (int angleCtr = 0;
		 angleCtr < m_vertexAngles.size() && m_vertexAngles.size() > 3;
		 angleCtr++)
	{
		bool stat = false;

		VertexAngle *vAngle = m_vertexAngles[angleCtr];
		double angle = vAngle->m_angle;

		if (angle > 175. * CommonConstants::PI/180. && angle < 185. * CommonConstants::PI/180.)
		{
			//angle is large enough, try splitting it
			size_t current = GetVertexIndex(vAngle->m_vertex);

			for (int i = 0; i < 2; i++)
			{
				if (i == 0)
				{
					//try chooping size_t previous angle
					current = GetPreviousIndex(current);
				}
				else
				{
					//try chooping size_t next angle
					current = GetNextIndex(current);
				}

				PolygonVertex *polyVertex = m_vertices[current];
				if (polyVertex->m_vertexAngle->m_angle > 175. * CommonConstants::PI/180.)
				{
					//do not chop this angle
					continue;
				}

				size_t next = GetNextIndex(current);
				size_t previous = GetPreviousIndex(current);

				if (!CheckBadTriangle(current, next, previous))
				{
					size_t angleIndex = GetAngleIndex(current);
					CreateTriplet(current, next, previous, angleIndex);
					stat = true;
					angleSplit = true;
					break;
				}
			}
		}
		if (stat == true)
		{
			//one angle was split, start all over again
			angleCtr = -1;
		}
	}
	return angleSplit;
}

//-----------------------------------------------------------------------------
	
void Polygon::SortAngles()
{
	CmpAngles cmp;
	std::sort (m_vertexAngles.begin(), m_vertexAngles.end(), cmp);
}
//-----------------------------------------------------------------------------

void Polygon::SetAnticlockwise()
{
	size_t currentIndex = -1;
	
	PolygonVertex *vertex = GetMaxXvertex(currentIndex);	
	
	size_t nextIndex = GetNextIndex(currentIndex);
	size_t previousIndex = GetPreviousIndex(currentIndex);

	PolygonVertex* nextVertex = m_vertices[nextIndex];
	PolygonVertex* previousVertex = m_vertices[previousIndex];

	Point2D currentPoint = vertex->m_point;
	Point2D nextPoint = nextVertex->m_point;
	Point2D previousPoint = previousVertex ->m_point;

	Vector v1(nextPoint.GetX() - currentPoint.GetX(), 
			  nextPoint.GetY() - currentPoint.GetY(),
			  0);
	Vector v2(previousPoint.GetX() - currentPoint.GetX(),
			  previousPoint.GetY() - currentPoint.GetY(),
			  0);

	v1.Normalize();
	v2.Normalize();

	Vector product;
	v1.CrossProduct(v2,product);
	bool antiClockwise = true;

	if(v1.GetSqrMagnitude() > CommonConstants::PNT_TOL)
	{
		if(product.GetK() < 0)
			antiClockwise = false;
	}
	else
	{
		if(previousPoint.GetY() > nextPoint.GetY())
			antiClockwise = false;
	}

	if(!antiClockwise)
		std::reverse (m_vertices.begin(), m_vertices.end());
}


//-----------------------------------------------------------------------------

bool Polygon::Triangulate()
{
	bool status = false;

	SetAnticlockwise();
	
	for(int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i]->m_vertexAngle->m_angle = CalculateAngle(i);

		VertexAngle* vertexAngle = new VertexAngle;
		vertexAngle->m_angle = m_vertices[i]->m_vertexAngle->m_angle;
		vertexAngle->m_vertex = m_vertices[i];

		m_vertexAngles.push_back(vertexAngle);
	}
	
	SortAngles();
	bool isBadTriangle = false; 

	SplitAngleAroundPI();

	size_t cur = 0, next = 0, previous = 0;

	while(m_vertices.size() > 3)
	{
		size_t startingSize = m_vertexAngles.size();
		for(int j = 0; j < m_vertexAngles.size(); j++)
		{
			cur = GetVertexIndex(m_vertexAngles[j]->m_vertex);

			next = GetNextIndex(cur);
			previous = GetPreviousIndex(cur);
			
			isBadTriangle = CheckBadTriangle(cur, next, previous);	
			if(isBadTriangle == false)
			{
				CreateTriplet(cur, next, previous, j);
				break;
			}
		}
		if(startingSize == m_vertexAngles.size())
		{
			//No vertex could be chopped
			//erroneaous polygon
			isBadTriangle = true;
			break;
		}
	}
	if(!isBadTriangle)
	{
		if(m_vertices.size() == 3)
		{
			CreateTriplet(1, 2, 0, -1);
			status = true;
		}
	}
	return status;
}
//-----------------------------------------------------------------------------

Polygon::PolygonVertex* Polygon::GetMaxXvertex(size_t &index)const
{
	PolygonVertex *maxVertex = 0;
	double maxX = -DBL_MAX;

	for(int i = 0; i < m_vertices.size(); i++)
	{
		if(m_vertices[i]->m_point.GetX() > maxX)
		{
			maxX = (double) m_vertices[i]->m_point.GetX();
			maxVertex = m_vertices[i];
			index = i;
		}
	}
	return maxVertex;
}

//-----------------------------------------------------------------------------

bool Polygon::IsPointInPolygon(const Point2D &point)const
{
    bool inside = false;
	size_t numPoints = m_polygonPoints.size();

	//This is not a polygon
    if (m_polygonPoints.size() < 3)
	{
		return false;
	}

    double xold = m_polygonPoints[numPoints-1].GetX();
    double yold = m_polygonPoints[numPoints-1].GetY();

	bool insideBox = CheckPointInBoundingBox(point);
	if(!insideBox)
		return false;
	
	bool pointOnPolygon = CheckPointOnPolygon(point);

    for (int i = 0; i < numPoints && !pointOnPolygon; i++) 
	{
		double xnew = m_polygonPoints[i].GetX();
		double ynew = m_polygonPoints[i].GetY();

		if ((xnew < point.GetX()) == (point.GetX() <= xold))
		{
			double numerator = point.GetX() - xnew;
			double denominator = xold - xnew;
			double uVal = 0;
			if(fabs(denominator) > CommonConstants::ZERO)
				uVal = numerator / denominator;
			else
				uVal = 1;
			double cmpYval = ynew + uVal*(yold - ynew);
			if(cmpYval >= point.GetY())
				inside = !inside;
		 }
		 xold = xnew;
		 yold = ynew;
     }
     return inside;
}

//-----------------------------------------------------------------------------

bool Polygon::IsPointOutsideOfPolygon(const Point2D &point)const
{
	bool inside = IsPointInPolygon(point);

	if (inside)
		return false;

	return true;
}

//-----------------------------------------------------------------------------

double Polygon::GetArea()
{
	double area = 0.0;
	size_t numPoints = GetNumPoints();

	for(int i = 0; i < numPoints; i++)
	{
		int next = i+1;
		if(i == numPoints - 1)
			next = 0;

		double tmpArea = 
				(m_polygonPoints[i].GetX() * m_polygonPoints[next].GetY())
				- (m_polygonPoints[i].GetY() * m_polygonPoints[next].GetX());

		area += tmpArea;
	}

	return fabs(0.5 * area);
}

//-----------------------------------------------------------------------------

Polygon Polygon::GetNonUniformOffset(std::vector<double>&offsetValues)const
{
	//Number of offset values should be same as number of points in polygon
	assert(offsetValues.size() == m_polygonPoints.size());

	// Here it is assumed that the polygon orientation is Anticlockwise
	Polygon originalPolygon(*this);

	// find direction
	Vector normal(0, 0, 1);

	int numPoints = originalPolygon.GetNumPoints();
	std::vector<Segment2D> offsetSegments;
	int index = 0;
	double offsetValue = 0.0;

	for (int i = 0; i < numPoints; i++)
	{
		if (i == numPoints - 1)
			index = 0;
		else
			index = i + 1;

		Point2D firstPoint = originalPolygon.GetPoint(i);
		Point2D secondPoint = originalPolygon.GetPoint(index);

		Vector firstSegVector(firstPoint, secondPoint);
		Vector outerDirection;
		firstSegVector.CrossProduct(normal, outerDirection);
		Point3D firstOffsetPoint3d, secondOffsetPoint3d;

		offsetValue = offsetValues[i];		

		outerDirection.GetPointAlongVector(firstPoint, firstOffsetPoint3d, offsetValue);
		outerDirection.GetPointAlongVector(secondPoint, secondOffsetPoint3d, offsetValue);

		Point2D firstOffsetPoint2d(firstOffsetPoint3d.GetX(), firstOffsetPoint3d.GetY());
		Point2D secondOffsetPoint2d(secondOffsetPoint3d.GetX(), secondOffsetPoint3d.GetY());

		Segment2D tempSegment(firstOffsetPoint2d, secondOffsetPoint2d);
		offsetSegments.push_back(tempSegment);
	}

	std::vector<Point2D> offsetPoints;
	offsetPoints.resize(numPoints);
	for (int i = 0; i < numPoints; i++)
	{
		int index;
		if (i == 0)
			index = numPoints - 1;
		else
			index = i - 1;

		Segment2D firstSegment = offsetSegments[index];
		Segment2D secondSegment = offsetSegments[i];
		double snappingTol = CommonUtil::CommonConstants::ZERO;
		double angleTol = 5.*CommonUtil::CommonConstants::PI / 180.;
		Segment2DIntersector intersector(&firstSegment, snappingTol);
		Point2D point;

		intersector.Execute(&secondSegment, angleTol);

		if (!intersector.IsParallel()) // intersecting
		{
			double uValOrgSeg, vValTestSeg;
			intersector.GetuValForTwoSegments(uValOrgSeg, vValTestSeg);

			point = firstSegment.GetPointOnCurve(uValOrgSeg);
		}
		else // segments are parallel
		{
			Point2D point1 = firstSegment.GetStartPoint();
			Point2D point2 = secondSegment.GetEndPoint();

			point.Set((point1.GetX() + point2.GetX()) / 2.0, (point1.GetY() + point2.GetY()) / 2.0);
		}
		offsetPoints[i] = point;
	}


	Polygon offset(offsetPoints);
	return offset;
}

//-----------------------------------------------------------------------------

//offset a polygon by given offset value
Polygon Polygon::GetOuterOffset(double offsetValue)const
{
	std::vector<double>offsetValues;
	for (size_t ptCount = 0; ptCount < m_polygonPoints.size(); ptCount++)
		offsetValues.push_back(offsetValue);
	return GetNonUniformOffset(offsetValues);
}

//-----------------------------------------------------------------------------

//offset a side of polygon by given offset value
Polygon Polygon::GetOneSideOffset(int index, double offsetValue)const
{
	std::vector<double>offsetValues;
	for (size_t ptCount = 0; ptCount < m_polygonPoints.size(); ptCount++)
	{
		if (ptCount == index)
			offsetValues.push_back(offsetValue);
		else
			offsetValues.push_back(0.0);
	}

	return GetNonUniformOffset(offsetValues);
}

//-----------------------------------------------------------------------------

BoundingBox2D Polygon::GetBoundingBox(double expansionTol)const
{
	double xMin = DBL_MAX;
	double yMin = DBL_MAX;
	double xMax = -DBL_MAX;
	double yMax = -DBL_MAX;

	for(int i = 0; i < m_polygonPoints.size(); i++)
	{
		Point2D tempPoint = m_polygonPoints[i];
		double xVal = tempPoint.GetX();
		double yVal = tempPoint.GetY();

		if(yVal < yMin)
			yMin = yVal;

		if(yVal > yMax)
			yMax = yVal;

		if(xVal < xMin)
			xMin = xVal;

		if(xVal > xMax)
			xMax = xVal;
	}
	
	xMin -= expansionTol;
	xMax += expansionTol;
	yMin -= expansionTol;
	yMax += expansionTol;

	return BoundingBox2D(xMin, xMax, yMin, yMax);
}

//-----------------------------------------------------------------------------

void Polygon::Tessellate(std::vector<Point2D> &points,
						 double tessellationTol)const
{
	points.clear();
	for(int i = 0; i < m_polygonPoints.size(); i++)
	{
		points.push_back(m_polygonPoints[i]);
	}

}

//-----------------------------------------------------------------------------

const Point2D &Polygon::GetNextPoint(int index)const
{
	assert(index < GetNumPoints());
	
	int nextIndex = index + 1;
	if(index == GetNumPoints() - 1)
		nextIndex = 0;
	return m_polygonPoints[nextIndex];
}

//------------------------------------------------------------------------

const Point2D &Polygon::GetPreviousPoint(int index)const
{
	assert(index < GetNumPoints());
	
	size_t preIndex = index - 1;
	if(index == 0)
		preIndex = GetNumPoints() - 1;
	return m_polygonPoints[preIndex];
}


//---------------------------------------------------------------------
/*
void Polygon::Orient(bool antiClockWise)
{
	bool status = false;

	//set antiClockWise
	int a, b;
	double minX, maxX, minY, maxY;
	Point2D currentPoint, previousPoint, nextPoint;
	getExtents(minX, maxX, minY, maxY);
	for(int i = 0; i < m_polygonPoints.size(); i++)
	{
		if(m_polygonPoints[i].GetX() == maxX)
		{
			currentPoint = m_polygonPoints[i];
			if(i == 0)
				a = m_polygonPoints.size() - 1;
			else
				a = i - 1;
			previousPoint = m_polygonPoints[a];

			if(i == m_polygonPoints.size() - 1)
				b = 0;
			else
				b = i + 1;

			nextPoint = m_polygonPoints[b];
			Vector v2(currentPoint, nextPoint);
			Vector v1(currentPoint, previousPoint);
			Vector crossProd;
			v1.CrossProduct(v2,crossProd);
			if(crossProd.GetK() < 0)
			{
				//points are anticlockwise
				status = true;
			}
			else if(crossProd.GetK() == 0)
			{
				if(currentPoint.GetY() < nextPoint.GetY() &&
				   currentPoint.GetY() > previousPoint.GetY())
				{
				status = true;
				}
			}
		}
	}
	
	if(antiClockWise != status)
	{
		//set clockwise
		std::reverse(m_polygonPoints.begin(), m_polygonPoints.end());
	}

}
*/
//-----------------------------------------------------------------------------

void Polygon::GetSegmentArray(std::vector<Segment2D>& allSegments)const
{
	allSegments.clear();
	size_t numPoints = m_polygonPoints.size();
	int index = 0;
	for(int i = 0; i < numPoints; i++)
	{
		//Point2D tempPoint1 = m_polygonPoints[i];
		if(i == numPoints - 1)
			index = 0;
		else
			index = i + 1;
		Segment2D tempSegment(m_polygonPoints[i], m_polygonPoints[index]);
		allSegments.push_back(tempSegment);		
	}
}


//-----------------------------------------------------------------------------

void Polygon::CleanSmallEdges(double cleanlinessFactor)
{
	bool pointRemovalPossible = true;
	while(pointRemovalPossible && m_polygonPoints.size() > 3)
	{
		pointRemovalPossible = false;
		double maxLen = -DBL_MAX;
		double minLen = DBL_MAX;
		int m = 0;
		for(int i = 0; i < m_polygonPoints.size(); i++)
		{
			int nextIndex = i+1;
			if (i == m_polygonPoints.size() - 1)
				nextIndex = 0;

			double len = m_polygonPoints[i].Distance(m_polygonPoints[nextIndex]);
			if(len > maxLen)
				maxLen = len;
			if(len < minLen)
			{
				minLen = len;
				m = nextIndex;
			}
		}
		if(minLen/maxLen < cleanlinessFactor)
		{
			m_polygonPoints.erase(m_polygonPoints.begin()+m);
			pointRemovalPossible = true;
		}
	}
}

void Polygon::Dump(Report &report, int tabIndent)const
{
	report.AddMarker(tabIndent);
	
	report.Add(tabIndent, "Polygon\n");
	for(int i = 0; i < m_polygonPoints.size(); i++)
	{
		const Point2D &tmpPoint = m_polygonPoints[i];
		report.Add(tabIndent+1, "%f,%f,0.0\n", tmpPoint.GetX(), tmpPoint.GetY());
	}
	report.AddMarker(tabIndent);
}

//-----------------------------------------------------------------------------

//DEBUG START 
void Polygon::PrintToFile(FILE* fp)
{
	for(int i = 0; i < m_polygonPoints.size(); i++)
	{
		const Point2D &tmpPoint = m_polygonPoints[i];
		fprintf(fp, "\n%d point x: %f y: %f",i, tmpPoint.GetX(),
												tmpPoint.GetY());
	}
}
//DEBUG END 

//-----------------------------------------------------------------------------
//using Douglas Peucker algorithim to reduce the number of points in polygon
bool Polygon::ReducePoints(float tolerance, std::vector<Point2D>& reducedPoints)
{
	if (m_polygonPoints.size() < 3)
            return false;

	//vertex reduction 
	std::vector<Point2D> vertexReducedPtInPolygon;
	vertexReducedPtInPolygon.reserve(m_polygonPoints.size());

	vertexReductionwithinTol(tolerance,vertexReducedPtInPolygon);

	bool stat = false;

	int firstPointIndex = 0;
	int lastPointIndex = (int)(vertexReducedPtInPolygon.size() - 1);

	std::vector<int> pointIndexsToKeep;
	pointIndexsToKeep.reserve(lastPointIndex);

	//Add the first and last index to the keepers
	pointIndexsToKeep.push_back(firstPointIndex);
	pointIndexsToKeep.push_back(lastPointIndex);

	//The first and the last point can not be the same
	while (vertexReducedPtInPolygon[firstPointIndex].IsEqual(vertexReducedPtInPolygon[lastPointIndex]))
	{
		lastPointIndex--;
	} 

	//reducer function
	douglasPeuckerPointReduction(vertexReducedPtInPolygon,firstPointIndex,lastPointIndex,tolerance,pointIndexsToKeep);

	//sort polygon points based on a point index
	//sort the array
	int temp = 0;
    for(int i=0; i<pointIndexsToKeep.size(); ++i)
    {
        for(int j=0; j<pointIndexsToKeep.size(); ++j)
        {
            if(pointIndexsToKeep[i]<pointIndexsToKeep[j])
            {
                temp = pointIndexsToKeep[i];
                pointIndexsToKeep[i] = pointIndexsToKeep[j];
                pointIndexsToKeep[j] = temp;
            }
        }
    }

	for(int indexCount = 0; indexCount < pointIndexsToKeep.size(); ++indexCount)
	{
		int index = pointIndexsToKeep[indexCount];
		reducedPoints.push_back(vertexReducedPtInPolygon[index]);
	}

	return stat;
}

//-----------------------------------------------------------------------------

void Polygon::ReverseIndices()
{
	size_t numIndex = 0;

	for(int i = m_vertices.size()-1; i >= 0; --i)
	{
		m_vertices[i]->m_index = numIndex;
		++numIndex;
	}
}

//-----------------------------------------------------------------------------

bool Polygon::IsAnticlockwise(bool uMinBound, bool vMinBound,
	bool uMaxBound, bool vMaxBound,
	bool* intersectingPolygon)const
{
	bool senceUMax = false;
	bool senceVMax = false;
	bool senceUMin = false;
	bool senceVMin = false;

	createCachedExtents();
	if (uMaxBound == false)
	{
		for (int i = 0; i < m_polygonPoints.size(); i++)
		{
			if (m_polygonPoints[i].GetX() == m_cachedExtents[1])
			{
				const Point2D &currentPoint = m_polygonPoints[i];
				const Point2D &nextPoint = GetNextPoint(i);
				const Point2D &previousPoint = GetPreviousPoint(i);

				Vector v2(currentPoint, nextPoint);
				Vector v1(currentPoint, previousPoint);

				if (!v1.IsValid())
				{
					int prevIndex = i - 1;
					if (prevIndex < 0)
						prevIndex = (int)(m_polygonPoints.size() - 1);
					v1.Set(currentPoint, GetPreviousPoint(prevIndex));

					if (!v1.IsValid())
						return false;
				}

				if (!v2.IsValid())
				{
					int nextIndex = i + 1;
					if (nextIndex == (int)m_polygonPoints.size())
						nextIndex = 0;
					v2.Set(currentPoint, GetNextPoint(nextIndex));

					if (!v2.IsValid())
						return false;
				}

				Vector crossProd;
				v1.CrossProduct(v2, crossProd);

				if (crossProd.GetK() < 0)
				{
					//points are anticlockwise
					senceUMax = true;
					break;
				}
				else if (crossProd.GetK() == 0)
				{
					if (currentPoint.GetY() < nextPoint.GetY() &&
						currentPoint.GetY() > previousPoint.GetY())
					{
						senceUMax = true;
						break;
					}
				}
			}
		}
	}
	if (vMaxBound == false)
	{
		for (int i = 0; i < m_polygonPoints.size(); i++)
		{
			if (m_polygonPoints[i].GetY() == m_cachedExtents[3])
			{
				const Point2D &currentPoint = m_polygonPoints[i];
				const Point2D &nextPoint = GetNextPoint(i);
				const Point2D &previousPoint = GetPreviousPoint(i);

				Vector v2(currentPoint, nextPoint);
				Vector v1(currentPoint, previousPoint);

				if (!v1.IsValid())
				{
					int prevIndex = i - 1;
					if (prevIndex < 0)
						prevIndex = (int)(m_polygonPoints.size() - 1);
					v1.Set(currentPoint, GetPreviousPoint(prevIndex));

					if (!v1.IsValid())
						return false;
				}

				if (!v2.IsValid())
				{
					int nextIndex = i + 1;
					if (nextIndex == (int)m_polygonPoints.size())
						nextIndex = 0;
					v2.Set(currentPoint, GetNextPoint(nextIndex));

					if (!v2.IsValid())
						return false;
				}

				Vector crossProd;
				v1.CrossProduct(v2, crossProd);

				if (crossProd.GetK() < 0)
				{
					//points are anticlockwise
					senceVMax = true;
					break;
				}
				else if (crossProd.GetK() == 0)
				{
					if (currentPoint.GetX() > nextPoint.GetX() &&
						currentPoint.GetX() < previousPoint.GetX())
					{
						senceVMax = true;
						break;
					}
				}
			}
		}
	}
	if (uMinBound == false)
	{
		for (int i = 0; i < m_polygonPoints.size(); i++)
		{
			if (m_polygonPoints[i].GetX() == m_cachedExtents[0])
			{
				const Point2D &currentPoint = m_polygonPoints[i];
				const Point2D &nextPoint = GetNextPoint(i);
				const Point2D &previousPoint = GetPreviousPoint(i);

				Vector v2(currentPoint, nextPoint);
				Vector v1(currentPoint, previousPoint);

				if (!v1.IsValid())
				{
					int prevIndex = i - 1;
					if (prevIndex < 0)
						prevIndex = (int)(m_polygonPoints.size() - 1);
					v1.Set(currentPoint, GetPreviousPoint(prevIndex));

					if (!v1.IsValid())
						return false;
				}

				if (!v2.IsValid())
				{
					int nextIndex = i + 1;
					if (nextIndex == (int)m_polygonPoints.size())
						nextIndex = 0;
					v2.Set(currentPoint, GetNextPoint(nextIndex));

					if (!v2.IsValid())
						return false;
				}

				Vector crossProd;
				v1.CrossProduct(v2, crossProd);

				if (crossProd.GetK() < 0)
				{
					//points are anticlockwise
					senceUMin = true;
					break;
				}
				else if (crossProd.GetK() == 0)
				{
					if (currentPoint.GetY() > nextPoint.GetY() &&
						currentPoint.GetY() < previousPoint.GetY())
					{
						senceUMin = true;
						break;
					}
				}
			}
		}
	}
	if (vMinBound == false)
	{
		for (int i = 0; i < m_polygonPoints.size(); i++)
		{
			if (m_polygonPoints[i].GetY() == m_cachedExtents[2])
			{
				const Point2D &currentPoint = m_polygonPoints[i];
				const Point2D &nextPoint = GetNextPoint(i);
				const Point2D &previousPoint = GetPreviousPoint(i);

				Vector v2(currentPoint, nextPoint);
				Vector v1(currentPoint, previousPoint);

				if (!v1.IsValid())
				{
					int prevIndex = i - 1;
					if (prevIndex < 0)
						prevIndex = (int)(m_polygonPoints.size() - 1);
					v1.Set(currentPoint, GetPreviousPoint(prevIndex));

					if (!v1.IsValid())
						return false;
				}

				if (!v2.IsValid())
				{
					int nextIndex = i + 1;
					if (nextIndex == (int)m_polygonPoints.size())
						nextIndex = 0;
					v2.Set(currentPoint, GetNextPoint(nextIndex));

					if (!v2.IsValid())
						return false;
				}

				Vector crossProd;
				v1.CrossProduct(v2, crossProd);

				if (crossProd.GetK() < 0)
				{
					//points are anticlockwise
					senceVMin = true;
					break;
				}
				else if (crossProd.GetK() == 0)
				{
					if (currentPoint.GetX() < nextPoint.GetX() &&
						currentPoint.GetX() > previousPoint.GetX())
					{
						senceVMin = true;
						break;
					}
				}
			}
		}
	}

	if (uMinBound == false)
	{
		if (vMinBound == false && uMaxBound == false && vMaxBound == false)
		{
			if (senceUMin == senceVMin && senceVMin == senceUMax && senceUMax == senceVMax)
				return senceVMin;
			else
				*intersectingPolygon = true;
		}
		else if (vMinBound == true && uMaxBound == false && vMaxBound == false)
		{
			if (senceUMin == senceUMax && senceUMax == senceVMax)
				return senceUMax;
			else
				*intersectingPolygon = true;
		}
		else if (vMinBound == true && uMaxBound == true && vMaxBound == false)
		{
			if (senceUMin == senceVMax)
				return senceUMax;
			else
				*intersectingPolygon = true;
		}
		else if (vMinBound == false && uMaxBound == true && vMaxBound == false)
		{
			if (senceUMin == senceVMin && senceVMin == senceVMax)
				return senceVMin;
			else
				*intersectingPolygon = true;
		}
		else if (vMinBound == false && uMaxBound == true && vMaxBound == true)
		{
			if (senceUMin == senceVMin)
				return senceVMin;
			else
				*intersectingPolygon = true;
		}
		else if (vMinBound == false && uMaxBound == false && vMaxBound == true)
		{
			if (senceUMin == senceVMin && senceVMin == senceUMax)
				return senceUMax;
			else
				*intersectingPolygon = true;
		}
		else
		{
			return senceUMax;
		}
	}
	else if (uMinBound == true)
	{
		if (vMinBound == false && uMaxBound == false && vMaxBound == false)
		{
			if (senceVMin == senceUMax && senceUMax == senceVMax)
				return senceVMin;
			else
				*intersectingPolygon = true;
		}
		else if (vMinBound == true && uMaxBound == false && vMaxBound == false)
		{
			if (senceUMax == senceVMax)
				return senceUMax;
			else
				*intersectingPolygon = true;
		}
		else if (vMinBound == true && uMaxBound == true && vMaxBound == false)
		{
			return senceVMax;
		}
		else if (vMinBound == false && uMaxBound == true && vMaxBound == false)
		{
			if (senceVMin == senceVMax)
				return senceVMin;
			else
				*intersectingPolygon = true;
		}
		else if (vMinBound == false && uMaxBound == true && vMaxBound == true)
		{
			return senceVMin;
		}
		else if (vMinBound == false && uMaxBound == false && vMaxBound == true)
		{
			if (senceVMin == senceUMax)
				return senceVMin;
			else
				*intersectingPolygon = true;
		}
	}

	return senceUMax;
}

//-----------------------------------------------------------------------------

bool Polygon::Orient(bool antiClockWise) // from traker
{
	bool status = IsAnticlockwise();

	if (antiClockWise != status)
	{
		//set clockwise
		std::reverse(m_polygonPoints.begin(), m_polygonPoints.end());
	}
	return status;
}

//-----------------------------------------------------------------------------

bool Polygon::IsSelfIntersecting(const double tol)
{
	bool isSelfIntersecting = false;
	bool isExactIntersection = false;
	bool isBoundaryIntersection = false;
	bool isParallel = false;

	std::vector<Segment2D> segmentArray;
	GetSegmentArray(segmentArray);

	Segment2D seg1, seg2;
	size_t numSegments = segmentArray.size();
	
	for (size_t iSeg = 0; iSeg < numSegments; ++iSeg)
	{
		seg1 = segmentArray[iSeg];

		for (size_t jSeg = iSeg + 1; jSeg < numSegments; ++jSeg)
		{
			seg2 = segmentArray[jSeg];

			isSelfIntersecting = seg1.IsIntersectingNew(seg2, isExactIntersection, isBoundaryIntersection, isParallel, tol);

			if (isExactIntersection)
				break;
		}

		if (isExactIntersection)
			break;
	}
		
	return isExactIntersection;
}

//-----------------------------------------------------------------------------

}