#pragma once




#ifdef DEBUG_PRINT
#define DPrint(...) { printf(__VA_ARGS__);}
#define FPrint() { printf("%s\n", __FUNCTION__);}
#define DPrintMemCol8(_b_, _s_) \
{ \
    for ( size_t _i_ = 0; _i_ < (size_t)_s_; _i_+=0x10 ) \
    { \
        size_t _end_ = (_i_+0x10<(size_t)_s_)?(_i_+0x10):((size_t)_s_); \
        uint32_t _gap_ = (_i_+0x10<=_s_) ? 0 : (uint32_t)((0x10+_i_-(size_t)_s_)*3); \
        printf("%p  ", (((uint8_t*)_b_)+_i_)); \
         \
        for ( size_t _j_ = _i_, _k_=0; _j_ < _end_; _j_++, _k_++ ) \
        { \
            printf("%02x", ((uint8_t*)_b_)[_j_]); \
            printf("%c", (_k_==7?'-':' ')); \
        } \
        for ( uint32_t _j_ = 0; _j_ < _gap_; _j_++ ) \
        { \
            printf(" "); \
        } \
        printf("  "); \
        for ( size_t _j_ = _i_; _j_ < _end_; _j_++ ) \
        { \
            if ( ((uint8_t*)_b_)[_j_] < 0x20 || ((uint8_t*)_b_)[_j_] > 0x7E || ((uint8_t*)_b_)[_j_] == 0x25 ) \
            { \
                printf("."); \
            }  \
            else \
            { \
                printf("%c", ((uint8_t*)_b_)[_j_]); \
            } \
        } \
        printf("\n"); \
    } \
}

#define DPrint_C(__value__, __prefix__) \
     printf("%s%s: %c\n", __prefix__, #__value__, (char)__value__);

#define DPrint_D(__value__, __prefix__) \
     printf("%s%s: %llu\n", __prefix__, #__value__, (size_t)__value__);

#define DPrint_H(__value__, __prefix__) \
     printf("%s%s: 0x%llx\n", __prefix__, #__value__, (size_t)__value__);

#define DPrint_HD(__value__, __prefix__) \
     printf("%s%s: 0x%llx (%llu)\n", __prefix__, #__value__, (size_t)__value__, (size_t)__value__);

#define DPRINT_PTR(__value__, __prefix__) \
     printf("%s%s: %p\n", __prefix__, #__value__, (PVOID)__value__);

#define DPRINT_A(__value__, __prefix__) \
     printf("%s%s: %s\n", __prefix__, #__value__, (PCHAR)__value__);
#else
#define DPrint(...)
#define DPrintMemCol8(_b_, _s_)
#define FPrint(...)
#define DPrint_D(...)
#define DPrint_H(...)
#define DPrint_HD(...)
#define DPrint_PTR(...)
#define DPrint_A(...)
#define DPrint_Ax(...)
#define DPrint_C(...)
#endif

#ifdef ERROR_PRINT
#define EPrint(...) \
                {printf("[e] ");\
                 printf(__VA_ARGS__);}
#else
#define EPrint(...)
#endif

#define PRINT_INT_D(__value__, __prefix__) \
    printf("%s%s: %llu\n", __prefix__, #__value__, (unsigned __int64)__value__);

#define PRINT_INT_H(__value__, __prefix__) \
    printf("%s%s: 0x%llx\n", __prefix__, #__value__, (unsigned __int64)__value__);

#define PRINT_INT_HD(__value__, __prefix__) \
    printf("%s%s: 0x%llx (%llu)\n", __prefix__, #__value__, (unsigned __int64)__value__, (unsigned __int64)__value__);


#define HEX_CHAR_WIDTH(__hcw_v__, __hcw_w__) \
{ \
    uint8_t _hcw_w_ = 0x10; \
    for ( uint8_t _i_ = 0x38; _i_ > 0; _i_-=8 ) \
    { \
        if ( ! ((uint8_t)(__hcw_v__ >> _i_)) ) \
            _hcw_w_ -= 2; \
        else \
            break; \
    } \
    __hcw_w__ = _hcw_w_; \
}
