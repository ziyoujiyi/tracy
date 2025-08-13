#ifndef __TRACYPRINT_HPP__
#define __TRACYPRINT_HPP__

#ifdef TRACY_VERBOSE
#    include <stdio.h>
#    define TracyDebug( ... ) fprintf( stderr, __VA_ARGS__ );
#    define MyDebug( format, ... )                                                                                     \
        printf( "FUNC: %s | FILE: %s, LINE: %d: " format "\n", __func__, __FILE__, __LINE__, ##__VA_ARGS__ )
#else
#    define TracyDebug( ... )
#    define MyDebug( ... )
#endif

#endif
