#ifndef _COMMONUTIL_ESSENCECONTAINER_H_
#define _COMMONUTIL_ESSENCECONTAINER_H_


#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

namespace CommonUtil
{
    // This class is intended to keep all the TType pointers in the same order they were added using Add().
    // You can access to that list using ListGet(). 
    // This class is useful when you have a lot of Add() and Remove(), but few call to ListGet(). 
    // It is also working great when the number of items added is large.
    // The use of a internal counter that is always increasing ( Remove() does not reallocate the indexes )
    // can be problematic in some cases. If you add and remove repeately one element very often, the counter can overflow.
    // In that case, the function Compact() shall be called.
    template< class TType >
    class PtrOrderedList
    {
        mutable std::vector<TType* > m_nonSortedList;
        std::map< TType*, size_t > m_map;
        mutable bool m_nonSortedListValid;
        mutable size_t m_nextIndex;

    public:

        //Constructor
        PtrOrderedList()
            :m_nonSortedListValid( false ), m_nextIndex( 0 )
        {
        }

        ~PtrOrderedList(){}

        //-----------------------------------------------------------------------

        void Clear() { m_map.clear(); m_nonSortedList.clear(); m_nextIndex = 0; m_nonSortedListValid = false; }

        //-----------------------------------------------------------------------

        void PtrOrderedList< TType >::Add(TType* item_)
        {
            auto iter = m_map.find( item_ );

            if(iter == m_map.end() ) 
            {
                m_map[ item_ ] = m_nextIndex++;
                m_nonSortedListValid = false;
            }
        }

        //-----------------------------------------------------------------------

        void PtrOrderedList< TType >::Remove(TType* item_)
        {
            m_map.erase( item_ );
            m_nonSortedListValid = false;
            m_nonSortedList.clear();
            if( m_map.empty() )
            {
                Clear();
            }
        }

        //-----------------------------------------------------------------------

        struct SPairFlipper
        {
        public:
            template< typename T1, typename T2 >
            std::pair< T2, T1 >
                operator()( const std::pair< const T1, T2 >& pair_ ) const
            {
                return std::pair< T2, T1 >( pair_.second, pair_.first );
            }
        };

        //-----------------------------------------------------------------------

        const std::vector<TType* >& PtrOrderedList< TType >::ListGet() const
        {
            if(m_nonSortedListValid)
            {
                return m_nonSortedList;
            }

            std::map< size_t, TType* > flippedMap;
            std::transform( m_map.begin(), m_map.end(),
                std::inserter( flippedMap, flippedMap.begin() ), SPairFlipper() );

            m_nonSortedList.clear();
            m_nonSortedList.resize( flippedMap.size() );

            size_t index = 0;
            // sorted on indexing
            for( auto iter = flippedMap.begin(); iter != flippedMap.end(); ++iter, ++index )
            {
                m_nonSortedList[ index ] = iter->second;
            }
            m_nonSortedListValid = true;
            return m_nonSortedList;
        }

        size_t PtrOrderedList< TType >::SizeGet() const
        {
            if( m_nonSortedListValid )
            {
                return m_nonSortedList.size();
            }
            else
            {
                return m_map.size();
            }
        }

        void Compact()
        {
            std::vector<TType* > listCopy = ListGet();
            Clear();
            for( auto iter = listCopy.begin(); iter != listCopy.end(); ++iter )
            {
                Add( *iter );
            }

        }

    };
}
#endif