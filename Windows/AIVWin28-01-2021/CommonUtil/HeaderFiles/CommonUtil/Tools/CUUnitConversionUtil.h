// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================
#ifndef _UNIT_CONVERSION_UTIL_H_
#define _UNIT_CONVERSION_UTIL_H_

#include "CUCommonConstants.h"
#include <string>

//#include "CUPoint2D.h"

using namespace std;

namespace CommonUtil
{

class Point2D;

class UnitConversionUtil
{
	static UnitConversionUtil* m_unitConversionUtil;

	Units m_userUnit;

	Units m_prevUserUnit;

	Units m_importUnit;

	double m_uToIConversionFactor;

	double m_iToUConversionFactor;	

	//class made singleton
	UnitConversionUtil();

	~UnitConversionUtil();

	UnitConversionUtil(UnitConversionUtil const& copy);     	// Not Implemented

    UnitConversionUtil& operator=(UnitConversionUtil const& copy);	

	//double calculateConversionFactorForRetrieve(const Units fromUnit, const Units toUnit);

	double fromMicronTo(const Units toUnit);

	double fromMillimeterTo(const Units toUnit);

	double fromCentimeterTo(const Units toUnit);

	double fromMeterTo(const Units toUnit);

	double fromInchTo(const Units toUnit);

	double fromFootTo(const Units toUnit);

	double fromKilometerTo(const Units toUnit);

	double calculateConversionFactorForRetrieve(const Units fromUnit, const Units toUnit);
	
	int getPrecisionForCurrentUnit();

	void getMidPoint(const CommonUtil::Point2D &startPoint, const CommonUtil::Point2D &endPoint, CommonUtil::Point2D &midPoint);

public:

	static UnitConversionUtil* GetInstance();

	static void DeleteInstance();

	const double GetUToIConversionFactor();
	
	const double GetIToUConversionFactor();

	void ModifyUnits(const Units unit);

	bool UserUnitToInternalUnit();

	const double GetUserUnitToInternalUnitConversionFactor(const Units unit)const;

	bool InternalUnitToUserUnit();

	Units GetUserUnit()const{return m_userUnit;}

	Units GetImportUnit()const{return m_importUnit;}

	void SetImportUnit(const Units unit);

	//units in IGES
	void GetUnitInfoForIGESWrite(std::string &unitFlag, std::string &units);

	double GetConversionFactorForRetrieve();

	bool Format(string& Str);

	void GetNearestPoint(const double &minX, const double &minY, const double &maxX, const double &maxY, 
					     const double &screenX, const double &screenY, double &modifiedMinX, double &modifiedMinY);

	double GetConversionFactorFromPreviousUnit();

	Units GetPrevUserUnit()const{return m_prevUserUnit;}

	void Reset();
};

}

#endif