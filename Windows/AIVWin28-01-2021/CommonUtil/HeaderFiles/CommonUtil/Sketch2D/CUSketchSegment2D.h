
#pragma once

//------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------

namespace CommonUtil
{
	//Forward declarations
	class SketchVertex2D;
	class Sketch2D;

	class SketchSegment2D
	{
		friend class Sketch2D;

		SketchVertex2D *m_sketchVertices2D[2];
		unsigned char m_flags;//8th bit for checked segment

		SketchSegment2D(SketchVertex2D* vertex1, SketchVertex2D* vertex2);
		~SketchSegment2D();
		SketchVertex2D* getSketchVertex2D_1()const;
		SketchVertex2D* getSketchVertex2D_2()const;
		bool replaceVertex(const SketchVertex2D* toBeReplaced, SketchVertex2D* toReplaceWith);
		void setChecked(bool flag);
		bool isChecked()const;
	public:

		const SketchVertex2D* GetSketchVertex2D_1()const;
		const SketchVertex2D* GetSketchVertex2D_2()const;
		const SketchVertex2D* GetOtherSketchVertex2D(const SketchVertex2D* vertex)const;
		void GetMidPoint(double& x, double& y)const;
		float GetLength()const;
	};
}
//------------------------------------------------------------------------------------------------------------------