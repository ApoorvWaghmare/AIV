#pragma once
// =================================================================================================
// API configuration for the MKLWrapper dynamic library.
//
//
// -------------------------------------------------------------------------------------------------

#if defined BUILD_MKLWRAPPER_DLL 
	// Makes it possible to export the symbols from the DLL
	#define MKLWRAPPER_API __declspec( dllexport )
#elif defined USE_MKLWRAPPER_DLL
	// Makes it possible to import the symbols from the DLL
	#define MKLWRAPPER_API __declspec( dllimport )
#else
	// Makes it possible to use the library and the class in a static library
	#define MKLWRAPPER_API 
#endif 


// class 'xxx' needs to have dll-interface to be used by clients of class 'yyy'
//#   pragma warning( disable: 4251 )



