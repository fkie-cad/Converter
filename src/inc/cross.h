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

#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__APPLE__)
    #define strtok_s strtok_r
    #define strtok_s strtok_r
    #define strcpy_s(dst, size, src) strcpy(dst, src)
    #define swprintf_s swprintf
    // #define swprintf_s(dst, size, src) swprintf(dst, src)
#endif

#ifndef _WIN32
    #define _In_
    #define _Out_
    #define _Inout_
#endif