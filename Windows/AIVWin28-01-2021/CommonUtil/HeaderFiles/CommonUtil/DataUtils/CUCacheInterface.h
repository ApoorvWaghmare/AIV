//=============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
//=============================================================================

#ifndef _CACHE_INTERFACE_H_
#define _CACHE_INTERFACE_H_
#include <vector>

namespace CommonUtil
{
class OctreeCell;
class BInputOutputFile;
class CPPoint3D;
}

class CacheInterface
{
public:
	virtual bool DistributeCellDataAmongChildren(const CommonUtil::OctreeCell* octreeCell, size_t cellId, 
										int depth, std::vector<size_t>& childIds, size_t numCellPoints = 0) = 0;

	//For mesh
	virtual void GetDisplayMeshData(size_t cellId, std::vector<float>& vertices, std::vector<float>& normals) = 0;

	virtual void AddLeafCell(int depth, size_t cellId) = 0;

	virtual bool IsHavingData(size_t cellId)const = 0;

	virtual bool LoadCellForEntityId(size_t entityId) = 0;

	virtual void WriteDataIntoFile(size_t index) = 0;

	virtual void SetFile(CommonUtil::BInputOutputFile *file) = 0;

	virtual void GetDisplayPointData(size_t cellId, std::vector<float>& points, std::vector<size_t >& pointIndices,
											std::vector<unsigned char>& colorInfo, std::vector<unsigned char>& flags, 
											bool partialData = false) = 0;

	virtual void GetDisplayPointData(size_t cellId, std::vector<const CommonUtil::CPPoint3D* >& points) = 0;

	//indices are position in vector of cpPoint3d
	virtual const std::vector<size_t>& GetCellPointsIndices(size_t cellId)const = 0;

	virtual const CommonUtil::CPPoint3D& GetCpPoint3DFromPtIndex(size_t ptIndexInArray)const = 0;

	virtual const CommonUtil::CPPoint3D& GetCpPoint3DFromCellBasedPtIndex(size_t ptBitIndex) = 0;

	virtual bool GetPointDataFromIndex(size_t pointIndex, float point[3], unsigned char color[3], unsigned char &flag){return false;}

	virtual void GetPointsInCell(size_t cellId, std::vector<float>& points)const = 0;

	virtual bool SetSelectionToCellData(size_t index, bool selected, bool invertSelection, bool pointId, std::vector<size_t>& selPointIndices, bool applyOnlyHiddenPt = false) = 0;

	virtual void SetVisibilityToCellData(size_t index, bool visible, bool pointId) = 0;

	virtual bool IsHavingColor()const = 0;

	virtual bool WriteCache(size_t &fileSize, size_t &cacheMangrfileSize)const = 0;

	//set num points 
	virtual void UpdateProperty(size_t numPoints, double ext[6]) = 0;

	virtual void UpdateNumPoints(size_t numPoints){}

	virtual size_t GetCPPoint3DIndexInArray(size_t pointIndex) = 0;

	virtual size_t GetCPPointIndex(size_t indexInArray) = 0;
	
	virtual void GetDisplayMeshData(std::vector<float>& vertices, std::vector<unsigned int> &facetVerticesIndex, std::vector<float> &normals) = 0;

	virtual void GetDisplayMeshData(std::vector<float>& vertices, std::vector<unsigned int> &facetVerticesIndex, std::vector<float> &normals, std::vector<unsigned char> &colors) = 0;

	virtual void GetDisplayMeshData(std::vector<float>& vertices, std::vector<unsigned int> &unselectedfacetVerticesIndex, std::vector<unsigned int> &selectedfacetVerticesIndex,
		std::vector<float> &unselectedfacetNormals, std::vector<float> &selectedfacetNormals, std::vector<unsigned char> &colors){};
};

#endif //_CACHE_INTERFACE_H_
