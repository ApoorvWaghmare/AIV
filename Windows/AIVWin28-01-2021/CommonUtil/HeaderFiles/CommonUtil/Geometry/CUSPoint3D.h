#ifndef _COMMONUTIL_SPOINT3D_H_
#define _COMMONUTIL_SPOINT3D_H_

#include <cmath>

// This struct represents 3D point
struct SPoint3D
{
    double m_x;
    double m_y;
    double m_z;

	SPoint3D() : m_x( 0.0 ), m_y( 0.0 ), m_z( 0.0 ) {}

    SPoint3D( double x_, double y_, double z_ ) : m_x( x_ ), m_y( y_ ), m_z( z_ ) {}

    template < typename TPtType >
    explicit SPoint3D( const TPtType& pt_ ) : m_x( pt_[ 0 ] ), m_y( pt_[ 1 ] ), m_z( pt_[ 2 ] ) {}

    template < typename TPtType >
    SPoint3D& operator=( const TPtType& pt_ )
    {
		m_x = pt_[ 0 ];
		m_y = pt_[ 1 ];
		m_z = pt_[ 2 ];
		
        return *this;
    }

    bool operator==( const SPoint3D& pt_ )
    {
        return ( m_x == pt_.m_x ) && ( m_y == pt_.m_y ) && ( m_z == pt_.m_z );
    }

	double Distance(const SPoint3D& pt)const
	{
		return sqrt((m_x - pt.m_x) * (m_x - pt.m_x) +
			(m_y - pt.m_y) * (m_y - pt.m_y) +
			(m_z - pt.m_z) * (m_z - pt.m_z));
	}
};

#endif