// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================
#ifndef _CU_SEPERATING_AXIS_H_
#define _CU_SEPERATING_AXIS_H_
#include "CUpoint3d.h"
namespace CommonUtil
{
class Octree;
class OctreeCell;
class Vector;
//class Point3D;


class SeparatingAxisTheorem
{
	bool checkIfRefVectorIsSeperationAxis(double fDist1, double fDist2, double cDist1, double cDist2);

	bool checkIfFacetEdgeGivesNonIntersectingAxis(const CommonUtil::Vector& edge, const std::vector<CommonUtil::Point3D> &facetPoints,
													double cMinX, double cMinY, double cMinZ,
													double cMaxX, double cMaxY, double cMaxZ);

	void getBoxExtentsForParticularVector(double cMinX, double cMinY, double cMinZ,
											double cMaxX, double cMaxY, double cMaxZ,
											const CommonUtil::Vector& refVec, double &minDist, double &maxDist);

	void getFacetExtentsForParticularVector(const std::vector<CommonUtil::Point3D> &facetPoints, const CommonUtil::Vector& refVec, double &fMinDist, double &fMaxDist);

public:

	SeparatingAxisTheorem(){}

	~SeparatingAxisTheorem(){}

	//Check if facet and cell are intersecting
	//assumed that facet is only in triangular shape
	bool CheckIfFacetAndCellIntersects(const std::vector<float> &facetVertices, const CommonUtil::OctreeCell* octreeCell);
};
}
#endif