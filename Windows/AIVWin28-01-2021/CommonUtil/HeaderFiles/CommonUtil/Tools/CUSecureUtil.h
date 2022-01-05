
#ifndef _ESSENCE_SECUREUTIL_H_
#define _ESSENCE_SECUREUTIL_H_

#ifndef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES

#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1

#endif

#ifndef WINVER
#define WINVER 0x601
#endif

#ifndef _WINDOWS_
#include <afxver_.h>
#endif //_WINDOWS

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <io.h>
#include <xutility>
#include <algorithm>

#if _MFC_VER > 0x0600
	#define s_strlwr _strlwr_s
	#define s_fwprintf fwprintf_s
	#define s_sprintf sprintf_s
	#define s_sscanf sscanf_s
	#define s_strcpy strcpy_s
#else
	#define s_fwprintf fwprintf
	#define sprintf_s sprintf
	#define s_strlwr _strlwr
	#define strcpy_s strcpy
	#define s_sprintf sprintf_s
	#define s_sscanf sscanf_s
	#define s_strcpy strcpy_s
#endif //_MFC_VER

#pragma warning(disable:4786)


static FILE *s_fopen(const char *fname, const char *flags)
{
	FILE *fp = NULL;
#ifndef CPPCHECK_IGNORE_SECTION
	#if _MFC_VER > 0x0600
		fopen_s(&fp, fname, flags);
	#else
		fp = fopen(fname, flags);
	#endif
#endif
	return fp;
}

static void s_fprintf(FILE* fp, const char *fname, const char *flags)
{
	#if _MFC_VER > 0x0600
		fprintf_s(fp, fname, flags);
	#else
		fprintf(fp, fname, flags);
	#endif
}

static void s_fopen(FILE* &fp, const char *fname, const char *flags)
{
	fp = NULL;
#ifndef CPPCHECK_IGNORE_SECTION
	#if _MFC_VER > 0x0600
		fopen_s(&fp, fname, flags);
	#else
		fp = fopen(fname, flags);
	#endif
#endif
}

static FILE *s_wfopen(const wchar_t *fname, const wchar_t *flags)
{
	FILE *fp = NULL;
#ifndef CPPCHECK_IGNORE_SECTION
	#if _MFC_VER > 0x0600
		_wfopen_s(&fp, fname, flags);
	#else
		fp = _wfopen(fname, flags);
	#endif
#endif
	return fp;
}


static size_t s_mbstowcs(wchar_t (&wcstr)[1024], const char *outputFileName,
				 size_t count)
{
	size_t   i;
	#if _MFC_VER > 0x0600
		mbstowcs_s( &i, wcstr, outputFileName, count );
	#else
		i = mbstowcs(wcstr, outputFileName, count);
	#endif
	return i;
}

static size_t s_vswprintf(wchar_t *buffer, size_t sz, const wchar_t *format, ... )
{
	va_list argptr;
	va_start(argptr, format);
	#if _MFC_VER > 0x0600
		vswprintf_s( buffer, sz, format, argptr );
	#else
		sz = vswprintf(buffer, format, argptr);
	#endif

	va_end(argptr);
	return sz;
}


static void s_itow(int value, wchar_t *buffer, size_t sizeInCharacters,int radix)
{
	#if  _MFC_VER > 0x0600
		_itow_s(value, buffer, sizeInCharacters, radix);
	#else
		_itow(value, buffer, radix);
	#endif
}

static size_t s_wcstombs(const wchar_t *wcstr, char *str, size_t count)
{
	size_t   i;
	#if _MFC_VER > 0x0600
		wcstombs_s( &i, str, count, wcstr, count );
	#else
		i = wcstombs(str, wcstr, count);
	#endif
	return i;
}

static int s_sopen(const char* filename, int oflag, int shflag, int pmode)
{
	int   i;
	#if _MFC_VER > 0x0600
		_sopen_s( &i, filename, oflag, shflag, pmode);
	#else
		i = _sopen(filename, oflag, shflag, pmode);
	#endif
	return i;
}

static int s_wsopen(const wchar_t* filename, int oflag, int shflag, int pmode)
{
	int   i;
	#if _MFC_VER > 0x0600
		_wsopen_s( &i, filename, oflag, shflag, pmode);
	#else
		i = _wsopen(filename, oflag, shflag, pmode);
	#endif
	return i;
}

static size_t s_wcscpy(wchar_t* strDestination, const wchar_t* strSource,
					   size_t numWords = 1024)
{
	size_t   i=0;
	#if _MFC_VER > 0x0600
		wcscpy_s( strDestination, numWords, strSource );
	#else
		wcscpy(strDestination, strSource);
	#endif
	return i;
}

/*
static int s_swprintf(wchar_t *buffer, size_t sz, const wchar_t *format)
{
	#if _MFC_VER > 0x0600
		swprintf_s(buffer, sz, format);
	#else
		sz = swprintf(buffer, format);
	#endif
	return sz;
}
*/

static size_t f_write( const void* buffer_, size_t size_, size_t count_, FILE* stream_ )
{
    if ( size_ == 0 )
    {
        return 0;
    }
    size_t totalBytesToWrite = size_ * count_;
    size_t totalBytesWritten = 0;

#ifndef CPPCHECK_IGNORE_SECTION
    while ( totalBytesToWrite > 0 )
    {
        // We write in chunk of 2 GB.
        size_t bytesToWrite = std::min< size_t >( INT_MAX, totalBytesToWrite );
        size_t bytesWritten = fwrite( static_cast< const BYTE* >( buffer_ ) + totalBytesWritten, 1, bytesToWrite, stream_ );
        totalBytesWritten += bytesWritten;
        if ( bytesWritten != bytesToWrite )
        {
            break;
        }
        totalBytesToWrite -= bytesWritten;
    }
#endif
    return totalBytesWritten / size_;
}

#endif
