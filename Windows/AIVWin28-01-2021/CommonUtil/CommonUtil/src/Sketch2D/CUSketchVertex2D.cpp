#include "CUSketchVertex2D.h"

//------------------------------------------------------------------------------------------------------------------
namespace CommonUtil
{
	SketchVertex2D::SketchVertex2D(const CommonUtil::Point2D& point)
	{
		Set(point.GetX(), point.GetY());
		m_sketchSegment2D[0] = nullptr;
		m_sketchSegment2D[1] = nullptr;
	}

	//------------------------------------------------------------------------------------------------------------------

	SketchVertex2D::~SketchVertex2D()
	{}

	//------------------------------------------------------------------------------------------------------------------

	bool SketchVertex2D::setSketchSegment(SketchSegment2D* sketchSegment2D)
	{
		bool status = true;
		if (!m_sketchSegment2D[0])
			m_sketchSegment2D[0] = sketchSegment2D;
		else if (!m_sketchSegment2D[1])
			m_sketchSegment2D[1] = sketchSegment2D;
		else
			status = false;
		return status;
	}

	//------------------------------------------------------------------------------------------------------------------

	bool SketchVertex2D::removeSketchSegment(SketchSegment2D* sketchSegment2D)
	{
		bool status = true;
		if (m_sketchSegment2D[0] == sketchSegment2D)
		{
			m_sketchSegment2D[0] = m_sketchSegment2D[1];
			m_sketchSegment2D[1] = nullptr;
		}
		else if (m_sketchSegment2D[1] == sketchSegment2D)
			m_sketchSegment2D[1] = nullptr;
		else
			status = false;
		return status;
	}

	//------------------------------------------------------------------------------------------------------------------

	bool SketchVertex2D::isFree()const
	{
		return (!m_sketchSegment2D[0] || !m_sketchSegment2D[1]);
	}

	//------------------------------------------------------------------------------------------------------------------

	bool SketchVertex2D::isInvalid()const
	{
		return (!m_sketchSegment2D[0] && !m_sketchSegment2D[1]);
	}

	//------------------------------------------------------------------------------------------------------------------

	SketchSegment2D* SketchVertex2D::getSketchSegment2D_1()const
	{
		return m_sketchSegment2D[0];
	}

	//------------------------------------------------------------------------------------------------------------------

	SketchSegment2D* SketchVertex2D::getSketchSegment2D_2()const
	{
		return m_sketchSegment2D[1];
	}

	//------------------------------------------------------------------------------------------------------------------

	SketchSegment2D* SketchVertex2D::getOtherSketchSegment2D(const SketchSegment2D* sketchSegment)const
	{
		SketchSegment2D* tempSketchSeg = nullptr;
		if (sketchSegment != m_sketchSegment2D[0])
			tempSketchSeg = m_sketchSegment2D[0];
		else if (sketchSegment != m_sketchSegment2D[1])
			tempSketchSeg = m_sketchSegment2D[1];
		return tempSketchSeg;
	}

	//------------------------------------------------------------------------------------------------------------------

	const SketchSegment2D* SketchVertex2D::GetSketchSegment2D_1()const
	{
		return m_sketchSegment2D[0];
	}

	//------------------------------------------------------------------------------------------------------------------

	const SketchSegment2D* SketchVertex2D::GetSketchSegment2D_2()const
	{
		return m_sketchSegment2D[1];
	}

	//------------------------------------------------------------------------------------------------------------------

	const SketchSegment2D* SketchVertex2D::GetOtherSketchSegment2D(const SketchSegment2D* sketchSegment)const
	{
		return getOtherSketchSegment2D(sketchSegment);
	}
}
//------------------------------------------------------------------------------------------------------------------