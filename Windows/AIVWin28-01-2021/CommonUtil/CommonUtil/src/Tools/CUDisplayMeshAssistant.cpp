// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include "CUDisplayMeshAssistant.h"

#include "CUOctree.h"
#include "CUOctreeCell.h"


namespace CommonUtil
{

//-----------------------------------------------------------------------------
/*
DisplayMeshAssistant::DisplayMeshAssistant(Octree* octree, DisplayMeshCache* meshCache)
{
	m_octree = octree;
	m_meshCache = meshCache;
}*/

DisplayMeshAssistant::DisplayMeshAssistant(Octree* octree, CacheInterface* meshCache)
{
	m_octree = octree;
	m_meshCacheInterface = meshCache;
}
//-----------------------------------------------------------------------------

DisplayMeshAssistant::~DisplayMeshAssistant()
{

}

//-----------------------------------------------------------------------------
//
//void DisplayMeshAssistant::CreateCellData(std::vector<OctreeCell*>leafCells)
//{
//	for(size_t i = 0; i < leafCells.size(); ++i)
//	{
//		m_meshCache->AddCell(leafCells[i]->GetUniqueId());
//	}
//}
//-----------------------------------------------------------------------------
//
//void DisplayMeshAssistant::GetFacetsInCell(OctreeCell* leafCell,std::vector<DisplayFacetStubH>&facets)
//{
//	facets.clear();
//	m_meshCache->GetCellFacets(leafCell->GetUniqueId(), facets);	
//}
//-----------------------------------------------------------------------------
//
//DisplayVertexStubH DisplayMeshAssistant::GetDisplayVertex(size_t vertexID)
//{
//	return m_meshCache->GetDisplayVertex(vertexID);
//}

//-----------------------------------------------------------------------------
//
//DisplayVertexStubH DisplayMeshAssistant::CreateDisplayVertex(float x, float y, float z, size_t completeId)
//{
//	DisplayVertex* dVertex = new DisplayVertex(x,y,z,completeId);
//	return m_meshCache->AddVertex(completeId,dVertex);
//}

//-----------------------------------------------------------------------------
//
//void DisplayMeshAssistant::CreateDisplayFacetAndAddToCellData(DisplayVertexStubH vertices[3], float normal[3])
//{
//	DisplayFacet* dFacet = new DisplayFacet(vertices[0], vertices[1], vertices[2]);
//	dFacet->SetNormal(normal);
//	m_meshCache->AddFacet(vertices[0]->GetId(),dFacet);
//}

//-----------------------------------------------------------------------------

void DisplayMeshAssistant::GetDisplayData(size_t cellId, std::vector<float> &vertices, std::vector<float> &normals)
{
	m_meshCacheInterface->GetDisplayMeshData(cellId, vertices, normals);
}

//-----------------------------------------------------------------------------

bool DisplayMeshAssistant::IsHavingData(size_t cellId)const
{
	return m_meshCacheInterface->IsHavingData(cellId);
}

//-----------------------------------------------------------------------------

}