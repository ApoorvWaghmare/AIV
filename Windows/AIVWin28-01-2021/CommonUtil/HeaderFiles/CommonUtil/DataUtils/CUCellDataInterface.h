//=============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
//=============================================================================

#ifndef _CELLDATA_INTERFACE_H_
#define _CELLDATA_INTERFACE_H_
#include <vector>


class CellDataInterface
{
public:
	virtual void IncreaseCellDataEntityRefCount() = 0;

	virtual void DecreaseCellDataEntityRefCount() = 0;
};

#endif