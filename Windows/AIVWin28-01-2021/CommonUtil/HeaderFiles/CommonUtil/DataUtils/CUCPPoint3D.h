// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _CPPOINT3D_H_
#define _CPPOINT3D_H_

// Std Include Files
#include <float.h>
#include <vector>

// Include Files
#include "CUPoint3DF.h"

#pragma warning (disable: 4786)

namespace CommonUtil
{
class Point3D;
class PointCloud;
class Plane;
class Vector;

#pragma pack(push)  /* push current to stack */
#pragma pack(1)     /* set to 1 byte boundary */
// This class represents a point from cloud of points
class CPPoint3D: public Point3DF
{
	//std::vector<size_t> m_neighbors;

	//Plane m_rmsPlane;
	size_t m_pointIndex;
	//size_t m_cloudIndex;
	//float m_cost;
	//float m_avgDist;

	//color info
	unsigned char m_r, m_g, m_b;
	unsigned char m_Flags;
	//meanings of the flags:
	//bit 1: 1 -> is CPPoint3D valid
	//bit 2: 1 -> is CPPoint3D visited
	//bit 3: 1 -> is CPPoint3D on border point
	//bit 4: 1 -> is CPPoint3D normal valid 
	//bit 5: 1 -> is CPPoint3D neighbour computed
	//bit 6: 1 -> is CPPoint3D selected
	//bit 7: 1 -> is CPPoint3D visible
	//unsigned short m_intensity;

public:

	// Constructor
	CPPoint3D();

	// Constructor
	CPPoint3D(float x, float y, float z, unsigned char m_r = 0, unsigned char m_g = 0 , unsigned char m_b = 0, unsigned short intensity = 0);

	// Destructor
	~CPPoint3D();

	const std::vector<size_t>* GetNeighbours();

	void SetNeighbours(std::vector<size_t>& neighbours);

	void CleanUpData();

	//to set normal set rms plane
	//void SetRMSPlane(Plane plane);

	//Plane GetRMSPlane()const;

	unsigned short GetIntensity()
	{
		return 0;//m_intensity;
	}

	void SetIntensity(unsigned short intensity)
	{
		//m_intensity  =intensity;
	}
	
	void SetInvalid(bool flags = true);

	bool IsInvalid()const{ return ((m_Flags & 1) != 0); } //if anding of m_flag & (0000 0001) is not equal to zero then Valid flag is true

	void SetVisited (bool visited)
	{
		if(visited)
			m_Flags |= (1<<1);	//set 2nd bit to one by oring m_flag with (1<<1)(0000 0010);
		else
			m_Flags &= 0xFD;	//set 2nd bit to zero by anding m_flag with (1111 1101);
	}
	
	bool IsVisited()const{return ((m_Flags & (1<<1)) != 0);} //if anding of m_Flags & (1<<1)(0000 0010) is not equal to zero then Leaf flag is true

	void SetBorderPoint(bool borderPoint)
	{
		if(borderPoint)
			m_Flags |= (1<<2);	//set 3nd bit to one by oring m_flag with (1<<2)(0000 0100);
		else
			m_Flags &= 0xFB;	//set 3th bit to zero by anding m_flag with (1111 1011);
	}
	
	bool IsOnBorder(){return ((m_Flags & (1<<2)) != 0);}	 //if anding of m_Flags & (1<<1)(0000 0100) is not equal to zero then Leaf flag is true

	//check if CpPoint3D is having valid normal or not
	void SetValidNormal(bool vnflags)
	{
		if(vnflags)
			m_Flags |= (1<<3);	//set 4th bit to one by oring m_flag with (1<<3)(0000 1000);
		else
			m_Flags &= 0xF7;	//set 4th bit to zero by anding m_flag with (1111 0111);
	}
	
	bool IsValidNormal()const{ return ((m_Flags & (1<<3)) != 0); } //if anding of m_Flags & (1<<3)(0000 1000) is not equal to zero then Leaf flag is true

	//check if CpPoint3D neighbour is computed or not
	void SetNeighbourComputed(bool ncflags)
	{
		if(ncflags)
			m_Flags |= (1<<4);	//set 4th bit to one by oring m_flag with (1<<4)(0001 0000);
		else
			m_Flags &= 0xEF;	//set 4th bit to zero by anding m_flag with (1110 1111);
	}
	
	bool IsNeighboursComputed()const{ return ((m_Flags & (1<<4)) != 0); } //if anding of m_Flags & (1<<3)(0000 1000) is not equal to zero then Leaf flag is true

	//check if CpPoint3D is selected
	void SetSelected(bool sflags, bool applyToHidePt = false)
	{
		if(applyToHidePt)
		{
			if(sflags)
				m_Flags |= (1<<5);	//set 4th bit to one by oring m_flag with (1<<5)(0010 0000);
			else
				m_Flags &= 0xDF;	//set 4th bit to zero by anding m_flag with (1101 1111);
		}

		if(IsVisible())
		{
			if(sflags)
				m_Flags |= (1<<5);	//set 4th bit to one by oring m_flag with (1<<5)(0010 0000);
			else
				m_Flags &= 0xDF;	//set 4th bit to zero by anding m_flag with (1101 1111);
		}
	}
	
	bool IsSelected()const{ return ((m_Flags & (1<<5)) != 0); } //if anding of m_Flags & (1<<3)(0000 1000) is not equal to zero then Leaf flag is true

	//check if CpPoint3D is visible 
	void SetVisible(bool vflags)
	{
		if(vflags)
			m_Flags |= (1<<6);	//set 5th bit to one by oring m_flag with (1<<6)(0100 0000);
		else
			m_Flags &= 0xBF;	//set 5th bit to zero by anding m_flag with (1011 1111);
	}
	
	bool IsVisible()const{ return ((m_Flags & (1<<6)) != 0); } //if anding of m_Flags & (1<<3)(0000 1000) is not equal to zero then Leaf flag is true
	
	//Workflow flag
	void SetWorkflowFlag(bool wflag)
	{
		if(wflag)
			m_Flags |= (1 << 7);
		else
			m_Flags &= ~(1 << 7);
	}

	bool CheckForWorkFlowFlag()const { return ((m_Flags & (1 << 7)) != 0); }

	void SetPointIndex(size_t index)
	{
		m_pointIndex = index;
	}

	size_t GetPointIndex()const
	{
		return m_pointIndex;
	}

/*	void SetCost(float cost)
	{
		m_cost = cost;
	}

	float GetCost()const
	{
		return m_cost;
	}
*/
	/*void SetAvgDist(float avgDist)
	{
		//m_avgDist = avgDist;
	}

	float GetAvgDist()const
	{
		return 0;//m_avgDist;
	}*/

	/*void SetCloudIndex(size_t cloudIndex)
	{
		m_cloudIndex = cloudIndex;
	}

	size_t GetCloudIndex() const
	{
		return m_cloudIndex;
	}*/

	int GetCoordinates(float& x, float& y, float& z)const;
	int SetCoordinates(float x, float y, float z);
	//set colour info 
	void SetColorInfo(unsigned char r, unsigned char g, unsigned char b);
	int SetPointInfo(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b);
	
	//set flags
	void SetFlags(unsigned char flags);
	//void SetX( float x)
	//{
	//	m_x = x;
	//};
	//void SetY(float y)
	//{
	//	m_y = y;
	//};
	//void SetZ(float z)
	//{
	//	m_z = z;
	//};

	//get colour info
	//int GetCoordinates(float& x, float& y, float& z)const;
	int GetColorInfo(unsigned char& r, unsigned char& g, unsigned char& b)const;
	int GetPointInfo(float& x, float& y, float& z, unsigned char& r, unsigned char& g, unsigned char& b)const;
	//float GetX()const
	//{
	//	return m_x;
	//};
	//float GetY()const
	//{
	//	return m_y;
	//};
	//float GetZ()const
	//{
	//	return m_z;
	//};

	//computes square of the distance from another point
	//float SqrDistance(const CPPoint3D &point)const
	//{
	//	float sqrDist = (m_x-point.m_x) * (m_x-point.m_x) +
	//					 (m_y-point.m_y) * (m_y-point.m_y) +
	//					 (m_z-point.m_z) * (m_z-point.m_z);
	//
	//	return sqrDist;
	//}

	//float Distance(const CPPoint3D &point)const
	//{
	//	float sqrDistance = (m_x-point.m_x) * (m_x-point.m_x) +
	//						 (m_y-point.m_y) * (m_y-point.m_y) +
	//						 (m_z-point.m_z) * (m_z-point.m_z);
	//
	//	return sqrt(sqrDistance);
	//}
	bool GetNormal(double& i, double& j, double& k)const
	{
		// to be coded whenevr we keep normal  
		return false;
	}
	//get flags
	int GetFlags(unsigned char& flags)const;

	CPPoint3D&  operator = (const CPPoint3D& CPPoint3Dobj);
	
	operator Point3D() const;

	//set pointinfo
	void Set(float xCord, float yCord, float zCord, unsigned char rColor, unsigned char gColor, unsigned char bColor);
};
#pragma pack(pop)   /* restore original from stack */
}


#endif
