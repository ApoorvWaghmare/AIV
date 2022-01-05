// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _DISPLAY_MESH_ASSISTANT_H_
#define _DISPLAY_MESH_ASSISTANT_H_

#include <vector>
#include "CUCacheInterface.h"
namespace CommonUtil
{
class Octree;
class OctreeCell;
class DisplayMeshCache;

class DisplayVertex;
class DisplayFacet;

class DisplayMeshAssistant
{
//	size_t m_cellCounter;

protected:

	Octree* m_octree;

	//DisplayMeshCache* m_meshCache;

	CacheInterface* m_meshCacheInterface;
public:			

	//DisplayMeshAssistant(Octree* octree, DisplayMeshCache* meshCache);

	DisplayMeshAssistant(Octree* octree, CacheInterface* meshCache);

	virtual ~DisplayMeshAssistant();	
		
	Octree* GetOctree(){return m_octree;}

	//void CreateCellData(std::vector<OctreeCell*>leafCells);

	//void GetFacetsInCell(OctreeCell* leafCell, std::vector<DisplayFacetStubH>&facets);
	
	//Functions for Creating Display Mesh Cache
	//DisplayVertexStubH GetDisplayVertex(size_t vertexID);

	//Create DisplayVertex and add it to Corresponding cell
	//DisplayVertexStubH CreateDisplayVertex(float x, float y, float z, size_t entityId);

	//Create DisplayFacet and add it to Corresponding Cell
	//void CreateDisplayFacetAndAddToCellData(DisplayVertexStubH vertices[3], float normal[3]);

	void GetDisplayData(size_t cellId, std::vector<float> &vertices, std::vector<float> &normals);

	bool IsHavingData(size_t cellId)const;
};

}

#endif