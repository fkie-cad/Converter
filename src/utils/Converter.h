#ifndef CONVERTER_H
#define CONVERTER_H

#include <stdint.h>



int parsePlainBytes(
    _In_ const char* Raw, 
    _Inout_ uint8_t** Buffer, 
    _Inout_ uint32_t* Size, 
    _In_ uint32_t MaxBytes
);
int parseUint8(const char* arg, uint8_t* value, uint8_t base);
int parseUint64(const char* arg, uint64_t* value, uint8_t base);



#define IS_NUM_CHAR(__char__) \
    ( __char__ >= '0' && __char__ <= '9' )

#define IS_LC_HEX_CHAR(__char__) \
    ( __char__ >= 'a' && __char__ <= 'f' )

#define IS_UC_HEX_CHAR(__char__) \
    ( __char__ >= 'A' && __char__ <= 'F' )

#define IN_HEX_RANGE(__char__) \
    ( IS_NUM(__char__) || ( __char__ >= 'a' && __char__ <= 'f' )  || ( __char__ >= 'A' && __char__ <= 'F' ) )

/**
 * Parse plain byte string into byte array
 */
int parsePlainBytes(
    _In_ const char* Raw, 
    _Inout_ uint8_t** Buffer, 
    _Inout_ uint32_t* Size, 
    _In_ uint32_t MaxBytes
)
{
    uint32_t i, j;
    size_t raw_size = strlen(Raw);
    uint8_t* p = NULL;
    int malloced = 0;
    uint32_t buffer_size;
    int s = 0;

    uint8_t m1, m2;

    if ( raw_size > MaxBytes * 2ULL )
    {
        printf("Data too big!\n");
        return ERROR_BUFFER_OVERFLOW;
    }

    if ( raw_size == 0 )
    {
        printf("Buffer is empty!\n");
        return ERROR_INVALID_PARAMETER;
    }
    if ( raw_size % 2 != 0 )
    {
        printf("Buffer data is not byte aligned!\n");
        return ERROR_INVALID_PARAMETER;
    }
    
    buffer_size = (uint32_t) (raw_size / 2);

    if ( *Size && *Buffer && buffer_size > *Size )
    {
        printf("Provided buffer is too small: 0x%x < 0x%x!\n", *Size, buffer_size);
        return ERROR_INVALID_PARAMETER;
    }

    if ( *Buffer == NULL )
    {
        p = (uint8_t*) malloc(buffer_size);
        if ( p == NULL )
        {
            printf("No memory!\n");
            return ERROR_NOT_ENOUGH_MEMORY;
        }
        malloced = 1;
    }
    else
    {
        p = *Buffer;
    }

    for ( i = 0, j = 0; i < raw_size; i += 2, j++ )
    {
        //if ( !IN_HEX_RANGE(Raw[i]) || !IN_HEX_RANGE(Raw[i+1]) )
        //{
        //    s = ERROR_INVALID_PARAMETER;
        //    printf("Error: Byte string not in hex range!\n");
        //    break;
        //}

        if ( IS_NUM_CHAR(Raw[i]) )
            m1 = 0x30;
        else if ( IS_UC_HEX_CHAR(Raw[i]) )
            m1 = 0x37;
        else if ( IS_LC_HEX_CHAR(Raw[i]) )
            m1 = 0x57;
        else
        {
            s = ERROR_INVALID_PARAMETER;
            printf("Byte string not in hex range!\n");
            break;
        }
        
        if ( IS_NUM_CHAR(Raw[i+1]) )
            m2 = 0x30;
        else if ( IS_UC_HEX_CHAR(Raw[i+1]) )
            m2 = 0x37;
        else if ( IS_LC_HEX_CHAR(Raw[i+1]) )
            m2 = 0x57;
        else
        {
            s = ERROR_INVALID_PARAMETER;
            printf("Byte string not in hex range!\n");
            break;
        }

        p[j] = ((Raw[i] - m1)<<4) | ((Raw[i+1] - m2) & 0x0F);
    }
    
    if ( s != 0 )
    {
        if ( malloced && p )
            free(p);
    }
    else
    {
        *Size = buffer_size;
        *Buffer = p;
    }

    return s;
}

int parseUint8(const char* arg, uint8_t* value, uint8_t base)
{
    uint64_t result;
    int s = parseUint64(arg, &result, base);
    if ( s != 0 ) return s;
    if ( s > (uint8_t)-1 )
    {
        printf("Error: %s could not be converted to a byte: Out of range!\n", arg);
        return 5;
    }

    *value = (uint8_t) result;
    return 0;
}

int parseUint64(const char* arg, uint64_t* value, uint8_t base)
{
    char* endptr;
    int err_no = 0;
    errno = 0;
    uint64_t result;

    if ( base != 10 && base != 16 && base != 0 )
    {
        printf("Error: Unsupported base %u!\n", base);
        return 1;
    }

    if ( arg[0] ==  '-' )
    {
        printf("Error: %s could not be converted to a number: is negative!\n", arg);
        return 2;
    }

#if defined(_WIN32)
    result = strtoull(arg, &endptr, base);
#else
    result = strtoul(arg, &endptr, base);
#endif
    err_no = errno;

    if ( endptr == arg )
    {
        printf("Error: %s could not be converted to a number: Not a number!\n", arg);
        return 3;
    }
    if ( result == (uint64_t)-1 && err_no == ERANGE )
    {
        printf("Error: %s could not be converted to a number: Out of range!\n", arg);
        return 4;
    }

    *value = result;
    return 0;
}

#endif
