//=============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
//=============================================================================

#ifndef _COMMONUTIL_ENTITY_INTERFACE_H_
#define _COMMONUTIL_ENTITY_INTERFACE_H_

/*
The class EntityInterface is a base class to Entities of ECadViewer. 
The entities have 3 members -	1) it's type class pointer like PointCloud, Mesh etc.
							-	2) a render object which is used for rendering the data
							-	3) a tree item in treeview of the viewer

*/
#include <string>
#include <vector>
#include "CURenderInterface.h"


namespace CommonUtil
{
	class CPPoint3D;
	class Point3DF;
}

namespace CommonUtil
{
#pragma pack(push)
#pragma pack(1)
class EntityInterface
{
//protected:
//
//	std::string m_name;
//	int m_treeIndex;
//	bool m_visible;
//	unsigned char m_r,m_g,m_b;
public:

	virtual void SetName(std::string name) = 0;

	virtual std::string GetName()const = 0;


	virtual void SetTreeIndex(int index) = 0;

	virtual int GetTreeIndex()const = 0;

	EntityInterface(){}
	
	virtual ~EntityInterface() {}

	virtual void SetRenderObject(CommonUtil::RenderInterface* ro) = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual void SetColor(unsigned char r,unsigned char g,unsigned char b) = 0;

	virtual void GetColor(unsigned char &r,unsigned char &g,unsigned char &b) = 0;
	virtual void SetVisibility(bool visible) = 0;
	virtual bool IsVisible()const = 0;
	virtual void SetSelected(bool selected) = 0;
	virtual bool IsSelected()const= 0;
	virtual CommonUtil::RenderInterface* GetRenderObject()const = 0;
	virtual void GetOtherName(std::string &name)const{}

	virtual void SetTreeItem(void* treeItem) { SetTreeItem(treeItem); } /* = 0;*/
	virtual void* GetTreeItem()const = 0;


	virtual void SetRenderMode(int shadingMode) = 0;

	virtual bool GetCenterPoint(double *centerPoint)const{return false;}

	/*virtual bool GetPointsWithInDistance(const RevLib::PointCloudInterface* pointCloud, const double &maxDistance, double &longestDist,
					double &shortestDist, std::vector<size_t> &pointIndices, std::vector<double> &distArray)const { return false;}
*/
	//returns true if projected point is on entity
	//also gives shortest distance of point from entity
	//distance will be -ve if point inside the entity(like circle, cone, cylinder)
	//also gives projected point on entity if required
	virtual bool GetShortestDistanceFromEntity(const CommonUtil::CPPoint3D* referenceCPPoint, double &minDistance, CommonUtil::Point3DF* pointOnEntity = nullptr)const
					{return false;}
};
#pragma pack(pop)   /* restore original from stack */
}

#endif //_ESSENCE_ENTITY_INTERFACE_H_


