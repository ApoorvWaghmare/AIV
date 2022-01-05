// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include "CUUnitConversionUtil.h"
#include "assert.h"
#include "CUPoint2D.h"

using namespace std;

//-----------------------------------------------------------------------------

namespace CommonUtil
{
	
UnitConversionUtil* UnitConversionUtil::m_unitConversionUtil = nullptr;

//-----------------------------------------------------------------------------

UnitConversionUtil::UnitConversionUtil()
{
	m_userUnit = MILLIMETERS;
	m_prevUserUnit  = MILLIMETERS;
	m_importUnit = MILLIMETERS;
	m_uToIConversionFactor = 1.;
	m_iToUConversionFactor = 1.;
}

//-----------------------------------------------------------------------------

UnitConversionUtil::~UnitConversionUtil()
{
	
}

//-----------------------------------------------------------------------------

const double UnitConversionUtil::GetUToIConversionFactor()
{
	return m_uToIConversionFactor;
}

//-----------------------------------------------------------------------------

const double UnitConversionUtil::GetIToUConversionFactor()
{
	return m_iToUConversionFactor;
}

//-----------------------------------------------------------------------------

void UnitConversionUtil::ModifyUnits(const Units unit)
{
	m_prevUserUnit = m_userUnit;
	m_userUnit = unit;
	UserUnitToInternalUnit();
	InternalUnitToUserUnit();
}

//-----------------------------------------------------------------------------

UnitConversionUtil* UnitConversionUtil::GetInstance()
{
	if(!m_unitConversionUtil)
	{
		m_unitConversionUtil = new UnitConversionUtil();
	}

	return m_unitConversionUtil;
}

//-----------------------------------------------------------------------------

void UnitConversionUtil::DeleteInstance()
{
	if(m_unitConversionUtil)
	{
		delete m_unitConversionUtil;
		m_unitConversionUtil = 0;
	}
}

//-----------------------------------------------------------------------------

bool UnitConversionUtil::UserUnitToInternalUnit()
{
	bool conversionStatus = true;

	if(MICRONS == m_userUnit)
	{
		m_uToIConversionFactor =  1e-3;
	}
	else if(MILLIMETERS == m_userUnit)
	{
		m_uToIConversionFactor =  1.0;
	}
	else if(CENTIMETERS == m_userUnit)
	{
		m_uToIConversionFactor =  10.0;
	}
	else if(METERS == m_userUnit)
	{
		m_uToIConversionFactor =  1e3;
	}
	else if(INCHES == m_userUnit)
	{
		m_uToIConversionFactor =  25.4;
	}
	else if(FEET == m_userUnit)
	{
		m_uToIConversionFactor =  304.8;
	}
	else if (KILOMETERS == m_userUnit)
	{
		m_uToIConversionFactor = 1e6;
	}
	else
	{
		conversionStatus = false;
	}

	return conversionStatus;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

const double UnitConversionUtil::GetUserUnitToInternalUnitConversionFactor(const Units unit)const
{
	double conversionFactor = 1.0;

	if(MICRONS == unit)
	{
		conversionFactor =  1e-3;
	}
	else if(MILLIMETERS == unit)
	{
		conversionFactor =  1.0;
	}
	else if(CENTIMETERS == unit)
	{
		conversionFactor =  10.0;
	}
	else if(METERS == unit)
	{
		conversionFactor =  1e3;
	}
	else if(INCHES == unit)
	{
		conversionFactor =  25.4;
	}
	else if(FEET == unit)
	{
		conversionFactor =  304.8;
	}
	
	return conversionFactor;
}

//-----------------------------------------------------------------------------

bool UnitConversionUtil::InternalUnitToUserUnit()
{
	bool conversionStatus = true;

	if(MICRONS == m_userUnit)
	{
		m_iToUConversionFactor =  1000;
	}
	else if(MILLIMETERS == m_userUnit)
	{
		m_iToUConversionFactor =  1;
	}
	else if(CENTIMETERS == m_userUnit)
	{
		m_iToUConversionFactor =  0.1;
	}
	else if(METERS == m_userUnit)
	{
		m_iToUConversionFactor =  0.001;
	}
	else if(INCHES == m_userUnit)
	{
		m_iToUConversionFactor =  0.0393700787;
	}
	else if(FEET == m_userUnit)
	{
		m_iToUConversionFactor =  0.0032808399;
	}
	else
	{
		conversionStatus = false;
	}
	
	return conversionStatus;
}

//-----------------------------------------------------------------------------

void UnitConversionUtil::GetUnitInfoForIGESWrite(std::string &unitFlag, std::string &units)
{
	//returns parameters as required in IGES write
	//Following is the unitFlag- unit Map
	//Value - Model Units
	//	1	- Inches(default)
	//	2	- Millimeters
	//	3	- (See Parameter 15 for name of units)
	//	4	- Feet
	//	5	- Miles
	//	6	- Meters
	//	7	- Kilometers
	//	8	- Mils(i.e., 0.001 inch)
	//	9	- Microns
	//	10  - Centimeters
	//	11  - Microinches

	//Following is the Value - unit Map

	//  Value			- Model Units
	//  2HIN or 4HINCH	- Inches(default)
	//  2HMM			- Millimeters
	//  2HFT			- Feet
	//  2HMI			- Miles
	//  1HM				- Meters
	//  2HKM			- Kilometers
	//  3HMIL			- Mils
	//  2HUM			- Microns
	//  2HCM			- Centimeters
	//  3HUIN			- Microinches


	Units unit = GetUserUnit();
	switch(unit)
	{
	case 0://µm
		{
			unitFlag = "9";
			units = "2HUM";
			break;
		}
	case 1://mm
		{
			unitFlag = "2";
			units = "2HMM";
			break;
		}
	case 2://cm
		{
			unitFlag = "10";
			units = "2HCM";
			break;
		}
	case 3://m
		{
			unitFlag = "6";
			units = "1HM";
			break;
		}
	case 4://in
		{
			unitFlag = "1";
			units = "1HIN";
			break;
		}
	case 5://ft
		{
			unitFlag = "4";
			units = "2HFT";
			break;
		}
	case 6://km
		{
			unitFlag = "7";
			units = "2HKM";
			break;
		}
	default:
		{
			unitFlag = "2";
			units = "2HMM";
			break;
		}
	}
}

//-----------------------------------------------------------------------------

void UnitConversionUtil::SetImportUnit(const Units unit)
{
	m_importUnit = unit;
}

//-----------------------------------------------------------------------------

double UnitConversionUtil::calculateConversionFactorForRetrieve(const Units fromUnit, const Units toUnit)
{
	double conversionFactor = 1.;

	switch (fromUnit)
	{
	case MICRONS:
		conversionFactor = fromMicronTo(toUnit);
		break;
	case MILLIMETERS:
		conversionFactor = fromMillimeterTo(toUnit);
		break;
	case CENTIMETERS:
		conversionFactor = fromCentimeterTo(toUnit);
		break;
	case METERS:
		conversionFactor = fromMeterTo(toUnit);
		break;
	case INCHES:
		conversionFactor = fromInchTo(toUnit);
		break;
	case FEET:
		conversionFactor = fromFootTo(toUnit);
		break;
	case KILOMETERS:
		conversionFactor = fromKilometerTo(toUnit);
		break;
	default:
		conversionFactor = 1.;
		break;
	}

	return conversionFactor;
}

//-----------------------------------------------------------------------------

double UnitConversionUtil::fromMicronTo(const Units toUnit)
{
	double conversionFactor = 1.;

	if(MICRONS == toUnit)
	{
		conversionFactor =  1.;
	}
	else if(MILLIMETERS == toUnit)
	{
		conversionFactor = 0.001 ;
	}
	else if(CENTIMETERS == toUnit)
	{
		conversionFactor =  0.0001;
	}
	else if(METERS == toUnit)
	{
		conversionFactor =  0.000001;
	}
	else if(INCHES == toUnit)
	{
		conversionFactor =  0.0000393701;
	}
	else if(FEET == toUnit)
	{
		conversionFactor =  0.0000032808;
	}
	else if (KILOMETERS == toUnit)
	{
		conversionFactor = 1e-9;
	}

	return conversionFactor;
}
	
//-----------------------------------------------------------------------------

double UnitConversionUtil::fromMillimeterTo(const Units toUnit)
{
	double conversionFactor = 1.;

	if(MICRONS == toUnit)
	{
		conversionFactor =  1000;
	}
	else if(MILLIMETERS == toUnit)
	{
		conversionFactor =  1;
	}
	else if(CENTIMETERS == toUnit)
	{
		conversionFactor =  0.1;
	}
	else if(METERS == toUnit)
	{
		conversionFactor =  0.001;
	}
	else if(INCHES == toUnit)
	{
		conversionFactor =  0.0393700787;
	}
	else if(FEET == toUnit)
	{
		conversionFactor =  0.0032808399;
	}
	else if (KILOMETERS == toUnit)
	{
		conversionFactor = 1e-6;
	}
	
	return conversionFactor;
}

//-----------------------------------------------------------------------------
	
double UnitConversionUtil::fromCentimeterTo(const Units toUnit)
{
	double conversionFactor = 1.;

	if(MICRONS == toUnit)
	{
		conversionFactor =  10000;
	}
	else if(MILLIMETERS == toUnit)
	{
		conversionFactor =  10;
	}
	else if(CENTIMETERS == toUnit)
	{
		conversionFactor =  1;
	}
	else if(METERS == toUnit)
	{
		conversionFactor =  0.01;
	}
	else if(INCHES == toUnit)
	{
		conversionFactor =  0.3937007874;
	}
	else if(FEET == toUnit)
	{
		conversionFactor =  0.032808399;
	}
	else if (KILOMETERS == toUnit)
	{
		conversionFactor = 1e-5;
	}
	
	return conversionFactor;
}
	
//-----------------------------------------------------------------------------

double UnitConversionUtil::fromMeterTo(const Units toUnit)
{
	double conversionFactor = 1.;

	if(MICRONS == toUnit)
	{
		conversionFactor =  1000000;
	}
	else if(MILLIMETERS == toUnit)
	{
		conversionFactor =  1000;
	}
	else if(CENTIMETERS == toUnit)
	{
		conversionFactor =  100;
	}
	else if(METERS == toUnit)
	{
		conversionFactor =  1;
	}
	else if(INCHES == toUnit)
	{
		conversionFactor =  39.37007874;
	}
	else if(FEET == toUnit)
	{
		conversionFactor =  3.280839895;
	}
	else if (KILOMETERS == toUnit)
	{
		conversionFactor = 1e-3;
	}
	
	return conversionFactor;
}

//-----------------------------------------------------------------------------
	
double UnitConversionUtil::fromInchTo(const Units toUnit)
{
	double conversionFactor = 1.;
	
	if(MICRONS == toUnit)
	{
		conversionFactor =  25400;
	}
	else if(MILLIMETERS == toUnit)
	{
		conversionFactor =  25.4;
	}
	else if(CENTIMETERS == toUnit)
	{
		conversionFactor =  2.54;
	}
	else if(METERS == toUnit)
	{
		conversionFactor =  0.0254;
	}
	else if(INCHES == toUnit)
	{
		conversionFactor =  1;
	}
	else if(FEET == toUnit)
	{
		conversionFactor =  0.0833333333;
	}
	else if (KILOMETERS == toUnit)
	{
		conversionFactor = 0.0000254;
	}
	return conversionFactor;
}
	
//-----------------------------------------------------------------------------

double UnitConversionUtil::fromFootTo(const Units toUnit)
{
	double conversionFactor = 1.;

	if(MICRONS == toUnit)
	{
		conversionFactor =  304800;
	}
	else if(MILLIMETERS == toUnit)
	{
		conversionFactor =  304.8;
	}
	else if(CENTIMETERS == toUnit)
	{
		conversionFactor =  30.48;
	}
	else if(METERS == toUnit)
	{
		conversionFactor =  0.3048;
	}
	else if(INCHES == toUnit)
	{
		conversionFactor =  12;
	}
	else if(FEET == toUnit)
	{
		conversionFactor =  1;
	}
	else if (KILOMETERS == toUnit)
	{
		conversionFactor = 0.0003048;
	}
	
	return conversionFactor;
}

//-----------------------------------------------------------------------------

double UnitConversionUtil::fromKilometerTo(const Units toUnit)
{
	double conversionFactor = 1.;

	if (MICRONS == toUnit)
	{
		conversionFactor = 1e9;
	}
	else if (MILLIMETERS == toUnit)
	{
		conversionFactor = 1e6;
	}
	else if (CENTIMETERS == toUnit)
	{
		conversionFactor = 1e5;
	}
	else if (METERS == toUnit)
	{
		conversionFactor = 1e3;
	}
	else if (INCHES == toUnit)
	{
		conversionFactor = 39370.07874016;
	}
	else if (FEET == toUnit)
	{
		conversionFactor = 3280.839895013;
	}
	else if (KILOMETERS == toUnit)
	{
		conversionFactor = 1;
	}

	return conversionFactor;
}

//-----------------------------------------------------------------------------

double UnitConversionUtil::GetConversionFactorForRetrieve()
{
	double conversionFactor = 1.;

	if(m_importUnit == MILLIMETERS)
	{
		conversionFactor =  1.;
	}
	else
	{
		if(m_importUnit == m_userUnit)
		{
			conversionFactor = GetUToIConversionFactor(); 
		}
		else
		{
			conversionFactor = calculateConversionFactorForRetrieve(m_importUnit, MILLIMETERS);
		}
	}

	return conversionFactor;
}

//-----------------------------------------------------------------------------

bool UnitConversionUtil::Format(string& Str)
{
	bool success = false;
	int precision = getPrecisionForCurrentUnit();

	if(0 < precision && 7 > precision)
	{
		string strPrecised;
		size_t iCount = 0;
		for(iCount = 0; iCount < Str.length() && Str[iCount] != '.'; ++iCount)
		{
			strPrecised += Str[iCount];
		}
		strPrecised += '.';
		for(size_t jCount = iCount+1; jCount < iCount + precision + 1; ++jCount)
		{
			if(jCount > Str.length()-1)
			{
				strPrecised += '0';
				continue;
			}
			strPrecised += Str[jCount];
		}
		Str = strPrecised;
		success = true;
	}
	return success;
}

//-----------------------------------------------------------------------------

int UnitConversionUtil::getPrecisionForCurrentUnit()
{
	int precision = 1;
	switch(m_userUnit)
	{
	case MICRONS:
		precision = 1;
		break;
	case MILLIMETERS:
		precision = 3;
		break;
	case CENTIMETERS:
		precision = 4;
		break;
	case METERS:
		precision = 6;
		break;
	case INCHES:
		precision = 4;
		break;
	case FEET:
		precision = 5;
		break;
	case KILOMETERS:
		precision = 9;
		break;
	default:
		precision = 1;
		break;
	}
	return precision;
}

//-----------------------------------------------------------------------------

void UnitConversionUtil::GetNearestPoint(const double &minX, const double &minY, const double &maxX, const double &maxY, 
										 const double &screenX, const double &screenY, double &modifiedMinX, double &modifiedMinY)
{
	Point2D p1(minX, minY);
	Point2D p2(maxX, minY);
	Point2D p3(maxX, maxY);
	Point2D p4(minX, maxY);

	Point2D p5;
	Point2D p6;
	Point2D p7;
	Point2D p8;

	getMidPoint(p1, p2, p5);
	getMidPoint(p2, p3, p6);
	getMidPoint(p3, p4, p7);
	getMidPoint(p1, p4, p8);

	Point2D p9(screenX, screenY);

	double d1 = p1.Distance(p9);
	double d2 = p2.Distance(p9);
	double d3 = p3.Distance(p9);
	double d4 = p4.Distance(p9);
	double d5 = p5.Distance(p9);
	double d6 = p6.Distance(p9);
	double d7 = p7.Distance(p9);
	double d8 = p8.Distance(p9);

	double minDist = 0.0;
	std::vector<double> distances;
	distances.push_back(d1);
	distances.push_back(d2);
	distances.push_back(d3);
	distances.push_back(d4);
	distances.push_back(d5);
	distances.push_back(d6);
	distances.push_back(d7);
	distances.push_back(d8);

	for(size_t i = 0; i < distances.size(); ++i)
	{
		for(size_t j = i + 1; j < distances.size(); ++j)
		{
			if(distances[i] > distances[j])
			{
				double tempDist = distances[i];
				distances[i] = distances[j];
				distances[j] = tempDist;
			}
		}
	}

	if(distances.empty())
	{
		modifiedMinX = minX;
		modifiedMinY = minY;

		return;
	}

	if((d1 - distances[0]) < CommonConstants::PNT_TOL)
	{
		modifiedMinX = minX;
		modifiedMinY = minY;
	}
	else if((d2 - distances[0]) < CommonConstants::PNT_TOL)
	{
		modifiedMinX = maxX;
		modifiedMinY = minY;
	}
	else if((d3 - distances[0]) < CommonConstants::PNT_TOL)
	{
		modifiedMinX = maxX;
		modifiedMinY = maxY;
	}
	else if((d4 - distances[0]) < CommonConstants::PNT_TOL)
	{
		modifiedMinX = minX;
		modifiedMinY = maxY;
	}
	else if((d5 - distances[0]) < CommonConstants::PNT_TOL)
	{
		modifiedMinX = p5.GetX();
		modifiedMinY = p5.GetY();
	}
	else if((d6 - distances[0]) < CommonConstants::PNT_TOL)
	{
		modifiedMinX = p6.GetX();
		modifiedMinY = p6.GetY();
	}
	else if((d7 - distances[0]) < CommonConstants::PNT_TOL)
	{
		modifiedMinX = p7.GetX();
		modifiedMinY = p7.GetY();
	}
	else if((d8 - distances[0]) < CommonConstants::PNT_TOL)
	{
		modifiedMinX = p8.GetX();
		modifiedMinY = p8.GetY();
	}
	else
	{
		modifiedMinX = minX;
		modifiedMinY = minY;
	}
}

//-----------------------------------------------------------------------------

void UnitConversionUtil::getMidPoint(const Point2D &startPoint, const Point2D &endPoint, Point2D &midPoint)
{
	midPoint.SetX((startPoint.GetX() + endPoint.GetX()) / 2);
	midPoint.SetY((startPoint.GetY() + endPoint.GetY()) / 2);
}

//-----------------------------------------------------------------------------
//used in FormatDlgItem() of creation dlgs 
double UnitConversionUtil::GetConversionFactorFromPreviousUnit()//
{
	return	calculateConversionFactorForRetrieve(m_prevUserUnit, m_userUnit);
}

//-----------------------------------------------------------------------------

void UnitConversionUtil::Reset()
{
	m_userUnit = MILLIMETERS;
	m_prevUserUnit  = MILLIMETERS;
	m_importUnit = MILLIMETERS;
	m_uToIConversionFactor = 1.;
	m_iToUConversionFactor = 1.;
}

//-----------------------------------------------------------------------------

}