#ifndef _CU_SIZEOF_H_
#define _CU_SIZEOF_H_
#include<vector>
template< typename TType >
size_t SizeOfVector( const TType& vector_ )
{
    size_t size = 0;
    size += sizeof( TType::value_type ) * vector_.capacity();
    return size;
}

template< typename TType >
size_t SizeOfVectorEx( const TType& vector_ )
{
    size_t size = 0;
    for ( TType::const_iterator itr = vector_.begin(); itr != vector_.end(); ++itr )
    {   
        size += ( *itr ).SizeOf();
    }
	size_t valuetypesize = sizeof( TType::value_type );
    size += valuetypesize * ( vector_.capacity() - vector_.size() );
    return size;
}

template< typename TType >
size_t SizeOfPtrVectorEx( const TType& vector_ )
{
    size_t size = 0;
    for ( TType::const_iterator itr = vector_.begin(); itr != vector_.end(); ++itr )
    {   
        if( *itr != 0 )
        {
            size += ( *itr )->SizeOf();
        }
    }
	size_t valuetypesize = sizeof( TType::value_type );
    size += valuetypesize * vector_.capacity();
    return size;
}


#endif