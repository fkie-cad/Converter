#ifndef _ERRORS_H
#define _ERRORS_H

#if defined(__linux__) || defined(__linux) || defined(linux) || defined(__APPLE__)
#include <errno.h>
#define ERROR_NOT_ENOUGH_MEMORY (ENOMEM)
#define ERROR_INVALID_PARAMETER (-1)
#define ERROR_BUFFER_OVERFLOW (-1)
#elif defined(_WIN32)
#include <winerror.h>
//#define ERROR_NOT_ENOUGH_MEMORY (8L)
//#define ERROR_INVALID_PARAMETER (87L)
#endif

#endif
