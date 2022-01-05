
#pragma once

//------------------------------------------------------------------------------------------------------------------

#include "CUPoint2D.h"

//------------------------------------------------------------------------------------------------------------------

namespace CommonUtil
{
	//Forward declarations
	class Sketch2D;

	class SketchCircle2D
	{
		friend class Sketch2D;

		CommonUtil::Point2D m_center;
		float m_radius = 0.0;

		SketchCircle2D(const CommonUtil::Point2D& center, float radius);
		~SketchCircle2D();
	public:

		const CommonUtil::Point2D& GetCenter()const;
		float GetRadius()const;
		void SetCenter(const CommonUtil::Point2D& center);
		void SetRadius(float radius);
	};
}
//------------------------------------------------------------------------------------------------------------------