// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _COMMONUTIL_LINEFITTING_H_
#define _COMMONUTIL_LINEFITTING_H_

//standard include files
#include <vector>

//geom include files
#include "CUVector.h"

namespace CommonUtil
{

// This class represents a bounding box
class LineFitting
{
	//========================== private data members =========================

	CommonUtil::Point3D m_linePoint;

	CommonUtil::Vector m_lineDirection;

public:

	//======================== public member functions ========================

	//************************** list of constructors *************************

	//constuctor
	LineFitting()
	{}

	LineFitting(const CommonUtil::Point3D &linePoint, 
				const CommonUtil::Vector &lineDirection)
				: m_linePoint(linePoint), 
				  m_lineDirection(lineDirection)
	{}

	//****************************** destructor *******************************

	//destructor
	~LineFitting()
	{}

	//*************************** get/set methods *****************************

	//sets point on the line
	void SetLinePoint(const CommonUtil::Point3D &linePoint)
	{
		m_linePoint = linePoint;
	}

	//sets direction along line
	void SetLineDirection(const CommonUtil::Vector &lineDirection)
	{
		m_lineDirection = lineDirection;
	}

	//returns point on the line
	CommonUtil::Point3D GetLinePoint()const
	{
		return m_linePoint;
	}

	//returns direction of line
	CommonUtil::Vector GetLineDirection()const
	{
		return m_lineDirection;
	}

	//**************************general methods********************************

	//fits line through given data points
	void Execute(const std::vector<CommonUtil::Point3D> &dataPoints);

	//returns average deviation of the points from line
	double GetDeviation(const std::vector<CommonUtil::Point3D> &dataPoints)const;

	void GetExactLine(std::vector<CommonUtil::Point3D> dataPoints, double &deviation);

	void RefineExecute(const std::vector<CommonUtil::Point3D> &dataPoints);

};

}
#endif