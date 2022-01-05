// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================
#ifndef _DEBUG_DISPLAY_INTERFACE_H
#define _DEBUG_DISPLAY_INTERFACE_H
#include <vector>

namespace CommonUtil
{

class DebugDisplayInterface
{
public:
	virtual void CreateLineRenderObject(const std::vector<float>& points, const unsigned char color[3]) = 0;

	virtual void CreatePointRenderObject(const float &x, const float &y, const float &z, const unsigned char color[3]) = 0;
};

}
#endif