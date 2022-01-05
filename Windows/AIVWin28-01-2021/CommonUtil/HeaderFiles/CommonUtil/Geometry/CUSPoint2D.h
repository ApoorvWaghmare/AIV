#ifndef _COMMONUTIL_SPOINT2D_H_
#define _COMMONUTIL_SPOINT2D_H_

// This struct represents 2D point
struct SPoint2D
{
    double m_x;
    double m_y;

	SPoint2D() : m_x( 0.0 ), m_y( 0.0 ) {}

    SPoint2D( double x_, double y_ ) : m_x( x_ ), m_y( y_ ) {}

    template < typename TPtType >
    explicit SPoint2D( const TPtType& pt_ ) : m_x( pt_[ 0 ] ), m_y( pt_[ 1 ] ) {}

    template < typename TPtType >
    SPoint2D& operator=( const TPtType& pt_ )
    {
		m_x = pt_[ 0 ];
		m_y = pt_[ 1 ];
		
        return *this;
    }

    bool operator==( const SPoint2D& pt_ )
    {
        return ( m_x == pt_.m_x ) && ( m_y == pt_.m_y );
    }

};

#endif