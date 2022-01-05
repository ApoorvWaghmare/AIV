// ============================================================================
//			Copyright (c) 2005 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#include <algorithm>

//math include files
#include "CULineFitting.h"
#include "CUSqMatrix.h"
#include "CUSegment3D.h"

namespace CommonUtil
{

//-----------------------------------------------------------------------------

void LineFitting::Execute(const std::vector<CommonUtil::Point3D> &dataPoints)
{
	//input points are not collinear
	
	// the centroid of all the points is calculated
	size_t n = dataPoints.size();
	double x = 0, y = 0, z = 0;
	int i=0;
	for(i = 0; i < n; i++)
	{
		 x += dataPoints[i].GetX();
		 y += dataPoints[i].GetY();
		 z += dataPoints[i].GetZ();
	}
	
	x /= n;
	y /= n;
	z /= n;

	m_linePoint.Set(x, y, z);

	// compute the covariance matrix
    CommonUtil::SqMatrix covariance(3);
	for(i = 0; i < n; i++)
	{
        // temp2 = | d1 0 0 |
        //         | d2 0 0 |
        //         | d3 0 0 |
//         temp2.Init( 3, 3 );
// 		temp2(0,0) = dataPoints[i].GetX() - m_linePoint.GetX();
// 		temp2(0,1) = dataPoints[i].GetY() - m_linePoint.GetY();
// 		temp2(0,2) = dataPoints[i].GetZ() - m_linePoint.GetZ();
// 		
// 		temp2.GetTranspose(temp1);
        // temp1 = | d1 d2 d3 |
        //         | 0  0  0  |
        //         | 0  0  0  |

// 		temp1.Multiply(temp2, temp);
        // temp1 * temp2 = | d1 0 0 | * | d1 d2 d3 | = | d1^2  d1*d2 d1*d3 |
        //                 | d2 0 0 |   | 0  0  0  |   | d1*d2 d2*d2 d2*d3 |
        //                 | d3 0 0 |   | 0  0  0  |   | d1*d3 d2*d3 d3*d3 |
        //covariance.AddTo(temp);
        double d1 = dataPoints[i].GetX() - m_linePoint.GetX();
        double d2 = dataPoints[i].GetY() - m_linePoint.GetY();
        double d3 = dataPoints[i].GetZ() - m_linePoint.GetZ();
        double d1d2 = d1*d2;
        double d1d3 = d1*d3;
        double d2d3 = d2*d3;
        covariance( 0,0 ) += d1*d1;
        covariance( 0,1 ) += d1d2;
        covariance( 0,2 ) += d1d3;
        covariance( 1,0 ) += d1d2;
        covariance( 1,1 ) += d2*d2;
        covariance( 1,2 ) += d2d3;
        covariance( 2,0 ) += d1d3;
        covariance( 2,1 ) += d2d3;
        covariance( 2,2 ) += d3*d3;
	}

	std::vector<double> maxEigenVector(3);

	// The eigen CommonUtil::Vector corresponding to the maximum eigen value for the 
	// matrix "covariance" is obtained. This is the direction of line.
	covariance.GetMaxEigenVector(maxEigenVector);

	/*m_lineDirection.Set(maxEigenVector[0], maxEigenVector[1],
	maxEigenVector[2]);*/

	if (m_lineDirection.IsValid())
	{
		//Adjust Axis direction w.r.t. guess axis
		CommonUtil::Vector newAxis(maxEigenVector[0], maxEigenVector[1],
			maxEigenVector[2]);

		if (m_lineDirection.DotProduct(newAxis) < 0)
			newAxis.Reverse();

		m_lineDirection = newAxis;
	}
	else
	{
		m_lineDirection.Set(maxEigenVector[0], maxEigenVector[1],
			maxEigenVector[2]);
	}

	m_lineDirection.Normalize();
}

//-----------------------------------------------------------------------------

double LineFitting::GetDeviation(const std::vector<CommonUtil::Point3D> &dataPoints)const
{
	CommonUtil::Point3D otherPoint;
	double distance = 20;
	m_lineDirection.GetPointAlongVector(m_linePoint, otherPoint, distance);

	CommonUtil::Segment3D segment(m_linePoint, otherPoint);

	double snappingTol = CommonUtil::CommonConstants::PNT_TOL;
	size_t numDataPoints = dataPoints.size();
	double totalDistance = 0;
    CommonUtil::Point3D linePoint;
	for(int i = 0; i < numDataPoints; i++)
	{
		double uVal;
		segment.Project(dataPoints[i], snappingTol, uVal);
		segment.GetPoint(uVal, linePoint );

		double dist = dataPoints[i].Distance(linePoint);
		totalDistance += dist;
	}

	totalDistance = totalDistance/numDataPoints;
	return totalDistance;
}

//-----------------------------------------------------------------------------

static bool IsSquareValueExceedDistanceBy( double squareDistanceToTest_, double referenceDistance_, double tolerance_ )
{
    assert( referenceDistance_ >= 0.0 );
    assert( tolerance_ >= 0.0 );

    // We want to test if( fabs( sqrt( squareDistanceToTest_ ) - referenceDistance_ ) > tolerance_ )

    // Defining distanceToTest_ = sqrt(squareDistanceToTest_ ) and assuming it represent a distance value
    // Also assuming that referenceDistance_ represent a distance value
    // So we have distanceToTest_ >= 0 and referenceDistance_ >= 0
    // we change the condition to if(fabs( distanceToTest_ - referenceDistance_) > tolerance_)

    // if distanceToTest_ >= referenceDistance_ >= 0 thus distanceToTest_^2 >= referenceDistance_^2
    // then we convert it to if( distanceToTest_ - referenceDistance_ > tolerance_) 
    // then we convert it to if( distanceToTest_ > tolerance_ + referenceDistance_ ) 
    // then we convert it to if( distanceToTest_^2 > (tolerance_ + referenceDistance_)^2 ) 

    // if 0 <= distanceToTest_ <= referenceDistance_ thus distanceToTest_^2 <= referenceDistance_^2
    // then we convert it to if( -distanceToTest_ + referenceDistance_ > tolerance_) 
    // then we convert it to if( referenceDistance_ - tolerance_ > distanceToTest_ ) 
    // then we convert it to if( (referenceDistance_ - tolerance_)^2 > distanceToTest_^2 ) 
    // a) if referenceDistance_ - tolerance_ >= 0  
    //       then we convert it to if( (referenceDistance_ - tolerance_)^2 > distanceToTest_^2 ) 
    // b) if referenceDistance_ - tolerance_ <= 0  then this is always false since distanceToTest_ >= 0
    //       <=>  referenceDistance_ <= tolerance_

    // Thus the condition change to : 
    bool exceedTol = false;
    if( squareDistanceToTest_ >= referenceDistance_*referenceDistance_ )
    {
        double tempVal = (referenceDistance_ + tolerance_);
        exceedTol = squareDistanceToTest_ > (tempVal*tempVal);
    }
    else // squareDistanceToTest_ < referenceDistance_*referenceDistance_
    {
        if( referenceDistance_ <= tolerance_ )
        {
            exceedTol = false;
        }
        else
        {
            double tempVal = (referenceDistance_ - tolerance_);
            exceedTol = (tempVal*tempVal) > squareDistanceToTest_;
        }
    }
    //assert( ( fabs( sqrt( squareDistanceToTest_ ) - referenceDistance_ ) > tolerance_ ) == exceedTol );
    return exceedTol;
}

//-----------------------------------------------------------------------------

void LineFitting::GetExactLine(std::vector<CommonUtil::Point3D> dataPoints, 
							   double &deviation)
{
	do
	{
		Execute(dataPoints);

		deviation = GetDeviation(dataPoints);

		CommonUtil::Point3D otherPoint;
		double distance = 20;
		m_lineDirection.GetPointAlongVector(m_linePoint, otherPoint, distance);

		CommonUtil::Segment3D segment(m_linePoint, otherPoint);
		double snappingTol = CommonUtil::CommonConstants::PNT_TOL;

		size_t numDataPoints = dataPoints.size();
        CommonUtil::Point3D tmpPoint;
		std::vector<CommonUtil::Point3D> tmpDataPoints;
		for(int i = 0; i < numDataPoints; i++)
		{
			double uVal;
			segment.Project(dataPoints[i], snappingTol, uVal);
			segment.GetPoint(uVal, tmpPoint );
			double tmpDistSqr = tmpPoint.SqrDistance(dataPoints[i]);

			//if(fabs(tmpDist - deviation) > 1e-2)
            if( IsSquareValueExceedDistanceBy( tmpDistSqr, deviation, 1e-2 ) )
			{
				CommonUtil::Vector tmpVector(dataPoints[i], tmpPoint);
				CommonUtil::Point3D tmpDataPoint = dataPoints[i];
				if(tmpVector.IsValid())
				{
					tmpVector.GetPointAlongVector(dataPoints[i], tmpDataPoint, 
											  deviation);
				}
				tmpDataPoints.push_back(tmpDataPoint);
			}

			else
			{
				tmpDataPoints.push_back(tmpPoint);
			}
		}
		dataPoints.clear();
		dataPoints = tmpDataPoints;
	}while(deviation > 1e-2);	
}

//-----------------------------------------------------------------------------

void LineFitting::RefineExecute(const std::vector<CommonUtil::Point3D> &dataPoints)
{
    double minDev = 1.0e300;
	double deviation1;
	double deviation2;
	double ctr = 0;
    size_t numDataPoints = dataPoints.size();
	std::vector<CommonUtil::Point3D> tmpDataPoints;
    tmpDataPoints.reserve( numDataPoints );
    CommonUtil::Point3D tmpPoint;
	do
	{
        tmpDataPoints.clear();
        // Already computed from the previous loop
		deviation1 = ctr == 0 ? GetDeviation(dataPoints) : deviation2;
        minDev = std::min( minDev, deviation1 );

		CommonUtil::Point3D otherPoint;
		double distance = 20;
		m_lineDirection.GetPointAlongVector(m_linePoint, otherPoint, distance);

		CommonUtil::Segment3D segment(m_linePoint, otherPoint);
		double snappingTol = CommonUtil::CommonConstants::PNT_TOL;

		for(int i = 0; i < numDataPoints; i++)
		{
			double uVal;
			segment.Project(dataPoints[i], snappingTol, uVal);
			segment.GetPoint(uVal, tmpPoint );
			double tmpDistSqr = tmpPoint.SqrDistance(dataPoints[i]);
			//if(fabs(tmpDist - deviation1) > 1e-2)
            if( IsSquareValueExceedDistanceBy( tmpDistSqr, deviation1, 1e-2 ) )
			{
				CommonUtil::Vector tmpVector(dataPoints[i], tmpPoint);
				CommonUtil::Point3D tmpDataPoint = dataPoints[i];
				if(tmpVector.IsValid())
				{
					tmpVector.GetPointAlongVector(dataPoints[i], tmpDataPoint, 
											  deviation1);
				}
				tmpDataPoints.push_back(tmpDataPoint);
			}

			else
			{
				tmpDataPoints.push_back(tmpPoint);
			}
		}

		Execute(tmpDataPoints);
		deviation2 = GetDeviation(dataPoints);
		ctr++;

        // Optimization, if the algo diverge, we make it stop earlier.
        if( ctr > 25 && deviation2 > 2.0*minDev )
        {
            break;
        }
	}while(fabs(deviation1 - deviation2) > 1e-5 && ctr < 200);	
}

//-----------------------------------------------------------------------------

}

