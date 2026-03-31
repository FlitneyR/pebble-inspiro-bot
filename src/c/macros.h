#pragma once

// crash trap
char* const __s_alwaysNull;
#define TRAP (*__s_alwaysNull = 0)

// no trap
//#define TRAP

#define GUARD( cond, ... ) \
    if ( !( cond ) ) { \
        APP_LOG( APP_LOG_LEVEL_ERROR, "!! " #cond ); \
        TRAP; \
        __VA_ARGS__; \
    }

#define CHECKED_FREE( func, ptr ) if ( ptr ) { func( ptr ); ptr = NULL; }
