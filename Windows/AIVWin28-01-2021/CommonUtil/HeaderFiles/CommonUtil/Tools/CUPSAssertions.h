#ifndef	_PSASSERTIONS_H_
#define	_PSASSERTIONS_H_

#ifndef PS_ENABLE_ASSERT

#define PS_ASSERT0()     ((void)0)
#define PS_VERIFY(expr)  (expr)

#else // #ifndef PS_ENABLE_ASSERT

int  PSDoAssert( const char*, int, const char* );
void PSDoDebugBreak();

inline bool verify___condition( bool res_, const char* exprTxt_, const char* file_, int line_ )
{
    if( !res_ && PSDoAssert( file_, line_, exprTxt_ ) == 1 )
    {
        PSDoDebugBreak();
    }
    return res_;
}

#define PS_ASSERT0()                                \
    if ( PSDoAssert( __FILE__, __LINE__, 0 ) == 1 ) \
    {                                               \
        PSDoDebugBreak();                           \
    }                                               \
    ((void)0)

#define PS_VERIFY( expr ) verify___condition( expr, #expr, __FILE__, __LINE__ )

#endif	// #ifndef PS_ENABLE_ASSERT

#define PS_IF_FAILED_ACTION( F, A ) if( !PS_VERIFY( F ) ) { A }

#endif // _PSASSERTIONS_H_
