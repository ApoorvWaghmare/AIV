#ifndef _COMMONUTIL_RENDER_INTERFACE_H_
#define _COMMONUTIL_RENDER_INTERFACE_H_

#include "CUEssenceHandle.h"

namespace CommonUtil
{
class EntityInterface;
class RenderInterface 
{

public:
	virtual ~RenderInterface() {}

	//reference counting interface.
	//the rendering objects will be reference counted for managing
	//multiple references, through the EssenceHandle mechanism.
	virtual void AddRef() = 0;
	virtual int Release() = 0;
	virtual int GetRefCount() = 0;

	//Unique key interface
	//user of rendering object has to supply it with a unique key
	//this key is used during selection of the rendered objects the selection
	//mechanism will return the unique key for the selected objects back to user
	virtual void SetEntity(CommonUtil::EntityInterface* key) = 0;
	virtual CommonUtil::EntityInterface* GetEntity() const = 0;

	//Common Rendering operations
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual bool IsVisible()const = 0;
	virtual void SetVisible(bool bvis = true) = 0;
	virtual void SetSelected(bool selected) = 0;
	virtual bool IsSelected()const = 0;
	virtual void SetRenderMode(int shadingMode) = 0;
	virtual void SetColor(unsigned char r,unsigned char g,unsigned char b) = 0;
	virtual void GetColor(unsigned char &r,unsigned char &g,unsigned char &b) = 0;
	virtual bool IsAnnotationVisible()const = 0;
	virtual void SetAnnotationVisible(const bool visible) = 0;
	virtual void SetSizeXAndYToAnnotation(const double sizeX, const double sizeY) = 0;
	virtual std::vector<double> GetAnnotationBasePoint()const = 0;
	virtual void SetAnnotationBasePoint(const std::vector<double> basePoint) = 0;
	virtual void GetAnnotationScreenPoint(double &screenX, double &screenY) = 0;
	virtual void ClearList()const = 0;
	virtual void SetHighLighted(bool isHighLighted) = 0;
	virtual bool IsHighLighted()const = 0;
};
typedef CommonUtilHandle<CommonUtil::RenderInterface> CURenderObject;
}
#endif