#include "CUSketchSegment2D.h"
#include "CUSketchVertex2D.h"

//------------------------------------------------------------------------------------------------------------------
namespace CommonUtil
{
	SketchSegment2D::SketchSegment2D(SketchVertex2D* vertex1, SketchVertex2D* vertex2)
	{
		m_flags = 0;
		m_sketchVertices2D[0] = vertex1;
		m_sketchVertices2D[1] = vertex2;
	}

	//------------------------------------------------------------------------------------------------------------------

	SketchSegment2D::~SketchSegment2D()
	{}

	//------------------------------------------------------------------------------------------------------------------

	SketchVertex2D* SketchSegment2D::getSketchVertex2D_1()const
	{
		return m_sketchVertices2D[0];
	}

	//------------------------------------------------------------------------------------------------------------------

	SketchVertex2D* SketchSegment2D::getSketchVertex2D_2()const
	{
		return m_sketchVertices2D[1];
	}

	//------------------------------------------------------------------------------------------------------------------

	bool SketchSegment2D::replaceVertex(const SketchVertex2D* toBeReplaced, SketchVertex2D* toReplaceWith)
	{
		bool status = false;
		if (toBeReplaced == m_sketchVertices2D[0])
		{
			m_sketchVertices2D[0] = toReplaceWith;
			status = true;
		}
		else if (toBeReplaced == m_sketchVertices2D[1])
		{
			m_sketchVertices2D[1] = toReplaceWith;
			status = true;
		}
		return status;
	}

	//------------------------------------------------------------------------------------------------------------------

	void SketchSegment2D::setChecked(bool flag)
	{
		if (flag)
			m_flags = m_flags || (1<<0);
		else
			m_flags = m_flags && (0<<0);
	}

	//------------------------------------------------------------------------------------------------------------------

	bool SketchSegment2D::isChecked()const
	{		
		return (m_flags && (1 << 0));
	}

	//------------------------------------------------------------------------------------------------------------------

	const SketchVertex2D* SketchSegment2D::GetSketchVertex2D_1()const
	{
		return m_sketchVertices2D[0];
	}

	//------------------------------------------------------------------------------------------------------------------

	const SketchVertex2D* SketchSegment2D::GetSketchVertex2D_2()const
	{
		return m_sketchVertices2D[1];
	}
	
	//------------------------------------------------------------------------------------------------------------------

	const SketchVertex2D* SketchSegment2D::GetOtherSketchVertex2D(const SketchVertex2D* vertex)const
	{
		SketchVertex2D* tempVertex = nullptr;
		if (vertex != m_sketchVertices2D[0])
			tempVertex = m_sketchVertices2D[0];
		else if (vertex != m_sketchVertices2D[1])
			tempVertex = m_sketchVertices2D[1];
		return tempVertex;
	}

	//------------------------------------------------------------------------------------------------------------------

	void SketchSegment2D::GetMidPoint(double& x, double& y)const
	{
		x = (m_sketchVertices2D[0]->GetX() + m_sketchVertices2D[1]->GetX()) / 2;
		y = (m_sketchVertices2D[0]->GetY() + m_sketchVertices2D[1]->GetY()) / 2;
	}

	//------------------------------------------------------------------------------------------------------------------

	float SketchSegment2D::GetLength()const
	{
		return m_sketchVertices2D[0]->Distance(*m_sketchVertices2D[1]);
	}
}
//------------------------------------------------------------------------------------------------------------------