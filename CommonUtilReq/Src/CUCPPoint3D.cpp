// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

// Std Include Files
#include <algorithm>

// Include Files
//#include "pointcloud.h"
#include "CUPlane.h"
#include "CUCPPoint3D.h"
#include "CUCommonConstants.h"
#include "CUPoint3D.h"

namespace CommonUtil
{
//MemManager* CPPoint3D::m_MemMgr = 0;
//-----------------------------------------------------------------------------

CPPoint3D::CPPoint3D():Point3DF(0,0,0)
{
	//m_avgDist = 0.;
	//m_cost = 0.;
	//Set visible flag true
	m_Flags = 0;
	m_Flags |= (1<<6);
	m_Flags &= 0xFB;
	m_r = 0;
	m_g = 0;
	m_b = 0;
	//m_intensity = 0;
}

//-----------------------------------------------------------------------------

CPPoint3D::CPPoint3D(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned short intensity):Point3DF(x,y,z)
{
	//m_avgDist = 0.;
	//m_cost = 0.;
	m_Flags = 0;
	m_Flags |= (1<<6);
	m_Flags &= 0xFB;
	m_r = r;
	m_g = g;
	m_b = b;

	//m_intensity = intensity;
}

//-----------------------------------------------------------------------------

CPPoint3D::~CPPoint3D()
{
	CleanUpData();
}

//-----------------------------------------------------------------------------

void CPPoint3D::CleanUpData()
{
	/*if (m_rmsPlane)
	{
		delete m_rmsPlane;
		m_rmsPlane = 0;
	}*/
}

//-----------------------------------------------------------------------------

void CPPoint3D::SetInvalid(bool flags)
{
	if(flags)
		m_Flags |= 1;		//set 1st bit to one by oring m_flag with (0000 0001);
	else
		m_Flags &= 0xFE;	//set 1st bit to zero by anding m_flag with (1111 1110);
}

//-----------------------------------------------------------------------------

//int CPPoint3D::GetCoordinates(float& x, float& y, float& z)const
//{
//	x = m_x;
//	y = m_y;
//	z = m_z;
//
//	return 0;
//}


//-----------------------------------------------------------------------------

int CPPoint3D::GetCoordinates(float& x, float& y, float& z)const
{
	x = GetX();
	y = GetY();
	z = GetZ();

	return 0;
}


//-----------------------------------------------------------------------------

int CPPoint3D::SetCoordinates(float x, float y, float z)
{
	SetX(x);
	SetY(y);
	SetZ(z);

	return 0;
}
//-----------------------------------------------------------------------------

void CPPoint3D::SetColorInfo(unsigned char r, unsigned char g, unsigned char b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}

//-----------------------------------------------------------------------------

int CPPoint3D::SetPointInfo(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b)
{
	m_r = r;
	m_g = g;
	m_b = b;
	return 0;
}
//-----------------------------------------------------------------------------
int CPPoint3D::GetColorInfo(unsigned char& r, unsigned char& g, unsigned char& b)const
{
	r = m_r;
	g = m_g;
	b = m_b;

	return 0;
}

//-----------------------------------------------------------------------------
int CPPoint3D::GetPointInfo(float &x, float &y, float &z, unsigned char &r, unsigned char &g, unsigned char &b)const
{
	r = m_r;
	r = m_g;
	r = m_b;
	return 0;
}
//-----------------------------------------------------------------------------
void CPPoint3D::SetFlags(unsigned char flags)
{
	m_Flags = flags;
}

//-----------------------------------------------------------------------------
int CPPoint3D::GetFlags(unsigned char& flags)const
{
	flags = m_Flags;
	return 0;
}
//-----------------------------------------------------------------------------

/*void CPPoint3D::SetRMSPlane(Plane plane)
{
	//m_rmsPlane = plane;
}
//-----------------------------------------------------------------------------

Plane CPPoint3D::GetRMSPlane()const
{
	return Plane();//m_rmsPlane;
}*/

//-----------------------------------------------------------------------------

CPPoint3D&  CPPoint3D::operator = (const CPPoint3D& CPPoint3Dobj)
{
	//m_neighbors = CPPoint3Dobj.m_neighbors;
	//m_rmsPlane = new Plane;
	//m_rmsPlane = CPPoint3Dobj.m_rmsPlane;
	//m_rmsPlane->SetPoint(CPPoint3Dobj.m_rmsPlane->GetPoint());
	m_pointIndex = CPPoint3Dobj.m_pointIndex;
	//m_cost = CPPoint3Dobj.m_cost;
	SetX(CPPoint3Dobj.GetX());
	SetY(CPPoint3Dobj.GetY());
	SetZ(CPPoint3Dobj.GetZ());
	m_r = CPPoint3Dobj.m_r;
	m_g = CPPoint3Dobj.m_g;
	m_b = CPPoint3Dobj.m_b;
	m_Flags = CPPoint3Dobj.m_Flags;

	return *this;
}

//-----------------------------------------------------------------------------

const std::vector<size_t>* CPPoint3D::GetNeighbours()
{
	std::vector<size_t> temp;
	return nullptr;//m_neighbors;
}

//-----------------------------------------------------------------------------

void CPPoint3D::SetNeighbours(std::vector<size_t>& neighbours)
{
	/*if(neighbours.size() >= CommonConstantsMAX_NEIGHBOURS)
		neighbours.resize(CommonConstantsMAX_NEIGHBOURS);*/

	//m_neighbors = neighbours;
}

//-----------------------------------------------------------------------------

CPPoint3D::operator Point3D() const
{
	return Point3D(GetX(),GetY(),GetZ());
}

//-----------------------------------------------------------------------------

void CPPoint3D::Set(float xCord, float yCord, float zCord, unsigned char rColor,
					unsigned char gColor, unsigned char bColor)
{
	SetX(xCord);
	SetY(yCord);
	SetZ(zCord);
	m_r = rColor;
	m_g = gColor;
	m_b = bColor;
}

//-----------------------------------------------------------------------------

}
