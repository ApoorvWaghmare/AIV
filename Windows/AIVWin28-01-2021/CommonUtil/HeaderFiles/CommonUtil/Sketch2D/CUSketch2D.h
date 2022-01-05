
#pragma once

//------------------------------------------------------------------------------------------------------------------

#include <vector>
#include "CUPoint2D.h"
#include "CUVector.h"
#include "CUPlane.h"

//------------------------------------------------------------------------------------------------------------------

namespace CommonUtil
{
	//Forward declarations
	class SketchSegment2D;
	class SketchVertex2D;
	class SketchCircle2D;

	class Sketch2D
	{
		std::vector<SketchVertex2D*>m_sketchVertices;
		std::vector<SketchSegment2D*>m_sketchSegments;
		std::vector<SketchCircle2D*>m_sketchCircles;
		CommonUtil::Plane m_sketchPlane;
		CommonUtil::Vector m_sketchPlaneXAxis, m_sketchPlaneYAxis;

		CommonUtil::SketchVertex2D* createSketchVertex2D(const CommonUtil::Point2D& vertex, double tol = CommonUtil::CommonConstants::SQR_PNT_TOL);
		bool removeSketchVertex2D(SketchVertex2D* vertex);
		void uncheckAllSketchSegment2DFlags();
	public:
		Sketch2D(CommonUtil::Plane& sketchPlane, CommonUtil::Vector& sketchPlaneXAxis);
		~Sketch2D();

		SketchSegment2D* CreateSketchSegment2D(const CommonUtil::Point2D& vertex1, const CommonUtil::Point2D& vertex2, double tol = CommonUtil::CommonConstants::SQR_PNT_TOL);
		bool DeleteSketchSegment2D(SketchSegment2D* sketchSegment2D);
		bool GetSketchVertex2Ds(double x, double y, std::vector<SketchVertex2D*>vertices)const;
		const std::vector<SketchSegment2D*>& GetSketchSegment2Ds()const;
		const CommonUtil::Plane& GetPlane()const;
		void GetAxes(CommonUtil::Vector& sketchPlaneXAxis, CommonUtil::Vector& sketchPlaneYAxis)const;
		bool UpdateSketchVertex2D(const SketchVertex2D* sketchVertex2D, double x, double y, bool replaceIfExist);
		void GetBoundingBox2D(double& xMin, double& yMin, double& xMax, double& yMax)const;
		bool AnalysePolylines(std::vector<std::vector<const CommonUtil::SketchVertex2D*>>&polygons, bool closed = true);
		SketchCircle2D* CreateSketchCircle2D(const CommonUtil::Point2D& center, float radius);
		bool DeleteSketchCircle2D(SketchCircle2D* sketchCircle2D);
		const std::vector<SketchCircle2D*>& GetSketchCircle2Ds()const;
	};
}
//------------------------------------------------------------------------------------------------------------------