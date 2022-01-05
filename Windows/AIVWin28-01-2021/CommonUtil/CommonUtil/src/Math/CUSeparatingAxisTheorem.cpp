#include"CUSeparatingAxisTheorem.h"
#include "CUvector.h"
#include "CUOctreeCell.h"
#include "CUBoundingBox3D.h"


namespace CommonUtil
{

//Check if RefVec is the axis of seperation for convex bodies 
bool SeparatingAxisTheorem::checkIfRefVectorIsSeperationAxis(double fDist1, double fDist2, double cDist1, double cDist2)
{
	if(cDist1 < cDist2)
	{
		if(((fDist1 < cDist1) && (fDist2 < cDist1)) ||
			((fDist1 > cDist2) && (fDist2 > cDist2)))
		{
			return true;
		}
	}
	else
	{
		if(((fDist1 < cDist2) && (fDist2 < cDist2)) ||
			((fDist1 > cDist1) && (fDist2 > cDist1)))
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------
//Check for edgeVector if crossproduct of edgeVector and standard axis gives seperation axis
bool SeparatingAxisTheorem::checkIfFacetEdgeGivesNonIntersectingAxis(const CommonUtil::Vector& edge, const std::vector<CommonUtil::Point3D> &facetPoints,
															double cMinX, double cMinY, double cMinZ,
															double cMaxX, double cMaxY, double cMaxZ)
{
	CommonUtil::Vector crossProduct;

	double cMinDist = DBL_MAX;
	double cMaxDist = -DBL_MAX;

	double fMinDist = DBL_MAX;
	double fMaxDist = -DBL_MAX;

	for(int i = 0; i < 3; ++i)
	{
		if(i == 0)
			crossProduct.Set(0,edge.GetK(),-edge.GetJ());	//Edge cross xaxis
		else if(i == 1)
			crossProduct.Set(-edge.GetK(),0,edge.GetI());	//Edge cross yaxis
		else if(i == 2)
			crossProduct.Set(edge.GetJ(),-edge.GetI(),0);	//Edge cross zaxis

		if(crossProduct.IsValid())
		{
			getBoxExtentsForParticularVector(cMinX,cMinY,cMinZ,cMaxX,cMaxY,cMaxZ,crossProduct,cMinDist,cMaxDist);
			getFacetExtentsForParticularVector(facetPoints, crossProduct, fMinDist, fMaxDist);

			if(checkIfRefVectorIsSeperationAxis(fMinDist, fMaxDist, cMinDist, cMaxDist))
				return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------

//Check if facet and cell are intersecting
//Here seperating axis theorem is used to check intersection of two convex bodies
//If we can find one axis seperating two convex bodies then bodies are non-intesecting
//Here no. of axis to be checked is 13 and those are
//X,Y,Z,facetNormal
//and cross product of FacetEdge with each of axis
bool SeparatingAxisTheorem::CheckIfFacetAndCellIntersects(const std::vector<float> &facetVertices, const CommonUtil::OctreeCell* octreeCell)
{
	std::vector<CommonUtil::Point3D> facetsPoint;
	facetsPoint.reserve(3);
	facetsPoint.push_back(CommonUtil::Point3D(facetVertices[0], facetVertices[1], facetVertices[2]));
	facetsPoint.push_back(CommonUtil::Point3D(facetVertices[3], facetVertices[4], facetVertices[5]));
	facetsPoint.push_back(CommonUtil::Point3D(facetVertices[6], facetVertices[7], facetVertices[8]));
	double fMinX = 0.0, fMinY = 0.0, fMinZ = 0.0, fMaxX = 0.0, fMaxY = 0.0, fMaxZ = 0.0;
	BoundingBox3D box(facetVertices);
	box.GetExtents(fMinX, fMinY, fMinZ, fMaxX, fMaxY, fMaxZ);
		
	fMinX -= CommonConstants::PNT_TOL; fMinY -= CommonConstants::PNT_TOL; fMinZ -= CommonConstants::PNT_TOL;
	fMaxX += CommonConstants::PNT_TOL; fMaxY += CommonConstants::PNT_TOL; fMaxZ += CommonConstants::PNT_TOL;

	float xMin,xMax,yMin,yMax,zMin,zMax;
	octreeCell->GetExtents(xMin,yMin,zMin,xMax,yMax,zMax);

	//Check for xAxis
	if(!(((fMinX < xMin) && (fMaxX < xMin)) ||
		((fMinX > xMax) && (fMaxX > xMax))))
	{
		
		//Check for yAxis
		if(!(((fMinY < yMin) && (fMaxY < yMin)) ||
			((fMaxY > yMax) && (fMinY > yMax))))
		{
			
			//Check for zAxis
			if(!(((fMinZ < zMin) &&( fMaxZ < zMin)) ||
				((fMaxZ > zMax) && (fMinZ > zMax))))
			{
				//check with facet normal
				double cMinDist = DBL_MAX;
				double cMaxDist = -DBL_MAX;

				double fMinDist = DBL_MAX;
				double fMaxDist = -DBL_MAX;

				CommonUtil::Vector fedge1(facetsPoint[0], facetsPoint[1]);
				CommonUtil::Vector fedge2(facetsPoint[1], facetsPoint[2]);

				CommonUtil::Vector facetNormal;
				fedge1.CrossProduct(fedge2,facetNormal);
				//facetNormal.Normalize();

				getBoxExtentsForParticularVector(xMin,yMin,zMin,xMax,yMax,zMax,facetNormal,cMinDist,cMaxDist);
				getFacetExtentsForParticularVector(facetsPoint, facetNormal, fMinDist, fMaxDist);

				if(!checkIfRefVectorIsSeperationAxis(fMinDist,fMaxDist,cMinDist,cMaxDist))
				{
					//check with vector fEdge1
					if (!checkIfFacetEdgeGivesNonIntersectingAxis(fedge1, facetsPoint, xMin, yMin, zMin, xMax, yMax, zMax))
					{
						//check with vector fEdge2
						if (!checkIfFacetEdgeGivesNonIntersectingAxis(fedge2, facetsPoint, xMin, yMin, zMin, xMax, yMax, zMax))
						{
							//check with vector fEdge3
							CommonUtil::Vector fedge3(facetsPoint[0], facetsPoint[2]);
							if (!checkIfFacetEdgeGivesNonIntersectingAxis(fedge3, facetsPoint, xMin, yMin, zMin, xMax, yMax, zMax))
								return true;
						}
					}
				}
			}
		}
	}

	return false;
}
//-----------------------------------------------------------------------------
//Get projection on normalized vector for corner points of box
void SeparatingAxisTheorem::getBoxExtentsForParticularVector(double cMinX, double cMinY, double cMinZ,
													double cMaxX, double cMaxY, double cMaxZ,
													const CommonUtil::Vector& refVec,double &cMinDist, double &cMaxDist)
{
	double i = refVec.GetI();
	double j = refVec.GetJ();
	double k = refVec.GetK();

	cMinDist = cMinX*i + cMinY*j + cMinZ*k;
	cMaxDist = cMaxX*i + cMaxY*j + cMaxZ*k;

	if(cMinDist > cMaxDist)
	{
		double tmpDist = cMinDist;
		cMinDist = cMaxDist;
		cMaxDist = tmpDist;
	}
	double tempDist = cMaxX*i + cMinY*j + cMinZ*k;

	if(tempDist < cMinDist)
		cMinDist = tempDist;
	else if(tempDist > cMaxDist)
		cMaxDist = tempDist;
	tempDist = cMaxX*i + cMaxY*j + cMinZ*k;

	if(tempDist < cMinDist)
		cMinDist = tempDist;
	else if(tempDist > cMaxDist)
		cMaxDist = tempDist;

	tempDist = cMinX*i + cMaxY*j + cMinZ*k;

	if(tempDist < cMinDist)
		cMinDist = tempDist;
	else if(tempDist > cMaxDist)
		cMaxDist = tempDist;

	tempDist = cMinX*i + cMinY*j + cMaxZ*k;

	if(tempDist < cMinDist)
		cMinDist = tempDist;
	else if(tempDist > cMaxDist)
		cMaxDist = tempDist;

	tempDist = cMinX*i + cMaxY*j + cMaxZ*k;

	if(tempDist < cMinDist)
		cMinDist = tempDist;
	else if(tempDist > cMaxDist)
		cMaxDist = tempDist;

	tempDist = cMaxX*i + cMinY*j + cMaxZ*k;

	if(tempDist < cMinDist)
		cMinDist = tempDist;
	else if(tempDist > cMaxDist)
		cMaxDist = tempDist;
}

//-----------------------------------------------------------------------------
void SeparatingAxisTheorem::getFacetExtentsForParticularVector(const std::vector<CommonUtil::Point3D> &facetPoints, const CommonUtil::Vector& refVec, double &fMinDist, double &fMaxDist)
{
	double i = refVec.GetI();
	double j = refVec.GetJ();
	double k = refVec.GetK();

	double x,y,z;
	x = facetPoints[0].GetX();
	y = facetPoints[0].GetY();
	z = facetPoints[0].GetZ();
	fMinDist = fMaxDist = x*i + y*j + z*k;

	x = facetPoints[1].GetX();
	y = facetPoints[1].GetY();
	z = facetPoints[1].GetZ();
	double tempDist = x*i + y*j + z*k;

	if(fMinDist > tempDist)
		fMinDist = tempDist;
	if(fMaxDist < tempDist)
		fMaxDist = tempDist;

	x = facetPoints[2].GetX();
	y = facetPoints[2].GetY();
	z = facetPoints[2].GetZ();
	tempDist = x*i + y*j + z*k;

	if(fMinDist > tempDist)
		fMinDist = tempDist;
	if(fMaxDist < tempDist)
		fMaxDist = tempDist;
}

//-----------------------------------------------------------------------------

}