// ============================================================================
//			Copyright (c) 2015 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _CACHEMANAGER_H_
#define _CACHEMANAGER_H_

#pragma once

// Std Include Files
#include <vector>
#include <unordered_map>
#include <math.h>
#include "CUCacheInterface.h"
#include "CUBinaryIOBufferBlock.h"
#include "CUBInputOutputFile.h"
#include <assert.h>

#include "CUTimeslice.h"

#define MAXBUFFERSIZE 1024

namespace CommonUtil
{

class FileHandler;
#pragma pack(push)
#pragma pack(1)
template <class C>
class CellInfo
{
	typedef std::unordered_map<size_t, CellInfo<C>*> CellIdCellInfoMap;

	unsigned char m_Flags;
	//meanings of the flags:
	//bit 1: 1 -> is cell info valid (no data member is valid)
	//bit 2: 1 -> is cell leaf cell (either cellData or fileOffset member valid), 0 -> non leaf cell (cellInfoMap member is valid)
	//bit 3: 1 -> is cell data loaded (cellData member is valid), 0 -> cell data not loaded ie. in file (fileOffset member is valid)
	//bit 4: 1 -> is cell having data, 0 -> empty cell
	int m_FileOffset;

	union
	{
		C* m_CellData;
		CellIdCellInfoMap* m_CellInfoMap;
	};

public:

	CellInfo() {
		m_CellInfoMap = 0;
		m_CellData = 0;
		m_Flags = 0;
		m_FileOffset = -1;
	}

	void SetFlags(unsigned char flags) { m_Flags = flags; }
	unsigned char GetFlags() const { return m_Flags; }
	void SetValidCell(bool bValid)
	{
		if(bValid)
			m_Flags |= 1;		//set 1st bit to one by oring m_flag with (0000 0001);
		else
			m_Flags &= 0xFE;	//set 1st bit to zero by anding m_flag with (1111 1110);
	}	

	bool IsValidCell()const{ return ((m_Flags & 1) != 0); } //if anding of m_flag & (0000 0001) is not equal to zero then Valid flag is true

	void SetLeafCell(bool bLeaf)
	{
		if(bLeaf)
			m_Flags |= (1<<1);	//set 2nd bit to one by oring m_flag with (1<<1)(0000 0010);
		else
			m_Flags &= 0xFD;	//set 8th bit to zero by anding m_flag with (1111 1101);
	}

	bool IsLeafCell()const{ return ((m_Flags & (1<<1)) != 0); } //if anding of m_Flags & (1<<1)(0000 0010) is not equal to zero then Leaf flag is true

	//check if cellData is lodded or not
	void SetLoadedCell(bool bLoad)
	{
		if(bLoad)
			m_Flags |= (1<<2);	//set 3nd bit to one by oring m_flag with (1<<2)(0000 0100);
		else
			m_Flags &= 0xFB;	//set 3th bit to zero by anding m_flag with (1111 1011);
	}

	bool IsLoadedCell()const{ return ((m_Flags & (1<<2)) != 0); } //if anding of m_Flags & (1<<1)(0000 0100) is not equal to zero then Leaf flag is true

	//check if cellData is having data or not
	void SetHavingData(bool dataPresent)
	{
		if(dataPresent)
			m_Flags |= (1<<3);	//set 4th bit to one by oring m_flag with (1<<3)(0000 1000);
		else
			m_Flags &= 0xF7;	//set 4th bit to zero by anding m_flag with (1111 0111);
	}

	bool IsHavingData()const{ return ((m_Flags & (1<<3)) != 0); } //if anding of m_Flags & (1<<3)(0000 1000) is not equal to zero then Leaf flag is true

	void SetFileOffset(int fileOffset)
	{
		//assert(IsValidCell());
		//assert(IsLeafCell());

		//assert(!IsLoadedCell());
		m_FileOffset = fileOffset;
	}

	int GetFileOffset() const
	{
		//assert(IsValidCell());
		//assert(IsLeafCell());
		//assert(!IsLoadedCell());
		if(IsLeafCell())
			return m_FileOffset;
		return -1;
	}

	void SetCellData(C* cellData)
	{
		assert(IsValidCell());
		assert(IsLeafCell());
		//assert(IsLoadedCell());
		SetLoadedCell(true);
		m_CellData = cellData;
	}

	C* GetCellData() const
	{
		assert(IsValidCell());
		assert(IsLeafCell());
		if(IsLoadedCell())
			return m_CellData;
		return nullptr;
	}

	void SetCellInfoMap(CellIdCellInfoMap *cellInfoMap)
	{
		assert(IsValidCell());
		assert(!IsLeafCell());
		m_CellInfoMap = cellInfoMap;
	}

	CellIdCellInfoMap* GetCellInfoMap() const
	{
		assert(IsValidCell());
		if(!IsLeafCell())
			return m_CellInfoMap;
		return nullptr;
	}
	
	size_t SizeOf()const
	{
		size_t size = sizeof(*this);
		return size;
	}
};

//------------------------------------------------------------------------------------
#pragma pack(pop)   /* restore original from stack */

#pragma pack(push)
#pragma pack(1)

template <class C>
class CacheManager
{
	typedef std::unordered_map<size_t, CellInfo<C>*> CellIdCellInfoMap;

	FileHandler *m_binaryIOfile;

	CacheInterface *m_cache;

	int m_CellInfoCacheDepth;

	std::vector<CellInfo<C>*> m_CellInfoCache;

	std::vector<C*> m_cellDataInMemory;

	int m_currentIndex;

//------------------------------------------------------------------------------------

	void initalizeManager()
	{
		BInputOutputFile file(L"C:\\Temp\\CacheManagerEntity.txt", true);

		//read numCellInfo in array
		__int64 numCellInfoInArray = 0; 
		file >> numCellInfoInArray;
		
		for(size_t index = 0; index < numCellInfoInArray; ++index)
		{
			unsigned char flags = 0;
			int offset = -1;

			//write offset and flags in file
			file >> flags;
			file >> offset;

			CellInfo<C>* cellInfo = new CellInfo<C>;
			cellInfo->SetFlags(flags);
			cellInfo->SetFileOffset(offset);

			m_CellInfoCache[index] = cellInfo;

			if(cellInfo->IsValidCell())
			{
				if(!cellInfo->IsLeafCell())
				{
					__int64 numCellInfoInMap = 0; 

					//read map entity in file
					file >> numCellInfoInMap;

					for(int ctr = 0; ctr < numCellInfoInMap; ++ctr) 
					{
						//read cellId and cellinfo of map
						__int64 cellId = 0;
						unsigned char mapFlags = 0;
						int mapOffset = -1;

						file >> cellId;

						file >> mapFlags;
						file >> mapOffset;

						CellInfo<C>* cellInfoInMap = new CellInfo<C>;
						cellInfoInMap->SetFlags(mapFlags);
						cellInfoInMap->SetFileOffset(mapOffset);

						if(!cellInfo->GetCellInfoMap())
							cellInfo->SetCellInfoMap(new CellIdCellInfoMap);

						//insert pair into map
						cellInfo->GetCellInfoMap()->insert(CellIdCellInfoMap::value_type(cellId, cellInfoInMap));
					}
				}
			}
		}
	}

//------------------------------------------------------------------------------------

	void writeCell(size_t currentIndex,  bool clearCellData = false)
	{
		C* cell  =  m_cellDataInMemory[currentIndex];
		if(cell)
		{
			size_t cellId = cell->m_cellId;
			int writeOffset = -1;
			CellInfo<C>* cellInfoToUnload = getValidLeafCellInfo(cellId);
			if(cellInfoToUnload->IsValidCell())
			{
				writeOffset = cellInfoToUnload->GetFileOffset();

				if(writeOffset == -1)
				{
					//get new offet and set this offset
					writeOffset = m_binaryIOfile->GetEndPosInFile();
					cellInfoToUnload->SetFileOffset(writeOffset);
				}

				m_binaryIOfile->SetFilePtrPosInFile(writeOffset);
				m_cache->WriteDataIntoFile(currentIndex);
				if(!clearCellData) //???VPL
				cellInfoToUnload->SetCellData(nullptr);
				//cellInfoToUnload.SetLoadedCell(false);
				//setValidLeafCellInfo(cellId, cellInfoToUnload);
			}
		}
	}

//------------------------------------------------------------------------------------

	size_t getCurrentIndex()
	{
		//if(!CommonConstants::INCORE)
		//{
		//	size_t savedCurrentIndex = m_currentIndex;
		//	bool done = false;
		//	while(!done)
		//	{
		//		C* cell = m_cellDataInMemory[m_currentIndex];

		//		if(cell)
		//		{
		//			writeCell(m_currentIndex);

		//			if(cell->IsEmpty())
		//				done = true;
		//			else
		//			{
		//				updateCurrentIndex();
		//				if(savedCurrentIndex == m_currentIndex)
		//				{
		//					done = true;

		//					//increase cellDataInMemory size
		//					size_t size = m_cellDataInMemory.size();
		//					m_currentIndex = (int)size + 1;
		//					size_t increasedSize = (size_t)(size*(0.2));
		//					size = size + increasedSize;
		//					m_cellDataInMemory.resize(size);
		//				}
		//			}
		//		}
		//		else
		//			done =true;
		//	}
		//}
		return m_currentIndex;
	}

//------------------------------------------------------------------------------------

	size_t getCellInfoCacheIndexFromCellId(size_t cellId) const
	{
		size_t cellInfoIndex = 0;

		//take first '3*m_CellInfoCacheDepth' bits from cellId and treat them as cache index
		cellInfoIndex = cellId >> (64 - 3 * m_CellInfoCacheDepth);

		return cellInfoIndex;
	}

//------------------------------------------------------------------------------------

	size_t getCellIdFromCacheIndex(size_t index) const
	{
		size_t cellId = 0;

		//take first '3*m_CellInfoCacheDepth' bits from cellId and treat them as cache index
		cellId = cellId << (64 - 3 * m_CellInfoCacheDepth);

		return cellId;
	}


//------------------------------------------------------------------------------------

	size_t getCellInfoMapIndexFromCellId(size_t cellId) const
	{
		size_t cellInfoIndex = 0;

		//take first '3*m_CellInfoCacheDepth' bits from cellId and treat them as cache index.
		cellInfoIndex = cellId << 3 * m_CellInfoCacheDepth;
		cellInfoIndex = cellInfoIndex >> 3 * m_CellInfoCacheDepth;

		return cellInfoIndex;
	}

//------------------------------------------------------------------------------------

	//void setValidLeafCellInfo(size_t cellId, CellInfo<C> cellInfo)
	//{
	//	//static TimeSlice* t1 = TIMESLICES.Add("setValidLeafCellInfo");
	//	//t1->Start();
	//	size_t cellInfoCacheIndex = getCellInfoCacheIndexFromCellId(cellId);
	//	CellInfo<C> getCellInfo = m_CellInfoCache[cellInfoCacheIndex];

	//	if(getCellInfo.IsValidCell())
	//	{
	//		if(!getCellInfo.IsLeafCell())
	//		{
	//			size_t cellInfoMapIndex = getCellInfoMapIndexFromCellId(cellId);
	//			if(getCellInfo.GetCellInfoMap())
	//				(*getCellInfo.GetCellInfoMap())[cellInfoMapIndex] = cellInfo;
	//		}
	//		else
	//			m_CellInfoCache[cellInfoCacheIndex] = cellInfo;
	//	}
	//	//t1->Stop();
	//}

//------------------------------------------------------------------------------------

	CellInfo<C>* getValidLeafCellInfo(size_t cellId)const
	{
		//static TimeSlice* t1 = TIMESLICES.Add("getValidLeafCellInfo");
		//t1->Start();
		size_t cellInfoCacheIndex = getCellInfoCacheIndexFromCellId(cellId);
		CellInfo<C>* cellInfo = m_CellInfoCache[cellInfoCacheIndex];

		assert(cellInfo->IsValidCell());

		if(!cellInfo->IsLeafCell())
		{
			size_t cellInfoMapIndex = getCellInfoMapIndexFromCellId(cellId);
			CellIdCellInfoMap* cellInfoMap = cellInfo->GetCellInfoMap();
			if(cellInfoMap)
			{
				cellInfo = (*cellInfoMap)[cellInfoMapIndex];
				/*CellIdCellInfoMap:: const_iterator iter = cellInfo->GetCellInfoMap()->find(cellInfoMapIndex);
				if(iter != cellInfo->GetCellInfoMap()->end())
					cellInfo = iter->second;*/
			}
		}
		//t1->Stop();
		return cellInfo;
	}

//------------------------------------------------------------------------------------

	bool getCellDataFromCacheInMemory(C*& cellData)
	{
		bool status = true;
		//C* cellData = nullptr;

		//write celldata into file if celldata present
		//size_t index = getCurrentIndex();

		cellData = new C;
		m_cellDataInMemory.push_back(cellData);

		//cellData = m_cellDataInMemory[m_currentIndex];

		//if(!cellData)
		//{
		//	cellData = new C;
		//	m_cellDataInMemory[m_currentIndex] = cellData;
		//}
		//status = updateCurrentIndex();

		return status;
	}

//-----------------------------------------------------------------------------

	bool updateCurrentIndex()
	{
		bool status = true;
		if(m_currentIndex < (m_cellDataInMemory.size() - 1))
			m_currentIndex++;
		else
		{
			if(!CommonConstants::INCORE)
				m_currentIndex = 0;
			else
				status = false;
		}

		return status;
	}


//-----------------------------------------------------------------------------

	void releaseCache()
	{
		//clear all celldata in memory and delete it 
		for(size_t cellDataCount = 0; cellDataCount < m_cellDataInMemory.size(); ++cellDataCount)
		{
			C* cellData = m_cellDataInMemory[cellDataCount];

			if(cellData)
			{
				delete cellData;
				cellData = 0;
			}
		}

		m_cellDataInMemory.clear();

		//clear all cellinfo Cache  and its map pointer 
		for(size_t cellInfoCacheCount = 0; cellInfoCacheCount < m_CellInfoCache.size(); ++cellInfoCacheCount)
		{
			CellInfo<C>* cellInfoInArray = m_CellInfoCache[cellInfoCacheCount];
			if(cellInfoInArray)
			{
				if(cellInfoInArray->IsValidCell() && cellInfoInArray->GetCellInfoMap())
				{
					if(!cellInfoInArray->IsLeafCell())
					{
						CellIdCellInfoMap* cellInfoMap = m_CellInfoCache[cellInfoCacheCount]->GetCellInfoMap();

						if(cellInfoMap)
						{
							//complete iter to map and delete all map
							for(CellIdCellInfoMap::iterator itr = cellInfoMap->begin(); itr != cellInfoMap->end(); ++itr) 
							{
								//write cellId and cellinfo of map
								CellInfo<C>* cellInfoInMap = itr->second;
								if(cellInfoInMap)
								{
									delete cellInfoInMap;
									cellInfoInMap = 0;
								}
							}

							delete cellInfoMap;
							cellInfoMap = 0;
						}
					}
				}

				delete cellInfoInArray;
				cellInfoInArray = 0;
			}
		}

		m_CellInfoCache.clear();
	}

//-----------------------------------------------------------------------------

public:

	CacheManager(int maxCellInMemory, int cellInfoCacheDepth, FileHandler* binaryIOFile, CacheInterface* cache, bool readOctree = false)
	{
		m_cellDataInMemory.reserve(maxCellInMemory);
		//m_cellDataInMemory.resize(maxCellInMemory);
		m_currentIndex = 0;
		m_CellInfoCacheDepth = cellInfoCacheDepth;
		m_CellInfoCache.resize((size_t)pow(2.0, 3*m_CellInfoCacheDepth));
		m_binaryIOfile = binaryIOFile;
		m_cache = cache;

		if(readOctree)
			initalizeManager();
	}

//------------------------------------------------------------------------------------

	~CacheManager()
	{
		if(m_binaryIOfile)
		{
			delete m_binaryIOfile;
			m_binaryIOfile = 0;
		}

		releaseCache();
	}

//------------------------------------------------------------------------------------

	bool GetCellData(size_t cellId,bool& readData, int& readOffset, C*& cellData, bool loadCelldata = false, bool havingData = false)
	{
		bool status = true;
	
		CellInfo<C>* cellInfo = getValidLeafCellInfo(cellId);

		assert(cellInfo->IsLeafCell());
		if(!cellInfo->IsLeafCell())
			status = false;

		//set having data
		if(havingData)
		{
			if(!cellInfo->IsHavingData())
			{
				cellInfo->SetHavingData(havingData);
				//setValidLeafCellInfo(cellId, cellInfo);
			}
		}

		if(status)
		{
			readData = false;
			cellData = cellInfo->GetCellData();
			if(!cellData)
			{
				int offset = cellInfo->GetFileOffset();
				if(offset > -1) //cell data already written to file, need to load.
				{
					readData = true;
					readOffset = cellInfo->GetFileOffset();
				}

				//either we want new celldata or celldata from memory
				//get the cell data to be used from cell data cache in memory
				status =  getCellDataFromCacheInMemory(cellData);

				if(status)
				{
					cellInfo->SetCellData(cellData);
					//setValidLeafCellInfo(cellId,cellInfo);
				}
			}
		}

		return status;
	}

//------------------------------------------------------------------------------------
	void Divide(size_t cellId, int depth, std::vector<size_t>& childIds)
	{
		CellInfo<C>* cellInfo = getValidLeafCellInfo(cellId);
		C* cellData = cellInfo->GetCellData();
		assert(cellData);
		cellInfo->SetCellData(nullptr);
		cellInfo->SetFlags(0);
		//setValidLeafCellInfo(cellId, cellInfo);

		for(size_t i=0; i < childIds.size(); i++)
		{
			AddNewCellInfo(depth, childIds[i]);
		}

		//reuse the celldata for child[0]
		cellInfo = getValidLeafCellInfo(childIds[0]);
		cellInfo->SetCellData(cellData);
		//setValidLeafCellInfo(childIds[0], cellInfo);
	}

//------------------------------------------------------------------------------------
	void AddNewCellInfo(int depth, size_t cellId)
	{
		size_t cellInfoCacheIndex = getCellInfoCacheIndexFromCellId(cellId);

		CellInfo<C>* cellInfo = nullptr;
		cellInfo = m_CellInfoCache[cellInfoCacheIndex];

		if(!cellInfo)
		{
			//create cellinfo
			cellInfo = new CellInfo<C>;
			m_CellInfoCache[cellInfoCacheIndex] = cellInfo;
		}

		cellInfo->SetValidCell(true);
		if(depth > m_CellInfoCacheDepth)
		{
			cellInfo->SetLeafCell(false);
			CellIdCellInfoMap *cellInfoMap = cellInfo->GetCellInfoMap();
			if(!cellInfoMap)
			{
				cellInfoMap = new CellIdCellInfoMap;
				cellInfo->SetCellInfoMap(cellInfoMap);
			}
			size_t cellInfoMapIndex = getCellInfoMapIndexFromCellId(cellId);

			CellInfo<C>* leafCellinfo = new CellInfo<C>;
			leafCellinfo->SetValidCell(true);
			leafCellinfo->SetLeafCell(true);
			leafCellinfo->SetLoadedCell(false);

			(*cellInfoMap)[cellInfoMapIndex] = leafCellinfo;
		}
		else
		{
			cellInfo->SetLeafCell(true);
			cellInfo->SetLoadedCell(false);
		}
	}

//------------------------------------------------------------------------------------

	void GetAllCellIdInMemory(std::vector<size_t>& allCellId)
	{
		allCellId.resize(m_cellDataInMemory.size());

		for(size_t cellDataCount = 0; cellDataCount < m_cellDataInMemory.size(); ++cellDataCount)
		{
			C* cellData = m_cellDataInMemory[cellDataCount];

			if(cellData)
				allCellId[cellDataCount] = cellData->m_cellId;
		}
	}

//------------------------------------------------------------------------------------

	void SetLoadedCell(size_t cellId)
	{
		CellInfo<C>* cellInfo = getValidLeafCellInfo(cellId);
		cellInfo->SetLoadedCell(true);

		//setValidLeafCellInfo(cellId, cellInfo);
	}

//------------------------------------------------------------------------------------

	C* GetCellInMemory(size_t index)
	{
		return m_cellDataInMemory[index];
	}

//------------------------------------------------------------------------------------
	void SetHavingData(size_t cellId, bool havingData)
	{
		CellInfo<C>* cellInfo = getValidLeafCellInfo(cellId);
		if(!cellInfo->IsHavingData())
		{
			cellInfo->SetHavingData(havingData);
			//setValidLeafCellInfo(cellId, cellInfo);
		}
	}

//------------------------------------------------------------------------------------

	bool IsHavingData(size_t cellId)const
	{
		CellInfo<C>* cellInfo = getValidLeafCellInfo(cellId);
		return cellInfo->IsHavingData();
	}

//------------------------------------------------------------------------------------

	void Flush()
	{
		BInputOutputFile *file = new BInputOutputFile(L"C:\\Temp\\CacheWrite.txt", false);

		m_cache->SetFile(file);
		//write all celldata present in memory to file
		for(size_t index = 0; index < m_cellDataInMemory.size(); ++index)
		{
			writeCell(index,true);
		}

		delete file;
	}

//------------------------------------------------------------------------------------

	void ReleaseManager(size_t &fileSize)
	{
		BInputOutputFile file(L"C:\\Temp\\CacheManagerEntity.txt", false);

		//write numCellInfo in array
		size_t numCellInfoInArray = m_CellInfoCache.size();
		size_t numCellInfoInMap = 0;
		CellInfo<C>* cellInfo = nullptr;
		CellInfo<C>* cellInfoInMap = nullptr;
		file << (__int64)numCellInfoInArray;

		for(size_t index = 0; index < numCellInfoInArray; ++index)
		{
			cellInfo = m_CellInfoCache[index];

			//write offset and flags in file
			file << cellInfo->GetFlags();
			file << cellInfo->GetFileOffset();

			if(cellInfo->IsValidCell())
			{
				if(!cellInfo->IsLeafCell())
				{
					//write map entity in file
					numCellInfoInMap = cellInfo->GetCellInfoMap()->size();
					file << (__int64)numCellInfoInMap;

					//write 
					for(CellIdCellInfoMap::iterator itr = cellInfo->GetCellInfoMap()->begin(); itr != cellInfo->GetCellInfoMap()->end(); ++itr) 
					{
						//write cellId and cellinfo of map
						file << (__int64)itr->first;
						cellInfoInMap = itr->second;
						file << cellInfoInMap->GetFlags();
						file << cellInfoInMap->GetFileOffset();
					}
				}
			}
		}

		fileSize = file.GetFileCurrentPosition();
	}

//------------------------------------------------------------------------------------

size_t SizeOf()const
{
	size_t size = sizeof(*this);
	size+= SizeOfPtrVectorEx(m_cellDataInMemory);
	size+= SizeOfPtrVectorEx(m_CellInfoCache);

	return size;
}

//------------------------------------------------------------------------------------
void ShrinkArray()
{
	m_cellDataInMemory.shrink_to_fit();
	size_t totalCell = m_cellDataInMemory.size();
	for (size_t cellCount = 0; cellCount < totalCell; ++cellCount)
	{
		m_cellDataInMemory[cellCount]->ShrinkArray();
	}
}

//------------------------------------------------------------------------------------
};
#pragma pack(pop)   /* restore original from stack */
}
#endif