#ifndef _COMMONUTIL_STRINGUTIL_H_
#define _COMMONUTIL_STRINGUTIL_H_

#include <algorithm>
#include <string>

#pragma warning( disable : 4505 )   // unreferenced local function has been removed

namespace CommonUtil
{

// wstring to string conversion methods
static const char c_WIDGET = '_';

static inline char ToChar( wchar_t inChar_ )
{
    return ( inChar_ > 0x00FF ? c_WIDGET : static_cast< char >( inChar_ ) );
}

static std::string WStringToString( const std::wstring& inStr_ )
{
    std::string outStr;
    outStr.resize( inStr_.length() );
    std::transform( inStr_.begin(), inStr_.end(), outStr.begin(), ToChar );

    return outStr;
}

static inline wchar_t ToWChar( char inChar_ )
{
    //return ( inChar_ > 0x00FF ? c_WIDGET : static_cast< char >( inChar_ ) );
	return static_cast< wchar_t >( inChar_ );
}

static std::wstring StringToWString( const std::string& inStr_ )
{
    std::wstring outStr;
    outStr.resize( inStr_.length() );
    std::transform( inStr_.begin(), inStr_.end(), outStr.begin(), ToWChar );

    return outStr;
}

static std::string IntegerToString( int intVal )
{
    char buffer [1024];
	itoa(intVal, buffer, 10);
	std::string outStr(buffer);

    return outStr;
}


};

#endif