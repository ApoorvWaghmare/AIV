#ifndef _COSTELEMENT_H_
#define _COSTELEMENT_H_

#include "CUMatrix.h"
#include "CUSqMatrix.h"

namespace CommonUtil
{

struct CostElement
{
	// QMatrix
	CommonUtil::SqMatrix m_QMatrix;
	
	// CMatrix
	CommonUtil::Matrix m_CMatrix;

	// Constant
	double m_constant;

	enum Axis
	{
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		UNKNOWN
	};

	Axis m_axis;

public:

	CostElement(const CommonUtil::SqMatrix& q, 
						const CommonUtil::Matrix& c, double d, Axis axis)
	{
		m_QMatrix = q;
		m_CMatrix = c;
		m_constant = d;
		m_axis = axis;
	}
	CostElement()
	{
		m_constant = 0.0;
		m_axis = UNKNOWN;
	}

	~CostElement()
	{}

	// Multiplies all the elements by given scaler
	void Multiply(double scaler);
};

}
#endif