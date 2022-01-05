
#pragma once

//------------------------------------------------------------------------------------------------------------------

#include "CUPoint2D.h"

//------------------------------------------------------------------------------------------------------------------

namespace CommonUtil
{
	//Forward declarations
	class Sketch2D;
	class SketchSegment2D;

	class SketchVertex2D : public Point2D
	{
		friend class Sketch2D;

		SketchSegment2D *m_sketchSegment2D[2];

		SketchVertex2D(const CommonUtil::Point2D& point);
		~SketchVertex2D();
		bool setSketchSegment(SketchSegment2D* sketchSegment2D);
		bool removeSketchSegment(SketchSegment2D* sketchSegment2D);
		bool isFree()const;
		bool isInvalid()const;
		SketchSegment2D* getSketchSegment2D_1()const;
		SketchSegment2D* getSketchSegment2D_2()const;
		SketchSegment2D* getOtherSketchSegment2D(const SketchSegment2D* sketchSegment)const;
	public:

		const SketchSegment2D* GetSketchSegment2D_1()const;
		const SketchSegment2D* GetSketchSegment2D_2()const;
		const SketchSegment2D* GetOtherSketchSegment2D(const SketchSegment2D* sketchSegment)const;
	};
}
//------------------------------------------------------------------------------------------------------------------