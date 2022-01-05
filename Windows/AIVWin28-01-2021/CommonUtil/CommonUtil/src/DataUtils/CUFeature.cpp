//#if defined(_MSC_VER)
//#undef max
//#undef min
//#pragma warning(disable:4018 4100 4663 4786 4710)
//#pragma warning(push,2)
//#endif
#include "CUFeature.h"


//--------------------------------------------------------------------------------

size_t Feature::m_entityCounter = 0;

//--------------------------------------------------------------------------------

Feature::Feature()
{
	m_treeIndex = 0;
	m_annotationVisible = true;
}

//--------------------------------------------------------------------------------

Feature::~Feature() {}

//--------------------------------------------------------------------------------

bool Feature::IsAnnotationVisible()const
{
	return m_annotationVisible;
}

//--------------------------------------------------------------------------------

void Feature::SetAnnotationVisibility(const bool visible)
{
	m_annotationVisible = visible;
}

//--------------------------------------------------------------------------------

std::vector<double> Feature::GetAnnotationBasePoint() const
{
	return m_renderObj->GetAnnotationBasePoint();
}

//--------------------------------------------------------------------------------

void Feature::SetAnnotationBasePoint(const std::vector<double>& annotationBasePoint)
{
	m_renderObj->SetAnnotationBasePoint(annotationBasePoint);
}

//--------------------------------------------------------------------------------

void Feature::GetAnnotationSize(double& annotationSizeX, double& annotationSizeY) const
{
	
}

//--------------------------------------------------------------------------------

void Feature::SetAnnotationSize(const double annotationSizeX, const double annotationSizeY)
{
	if (m_renderObj)
		m_renderObj->SetSizeXAndYToAnnotation(annotationSizeX, annotationSizeY);
}

//--------------------------------------------------------------------------------

void Feature::SetRenderObject(CommonUtil::RenderInterface* ro)
{
	int temp = 0;
	m_renderObj = ro;
}

//--------------------------------------------------------------------------------

void Feature::SetVisibility(bool visible)
{
	if (m_renderObj)
		m_renderObj->SetVisible(visible);
}

//--------------------------------------------------------------------------------