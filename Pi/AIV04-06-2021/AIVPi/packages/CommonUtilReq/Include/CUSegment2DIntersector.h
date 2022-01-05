// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _ESSENCE_SEGMENT2DINTERSECTOR_H_
#define _ESSENCE_SEGMENT2DINTERSECTOR_H_

//standard include files
#include<vector>

//math include files
#include "CUVector.h"

namespace CommonUtil
{
	//forward declaration
	class Segment2D;
}

namespace CommonUtil
{

//This class implements method of finding intersection of two segment
class Segment2DIntersector	
{

public :

	enum intersectionStatus
	{
		PARALLEL,
		INTERSECTING,
		NONINTERSECTING,
		UNKNOWN
	};

private :

	//========================== private data members =========================

	CommonUtil::Segment2D* m_segment;
	CommonUtil::Segment2D* m_testSegment;

	intersectionStatus m_status;

	double m_snappingTol;
	double m_uValOrgSeg;
	double m_uValTestSeg;

	bool m_isIntersecting;
	bool m_isParallel;
	bool m_connectedEndtoEnd;
	bool m_isOverlap;
	bool m_colinear;

	//======================= private members functions =======================

	//calculates the uVal
	void setIntersection();

	//checks parallelism and sets the bool
	bool checkParallel(CommonUtil::Vector::Direction &direction, double angleTol);
	

public :

	//======================== public member functions ========================


	//************************** list of constructors *************************

	//defualt constructor
	Segment2DIntersector(CommonUtil::Segment2D* segment, double snappingTol);

	//****************************** destructor *******************************

	// Destructor
	~Segment2DIntersector()
	{}

	//*************************** get/set methods *****************************

	//get the uVal
	void GetuValForTwoSegments(double &uValOrgSeg, double &vValTestSeg)const;

	//checks end to end connection and sets the bool
	void SetEndtoEndConnected();

	//**************************general methods********************************

	//checks overlap and sets the bool
	void CheckOverlap(double overlappingTol, double &uVal1, double &vVal1,
					  double &uVal2, double &vVal2);

	//checks that the segments are in one line or not
	bool CheckColinear();

	//returns the bool for parallel
	bool IsParallel()const;

	//returns the bool for  end to end connection 
	bool IsConnectedEndtoEnd()const;

	//returns the bool for overlap
	bool IsOverlap()const;

	//returns the bool for colinearity
	bool IsColinear()const;

	bool Execute(CommonUtil::Segment2D* testSegment, double angleTol);

};

}

#endif

