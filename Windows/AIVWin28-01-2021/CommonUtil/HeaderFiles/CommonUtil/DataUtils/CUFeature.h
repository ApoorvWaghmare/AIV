//=============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
//=============================================================================

#ifndef _FEATURE_H_
#define _FEATURE_H_


//-----------------------------------------------------------------------------
#include <vector>
#include "CUEntityInterface.h"
#include "CURenderInterface.h"
#include "CUSPoint3D.h"

//-----------------------------------------------------------------------------

namespace CommonUtil
{
	class Point3DF;
}

#pragma pack(push)
#pragma pack(1)

class Feature : public CommonUtil::EntityInterface
{	
	std::string m_name;
	int m_treeIndex;

protected:

	bool m_annotationVisible;
	static size_t m_entityCounter;
	CommonUtil::CURenderObject m_renderObj;

public:

	//Default Constructor
	Feature();

	//Default Destructure
	virtual ~Feature();
	
	//Get Set Method
	bool IsAnnotationVisible()const;
	void SetAnnotationVisibility(const bool visible);
	std::vector<double> GetAnnotationBasePoint() const;
	void SetAnnotationBasePoint(const std::vector<double>& annotationBasePoint);
	void GetAnnotationSize(double& annotationSizeX, double& annotationSizeY) const;
	void SetAnnotationSize(const double annotationSizeX, const double annotationSizeY);

	virtual void SetEntityID(size_t entityID = 0, bool idFromArgument = false) = 0;
	virtual size_t GetEntityID()const = 0;

	void SetRenderObject(CommonUtil::RenderInterface* ro);
	virtual void Show() {};
	virtual void Hide() {};

	virtual CommonUtil::RenderInterface* GetRenderObject()const{return m_renderObj.getData();};

	void SetName(std::string name)
	{
		m_name = name;
	}

	virtual std::string GetName()const
	{
		return m_name;
	}

	void SetTreeIndex(int index)
	{
		m_treeIndex = index;
	}

	virtual int GetTreeIndex()const
	{
		return m_treeIndex;
	}

	void SetVisibility(bool visible);
	
	virtual void SetColor(unsigned char r,unsigned char g,unsigned char b)
	{
		if (m_renderObj.getData())
			m_renderObj->SetColor(r, g, b);
	}
	virtual void GetColor(unsigned char &r,unsigned char &g,unsigned char &b)
	{
		if (m_renderObj.getData())
			m_renderObj->GetColor(r, g, b);
	}

	
	virtual bool IsVisible()const = 0;
	virtual void SetSelected(bool selected) = 0;
	virtual bool IsSelected()const= 0;
	virtual void* GetTreeItem()const  = 0;
	
	virtual void SetTreeItem(void* treeItem) = 0;
	
	void SetRenderMode(int shadingMode)
	{
		if (m_renderObj.getData())
			m_renderObj->SetRenderMode(shadingMode);
	}
};

#pragma pack(pop)   /* restore original from stack */

#endif

//-----------------------------------------------------------------------------
