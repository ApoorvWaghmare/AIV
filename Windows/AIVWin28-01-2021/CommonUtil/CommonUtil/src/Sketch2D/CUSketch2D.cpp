#include "CUSketch2D.h"
#include "CUSketchVertex2D.h"
#include "CUSketchSegment2D.h"
#include "CUSketchCircle2D.h"

//------------------------------------------------------------------------------------------------------------------
namespace CommonUtil
{
	Sketch2D::Sketch2D(CommonUtil::Plane& sketchPlane, CommonUtil::Vector& sketchPlaneXAxis)
	{
		m_sketchPlane = sketchPlane;
		m_sketchPlaneXAxis = sketchPlaneXAxis;
		m_sketchPlaneXAxis.Normalize();
		m_sketchPlane.GetNormal().CrossProduct(m_sketchPlaneXAxis, m_sketchPlaneYAxis);
		m_sketchPlaneYAxis.Normalize();
	}

	//------------------------------------------------------------------------------------------------------------------

	Sketch2D::~Sketch2D()
	{
		size_t numVertices = m_sketchVertices.size();
		for (size_t vCnt = 0; vCnt < numVertices; ++vCnt)
		{
			if (m_sketchVertices[vCnt])
				delete m_sketchVertices[vCnt];
		}
		size_t numSegments = m_sketchSegments.size();
		for (size_t sCnt = 0; sCnt < numSegments; ++sCnt)
		{
			if (m_sketchSegments[sCnt])
				delete m_sketchSegments[sCnt];
		}
		size_t numCircles = m_sketchCircles.size();
		for (size_t cCnt = 0; cCnt < numCircles; ++cCnt)
		{
			if (m_sketchCircles[cCnt])
				delete m_sketchCircles[cCnt];
		}
	}

	//------------------------------------------------------------------------------------------------------------------

	SketchVertex2D* Sketch2D::createSketchVertex2D(const CommonUtil::Point2D& vertex, double tol)
	{
		SketchVertex2D* sketchVertex2D = nullptr;
		size_t numVertices = m_sketchVertices.size();
		double distance = 0.0;
		for (size_t vCnt = 0; vCnt < numVertices; ++vCnt)
		{
			distance = m_sketchVertices[vCnt]->Distance(vertex);
			if (distance <= tol)
			{
				if (m_sketchVertices[vCnt]->isFree())
				{
					sketchVertex2D = m_sketchVertices[vCnt];
					break;
				}
			}
		}
		if (!sketchVertex2D)
		{
			sketchVertex2D = new SketchVertex2D(vertex);
			m_sketchVertices.push_back(sketchVertex2D);
		}
		return sketchVertex2D;
	}

	//------------------------------------------------------------------------------------------------------------------

	bool Sketch2D::removeSketchVertex2D(SketchVertex2D* vertex)
	{
		bool status = false;
		size_t numVertices = m_sketchVertices.size();
		for (size_t vCnt = 0; vCnt < numVertices; ++vCnt)
		{
			if (m_sketchVertices[vCnt] == vertex)
			{
				if (m_sketchVertices[vCnt]->isInvalid())
				{
					delete m_sketchVertices[vCnt];
					m_sketchVertices.erase(m_sketchVertices.begin() + vCnt);
					status = true;
				}
				break;
			}
		}
		return status;
	}

	//------------------------------------------------------------------------------------------------------------------

	void Sketch2D::uncheckAllSketchSegment2DFlags()
	{
		size_t numSegments = m_sketchSegments.size();
		for (size_t sCnt = 0; sCnt < numSegments; ++sCnt)
		{
			m_sketchSegments[sCnt]->setChecked(false);
		}
	}

	//------------------------------------------------------------------------------------------------------------------

	SketchSegment2D* Sketch2D::CreateSketchSegment2D(const CommonUtil::Point2D& vertex1, const CommonUtil::Point2D& vertex2, double tol)
	{
		SketchSegment2D* sketchSeg = nullptr;
		SketchVertex2D* sketchVer1 = createSketchVertex2D(vertex1, tol);
		SketchVertex2D* sketchVer2 = createSketchVertex2D(vertex2, tol);
		if (sketchVer1 && sketchVer2)
		{
			sketchSeg = new SketchSegment2D(sketchVer1, sketchVer2);
			m_sketchSegments.push_back(sketchSeg);
			sketchVer1->setSketchSegment(sketchSeg);
			sketchVer2->setSketchSegment(sketchSeg);
		}
		return sketchSeg;
	}

	//------------------------------------------------------------------------------------------------------------------
	
	bool Sketch2D::DeleteSketchSegment2D(SketchSegment2D* sketchSegment2D)
	{
		bool status = false;
		if (sketchSegment2D)
		{
			SketchVertex2D* sketchVer = sketchSegment2D->getSketchVertex2D_1();
			if (sketchVer)
			{
				sketchVer->removeSketchSegment(sketchSegment2D);
				if(sketchVer->isInvalid())
					removeSketchVertex2D(sketchVer);
				sketchVer = nullptr;
			}
			sketchVer = sketchSegment2D->getSketchVertex2D_2();
			if (sketchVer)
			{
				sketchVer->removeSketchSegment(sketchSegment2D);
				if (sketchVer->isInvalid())
					removeSketchVertex2D(sketchVer);
			}
			size_t numSegments = m_sketchSegments.size();
			for (size_t sCnt = 0; sCnt < numSegments; ++sCnt)
			{
				if (m_sketchSegments[sCnt] == sketchSegment2D)
				{
					delete m_sketchSegments[sCnt];
					m_sketchSegments.erase(m_sketchSegments.begin() + sCnt);
					status = true;
					break;
				}
			}
		}
		return status;
	}

	//------------------------------------------------------------------------------------------------------------------

	bool Sketch2D::GetSketchVertex2Ds(double x, double y, std::vector<SketchVertex2D*>vertices)const
	{
		bool status = false;
		size_t numVertices = m_sketchVertices.size();
		const CommonUtil::Point2D point(x, y);
		double distance = 0.0;
		for (size_t vCnt = 0; vCnt < numVertices; ++vCnt)
		{
			distance = m_sketchVertices[vCnt]->Distance(point);
			if (distance <= CommonConstants::SQR_PNT_TOL)
				vertices.push_back(m_sketchVertices[vCnt]);
		}
		if (!vertices.empty())
			status = true;
		return status;
	}

	//------------------------------------------------------------------------------------------------------------------

	const std::vector<SketchSegment2D*>& Sketch2D::GetSketchSegment2Ds()const
	{
		return m_sketchSegments;
	}

	//------------------------------------------------------------------------------------------------------------------

	const CommonUtil::Plane& Sketch2D::GetPlane()const
	{
		return m_sketchPlane;
	}

	//------------------------------------------------------------------------------------------------------------------

	void Sketch2D::GetAxes(CommonUtil::Vector& sketchPlaneXAxis, CommonUtil::Vector& sketchPlaneYAxis)const
	{
		sketchPlaneXAxis = m_sketchPlaneXAxis;
		sketchPlaneYAxis = m_sketchPlaneYAxis;
	}

	//------------------------------------------------------------------------------------------------------------------

	bool Sketch2D::UpdateSketchVertex2D(const SketchVertex2D* sketchVertex2D, double x, double y, bool replaceIfExist)
	{
		bool status = false;
		SketchVertex2D* tempVertex = nullptr;
		size_t numVertices1 = m_sketchVertices.size();
		for (size_t vCnt = 0; vCnt < numVertices1; ++vCnt)
		{
			if (sketchVertex2D == m_sketchVertices[vCnt])
			{
				tempVertex = m_sketchVertices[vCnt];
				break;
			}
		}
		if (replaceIfExist && sketchVertex2D->isFree())
		{
			std::vector<SketchVertex2D*>vertices;
			GetSketchVertex2Ds(x, y, vertices);
			size_t numVertices2 = vertices.size();
			for (size_t vCnt = 0; vCnt < numVertices2; ++vCnt)
			{
				if (sketchVertex2D == vertices[vCnt])
					continue;
				if (vertices[vCnt]->isFree())
				{
					CommonUtil::Point2D pt(x, y);
					const SketchVertex2D* ver1 = vertices[vCnt]->GetSketchSegment2D_1()->GetOtherSketchVertex2D(vertices[vCnt]);
					const SketchVertex2D* ver2 = sketchVertex2D->GetSketchSegment2D_1()->GetOtherSketchVertex2D(sketchVertex2D);
					CommonUtil::Point2D pt1(ver1->GetX(), ver1->GetY());
					CommonUtil::Point2D pt2(ver2->GetX(), ver2->GetY());
					CommonUtil::Vector vect1(pt, pt1);
					CommonUtil::Vector vect2(pt, pt2);
					vect1.Normalize();
					vect2.Normalize();
					float dotProduct = vect1.DotProduct(vect2);
					if (-CommonConstants::SQR_PNT_TOL < dotProduct &&
						dotProduct < CommonConstants::SQR_PNT_TOL)
					{
						vertices[vCnt]->setSketchSegment(sketchVertex2D->getSketchSegment2D_1());
						sketchVertex2D->getSketchSegment2D_1()->replaceVertex(sketchVertex2D, vertices[vCnt]);
						removeSketchVertex2D(tempVertex);
						status = true;
						break;
					}
				}
			}
		}
		if (!status)
		{
			if (tempVertex)
			{
				tempVertex->Set(x, y);
				status = true;
			}
		}
		return status;
	}

	//------------------------------------------------------------------------------------------------------------------

	void Sketch2D::GetBoundingBox2D(double& xMin, double& yMin, double& xMax, double& yMax)const
	{
		xMin = DBL_MAX;
		yMin = DBL_MAX;
		xMax = -DBL_MAX;
		yMax = -DBL_MAX;
		size_t numVertices = m_sketchVertices.size();
		for (size_t vCnt = 0; vCnt < numVertices; ++vCnt)
		{
			if (m_sketchVertices[vCnt]->GetX() < xMin)
				xMin = m_sketchVertices[vCnt]->GetX();
			if (m_sketchVertices[vCnt]->GetX() > xMax)
				xMax = m_sketchVertices[vCnt]->GetX();
			if (m_sketchVertices[vCnt]->GetY() < yMin)
				yMin = m_sketchVertices[vCnt]->GetY();
			if (m_sketchVertices[vCnt]->GetY() > yMax)
				yMax = m_sketchVertices[vCnt]->GetY();
		}
	}

	//------------------------------------------------------------------------------------------------------------------

	bool Sketch2D::AnalysePolylines(std::vector<std::vector<const CommonUtil::SketchVertex2D*>>&polygons, bool closed)
	{
		bool status = false;
		polygons.clear();
		size_t numSegments = m_sketchSegments.size();
		uncheckAllSketchSegment2DFlags();
		for (size_t sCnt = 0; sCnt < numSegments; ++sCnt)
		{
			std::vector<const CommonUtil::SketchVertex2D*>tempVertices;
			if (m_sketchSegments[sCnt]->isChecked())
				continue;
			tempVertices.push_back(m_sketchSegments[sCnt]->GetSketchVertex2D_1());
			tempVertices.push_back(m_sketchSegments[sCnt]->GetSketchVertex2D_2());
			size_t cnt = 1;
			SketchSegment2D* tempSketchSeg2D1 = nullptr;
			tempSketchSeg2D1 = m_sketchSegments[sCnt];
			while (1)
			{
				tempSketchSeg2D1->setChecked(true);
				if (closed && (tempVertices[0] == tempVertices[tempVertices.size() - 1]))
					break;

				SketchSegment2D* tempSketchSeg2D2 = tempVertices[cnt]->getOtherSketchSegment2D(tempSketchSeg2D1);
				if (!tempSketchSeg2D2)
					break;
				if (tempSketchSeg2D2->GetSketchVertex2D_1() != tempVertices[cnt])
					tempVertices.push_back(tempSketchSeg2D2->GetSketchVertex2D_1());
				else if (tempSketchSeg2D2->GetSketchVertex2D_2() != tempVertices[cnt])
					tempVertices.push_back(tempSketchSeg2D2->GetSketchVertex2D_2());
				else
					break;
				cnt++;
				tempSketchSeg2D1 = tempSketchSeg2D2;
			}
			if (!tempVertices.empty())
			{
				polygons.push_back(tempVertices);
				status = true;
			}
		}
		return status;
	}

	//------------------------------------------------------------------------------------------------------------------

	SketchCircle2D* Sketch2D::CreateSketchCircle2D(const CommonUtil::Point2D& center, float radius)
	{
		SketchCircle2D* sketchCircle2D = new SketchCircle2D(center, radius);
		m_sketchCircles.push_back(sketchCircle2D);
		return sketchCircle2D;
	}

	//------------------------------------------------------------------------------------------------------------------

	bool Sketch2D::DeleteSketchCircle2D(SketchCircle2D* sketchCircle2D)
	{
		bool status = false;
		size_t numCircles = m_sketchCircles.size();
		for (size_t cCnt = 0; cCnt < numCircles; ++cCnt)
		{
			if (m_sketchCircles[cCnt] == sketchCircle2D)
			{
				delete m_sketchCircles[cCnt];
				m_sketchCircles.erase(m_sketchCircles.begin() + cCnt);
				status = true;
				break;
			}
		}
		return status;
	}

	//------------------------------------------------------------------------------------------------------------------

	const std::vector<SketchCircle2D*>& Sketch2D::GetSketchCircle2Ds()const
	{
		return m_sketchCircles;
	}
}
//------------------------------------------------------------------------------------------------------------------