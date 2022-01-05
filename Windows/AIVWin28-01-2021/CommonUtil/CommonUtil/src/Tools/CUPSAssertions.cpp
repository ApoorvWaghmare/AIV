#include "CUPSAssertions.h"

#include <windows.h>
#include <map>
#include <sstream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "CUSecureUtil.h"

static long _PSAssertCount = -1;

static int MessageWindow( const char* srcFileName_, int srcFileLine_, const char* exprText_ );
static std::string ModuleNameGet();

const HKEY           MAIN_HKEY    = HKEY_LOCAL_MACHINE;
const wchar_t* const MAIN_SUB_KEY = L"Software\\InnovMetric\\Dev\\RunTimeDebugging\\PrescientAssertFile";

// =================================================================================================
//   Causes a break in the debugger
// -------------------------------------------------------------------------------------------------
void PSDoDebugBreak()
{
    DebugBreak();
}

// =================================================================================================
//   Handles an assertion
//
// Paramètre: srcFileName_ : Name of the source file where the assertion occured
// Paramètre: srcFileLine_ : Line number of the assertion
// Paramètre: exprText_    : Expression that failed
//
// Retour: 0 if the execution should continue, 1 to cause a break in the debugger
//
// Auteur(e): Charles Valiquette, Date: 07 mars 2012
// -------------------------------------------------------------------------------------------------
int PSDoAssert( const char* srcFileName_, int srcFileLine_, const char* exprText_ )
{
    int retval = 0;

    if( InterlockedIncrement( &_PSAssertCount ) > 0 )
    {
        // Handle the (hopefully rare) case of
        //
        // 1) ASSERT while already dealing with an ASSERT
        //      or
        // 2) two threads asserting at the same time
        std::stringstream secondChanceMsg;

        secondChanceMsg << "Second Chance Assertion: File " << srcFileName_ << ", Line " << srcFileLine_ << std::endl;

        OutputDebugStringA( secondChanceMsg.str().c_str() );

        InterlockedDecrement( &_PSAssertCount );

        // We break here to allow debugging of the second assertion
        PSDoDebugBreak();

        return 0;
    }

    std::string assertMsg = ( exprText_ != NULL ) ? "Prescient assertion: " : "Prescient assertion!";
    
    if ( exprText_ != NULL )
    {
        assertMsg += exprText_;
    }

    if( srcFileName_[ 0 ] != L'\0' )
    {
        std::stringstream detailedMsg;

        detailedMsg << ModuleNameGet() << ": " << srcFileName_ << "(" << srcFileLine_ << ") : " << assertMsg;

        assertMsg = detailedMsg.str();
    }

    // We output the message in the debugger
    OutputDebugStringA( ( assertMsg + "\n" ).c_str() );

    bool loggedInFile = false;

    // We display an assertion window if there is no testing being performed
	const char* assertionFilename = getenv( "PS_ASSERTION_FILE" );

    // Assertion file from environment variable
    if( assertionFilename != NULL )
	{
        loggedInFile = true;
        // Append assertion message to specified file
        FILE* pPSAssertFile = s_fopen( assertionFilename, "a+b" );

        if ( pPSAssertFile != NULL )
        {
            assertMsg += "\r\n";

            f_write( assertMsg.c_str(), sizeof( char ), assertMsg.size(), pPSAssertFile );

            fclose( pPSAssertFile );
        }
    }

    // Assertion file for testing report
    HKEY hKey;
    if ( RegOpenKeyExW( MAIN_HKEY, MAIN_SUB_KEY, 0, KEY_READ | KEY_WOW64_32KEY, &hKey ) != ERROR_SUCCESS  )
    {
        loggedInFile = true;
        wchar_t modFilename[ _MAX_PATH ];
        const DWORD len = GetModuleFileNameW( NULL, modFilename, _MAX_PATH );
        modFilename[ len - 3 ] = L'a';
        modFilename[ len - 2 ] = L's';
        modFilename[ len - 1 ] = L't';

        // Append assertion message to specified file
        bool exist = ! ( INVALID_FILE_ATTRIBUTES == GetFileAttributesW( modFilename ) && GetLastError() == ERROR_FILE_NOT_FOUND );
        FILE* pTestingAssertFile = s_wfopen( modFilename, exist ? L"ab" : L"wb" );

        if( pTestingAssertFile != NULL && ! exist )
        {
            // writing BOM for this new file.
            f_write( L"\xFEFF", sizeof( wchar_t ), 1, pTestingAssertFile );
        }

        if ( pTestingAssertFile != NULL )
        {
            assertMsg += "\r\n";

            std::wstring wideStringAssertMsg( assertMsg.begin(), assertMsg.end() );
            f_write( wideStringAssertMsg.c_str(), sizeof( wchar_t ), wideStringAssertMsg.size(), pTestingAssertFile );

            fclose( pTestingAssertFile );
        }
    }
    RegCloseKey( hKey );

    if( loggedInFile == false )
    {
        // Assertions are shown on a window
        retval = MessageWindow( srcFileName_, srcFileLine_, exprText_ );
    }

    InterlockedDecrement( &_PSAssertCount );

    return retval;
}

// =================================================================================================
//   Display a dialog box in case of assertion
//
// Parameter: szFile_        : File name
// Parameter: szLine_        : Line number
// Parameter: szUserMessage_ : User message
//
// Returns: 0 if the assertion should be ignored, 1 if the debugger should break.
// -------------------------------------------------------------------------------------------------
static int MessageWindow( const char* srcFileName_, int srcFileLine_, const char* exprText_ )
{
    std::stringstream assertMsgStr;

    assertMsgStr << "Debug Assertion!"             << std::endl;
    assertMsgStr                                   << std::endl;
    assertMsgStr << "Program: " << ModuleNameGet() << std::endl;
    assertMsgStr                                   << std::endl;

    if ( srcFileName_ != NULL )
    {
        assertMsgStr << "File: " << srcFileName_ << std::endl;
        assertMsgStr << "Line: " << srcFileLine_ << std::endl;
    }

    if ( exprText_ != NULL )
    {
        assertMsgStr                                << std::endl;
        assertMsgStr << "Expression: " << exprText_ << std::endl;
    }

    assertMsgStr                                                     << std::endl;
    assertMsgStr << "Prescient Assertion"                            << std::endl;
    assertMsgStr << "- Click Retry to debug"                         << std::endl;
    assertMsgStr << "- Shift+Ignore to always ignore this assertion" << std::endl;

    // We check if the assertion should be ignored
    std::string                             assertMsg = assertMsgStr.str();
    static std::map< std::string, bool >    ignoreAllPerAssertion;
    std::map< std::string, bool >::iterator assertInIgnoreAll = ignoreAllPerAssertion.find( assertMsg );

    if( assertInIgnoreAll != ignoreAllPerAssertion.end() && assertInIgnoreAll->second )
    {
        return 0;
    }

    // Report the warning/error
    HWND hWndParent = GetActiveWindow();

    if( hWndParent != NULL )
    {
        hWndParent = GetLastActivePopup( hWndParent );
    }

    int nCode = MessageBoxA( hWndParent,
                             assertMsg.c_str(),
                             "Prescient Debug Library",
                             MB_TASKMODAL | MB_ICONHAND | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND | MB_DEFBUTTON2 );

    // Abort: abort the program
    if ( IDABORT == nCode )
    {
        // To prevent memory leaks reporting
        _CrtSetDbgFlag( 0 );

        // Raise abort signal
        raise( SIGABRT );

        // We usually won't get here, but it's possible that
        // SIGABRT was ignored. So exit the program anyway.
        exit( 3 );
    }

    // Retry: return 1 to call the debugger
    if ( IDRETRY == nCode )
    {
        return 1;
    }

    // If the Shift key is pressed, the developer want to always ignore this assertion.
    if( ( HIBYTE( ::GetKeyState( VK_SHIFT ) ) != 0 ) )
    {
        ignoreAllPerAssertion[ assertMsg ] = true;
    }

    // Ignore: continue execution
    return 0;
}

static std::string ModuleNameGet()
{
    char modPath[ _MAX_PATH + 1 ] = "";

    DWORD lenModPath = GetModuleFileNameA( NULL, modPath, _MAX_PATH );

    if( ( lenModPath == _MAX_PATH ) || ( lenModPath == 0 ) )
    {
        // Path too long or problem with 'GetModuleFileName'.
        return "<unknowm program>";
    }
    else
    {
        char* pModFilename = modPath + lenModPath;

        while( pModFilename > modPath &&
               *( pModFilename - 1 ) != '\\' &&
               *( pModFilename - 1 ) != '/' )
        {
            --pModFilename;
        }
		
		return pModFilename;
    }
}
