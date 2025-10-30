#pragma once

#ifndef min
    #define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef max
    #define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef ALIGN_UP_BY
#define ALIGN_UP_BY(Address, Align) ( ((uint64_t)(Address) + (Align) - 1) & ~((Align) - 1) )
#endif

#if defined(linux) || defined (__linux)
    #define strtok_s strtok_r
#endif
