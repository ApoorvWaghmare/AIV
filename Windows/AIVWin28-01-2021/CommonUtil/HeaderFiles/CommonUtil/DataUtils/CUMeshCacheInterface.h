// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _MESH_CACHE_INTERFACE_H
#define _MESH_CACHE_INTERFACE_H

#include <vector>
#include "CUCPPoint3D.h"

namespace CommonUtil
{

class Octree;
class OctreeCell;


class MeshCacheInterface
{
	Octree *m_octree;

public:
	MeshCacheInterface();

	~MeshCacheInterface();

	virtual void GetVertexIndices(size_t cellID, std::vector<unsigned int>& indices) = 0;

	//index - combination of cellid and it's possition in cell data
	virtual const CommonUtil::CPPoint3D & GetCPPoint3DFromVertexIndex(size_t vertexCellIndex)/*const*/ = 0;

	virtual void GetDisplayMeshData(std::vector<float>& vertices, std::vector<unsigned int> &unselectedfacetVerticesIndex, std::vector<unsigned int> &selectedfacetVerticesIndex,
		std::vector<float> &unselectedfacetNormals, std::vector<float> &selectedfacetNormals, std::vector<unsigned char> &colors) = 0;
};

}

#endif