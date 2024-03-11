#include "inc/errors.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Args.h"
#include "print.h"
#include "utils/Converter.h"

#define FORMAT_NONE    (0x0)
#define FORMAT_INT8    (0x1)
#define FORMAT_INT16   (0x2)
#define FORMAT_INT32   (0x4)
#define FORMAT_INT64   (0x8)

#define FORMAT_ALL   (FORMAT_INT8|FORMAT_INT16|FORMAT_INT32|FORMAT_INT64)


//#define FLAG_VERBOSE    (0x01)
#define MODE_NONE       (0x00) // 000
#define MODE_NUMBER_A   (0x01) // 001
#define MODE_NUMBER_U   (0x02) // 010
#define MODE_STRING_A   (0x03) // 011
#define MODE_STRING_U   (0x04) // 100


typedef struct _BUFFER {
    uint8_t* Bytes;
    uint32_t Size;
} BUFFER, *PBUFFER;

typedef struct _FLAGS {
    uint32_t Verbose:1;
    uint32_t Mode:3;
    uint32_t Reserved:28;
} FLAGS, *PFLAGS;

void printBlocks(const uint8_t* string, size_t n, uint32_t block_size);
void printIntBlocks(const uint8_t* bytes, size_t n, uint32_t block_size);

void printUsage();
void printHelp();


static int parseParams(
    int argc, 
    char** argv, 
    PBUFFER Data, 
    uint32_t* format, 
    PFLAGS flags, 
    int start_i
);

static int checkParams(
    PBUFFER Data, 
    uint32_t format,
    PFLAGS flags
);


int main(int argc, char** argv)
{
    if ( argc < 2 )
    {
        printUsage();
        return -1;
    }
    
    if ( isAskForHelp(argc, argv) )
    {
        printHelp();
        return 0;
    }
    
    BUFFER data = {0};
    FLAGS flags = { 0 };
    uint32_t format = FORMAT_INT8;
    size_t i;
    
    
    int s = parseParams(argc, argv, &data, &format, &flags, 1);
    if ( s != 0 )
    {
        printUsage();
        goto clean;
    }

    s = checkParams(&data, format, &flags);
    if ( s != 0 )
    {
        printUsage();
        goto clean;
    }
    
    switch ( flags.Mode ) 
    {
        case MODE_STRING_A:
        {
            uint8_t* string = (uint8_t*)data.Bytes;
            size_t n = data.Size - 1;
            printf("string: %s\n", (char*)string);
    
            if ( format & FORMAT_INT8 )
            {
                printf("FORMAT_INT8\n");
                printf("hex bytes:\n");
                for ( i = 0; i < n; i++ )
                {
                    printf("%02x ", string[i]);
                }
                printf("\n\n");
            }
            if ( format & FORMAT_INT16 )
            {
                printf("FORMAT_INT16\n");
                printBlocks(string, n, 2);
                printf("\n");
            }
            if ( format & FORMAT_INT32 )
            {
                printf("FORMAT_INT32\n");
                printBlocks(string, n, 4);
                printf("\n");
            }
            if ( format & FORMAT_INT64 )
            {
                printf("FORMAT_INT64\n");
                printBlocks(string, n, 8);
                printf("\n");
            }

            break;
        }
        case MODE_STRING_U:
        {
            uint8_t* string = (uint8_t*)data.Bytes;
            wchar_t* string_w = (wchar_t*)data.Bytes;
            size_t n = data.Size - 2;
            printf("string: %ws\n", string_w);
    
            if ( format & FORMAT_INT8 )
            {
                printf("FORMAT_INT8\n");
                printf("hex bytes:\n");
                for ( i = 0; i < n; i++ )
                {
                    printf("%02x ", string[i]);
                }
                printf("\n\n");
            }
            if ( format & FORMAT_INT16 )
            {
                printf("FORMAT_INT16\n");
                printBlocks(string, n, 2);
                printf("\n");
            }
            if ( format & FORMAT_INT32 )
            {
                printf("FORMAT_INT32\n");
                printBlocks(string, n, 4);
                printf("\n");
            }
            if ( format & FORMAT_INT64 )
            {
                printf("FORMAT_INT64\n");
                printBlocks(string, n, 8);
                printf("\n");
            }

            break;
        }
        case MODE_NUMBER_A:
        {
            size_t n = data.Size;

            DPrintMemCol8(data.Bytes, data.Size);
        
            //if ( format & FORMAT_INT8 )
            {
                //printf("FORMAT_INT8\n");
                printf("string:\n");
                printf("%.*s\n", (uint32_t)(n), &data.Bytes[0]);
                printf("chars:\n");
                for ( i = 0; i < n; i++ )
                {
                    printf("%c ", data.Bytes[i]);
                }
                printf("\n\n");
            }
            //if ( format & FORMAT_INT16 )
            //{
            //    printf("FORMAT_INT16\n");
            //    printIntBlocks(data.Bytes, n, 2);
            //    printf("\n");
            //}
            //if ( format & FORMAT_INT32 )
            //{
            //    printf("FORMAT_INT32\n");
            //    printIntBlocks(data.Bytes, n, 4);
            //    printf("\n");
            //}
            //if ( format & FORMAT_INT64 )
            //{
            //    printf("FORMAT_INT64\n");
            //    printIntBlocks(data.Bytes, n, 8);
            //    printf("\n");
            //}

            break;
        }
        case MODE_NUMBER_U:
        {
            size_t n = data.Size;
        
            //if ( format & FORMAT_INT8 )
            //{
            //    printf("FORMAT_INT8\n");
            //    printf("chars:\n");
            //    for ( i = 0; i < n; i++ )
            //    {
            //        printf("%c ", data.Bytes[i]);
            //    }
            //    printf("\n\n");
            //}
            //if ( format & FORMAT_INT16 )
            //{
                //printf("FORMAT_INT16\n");
                printf("string:\n");
                printf("%.*ws\n", (uint32_t)(n/2), (uint16_t*)&data.Bytes[0]);
                printf("wchars:\n");
                for ( i = 0; i < n; i+=2 )
                {
                    printf("%wc ", *(uint16_t*)&data.Bytes[i]);
                }
                printf("\n\n");
            //}
            //if ( format & FORMAT_INT32 )
            //{
            //    printf("FORMAT_INT32\n");
            //    printIntBlocks(data.Bytes, n, 4);
            //    printf("\n");
            //}
            //if ( format & FORMAT_INT64 )
            //{
            //    printf("FORMAT_INT64\n");
            //    printIntBlocks(data.Bytes, n, 8);
            //    printf("\n");
            //}

            break;
        }
        default:
            break;
    }

clean:
    if ( data.Bytes )
        free(data.Bytes);
    
    return 0;
}

void printBlocks(const uint8_t* string, size_t n, uint32_t block_size)
{
    size_t i;

    size_t j = 0;  
    uint32_t parts = (uint32_t)(n / block_size);
    uint32_t rest = (uint32_t)(n % block_size);
    uint32_t block_str_size = (uint32_t)(block_size * 2);
    
    if ( parts > 0 )
    {
        printf("parts:\n");
        if ( block_size == 8 )
        {
            for ( i = 0, j = 0; i < parts; i++, j+=block_size )
            {
                printf("%0*llx ", block_str_size, *(uint64_t*)&string[j]);
            }
        }
        else if ( block_size == 4 )
        {
            for ( i = 0, j = 0; i < parts; i++, j+=block_size )
            {
                printf("%0*x ", block_str_size, *(uint32_t*)&string[j]);
            }
        }
        else if ( block_size == 2 )
        {
            for ( i = 0, j = 0; i < parts; i++, j+=block_size )
            {
                printf("%0*x ", block_str_size, *(uint16_t*)&string[j]);
            }
        }
        printf("\n");
    }
    if ( rest > 0 )
    {
        printf("rest:\n");
        for ( i = j; i < n; i++ )
        {
            printf("%02x ", string[i]);
        }
        printf("\n");
    }
}

void printIntBlocks(const uint8_t* bytes, size_t n, uint32_t block_size)
{
    
    size_t i;

    size_t j = 0;  
    uint32_t parts = (uint32_t)(n / block_size);
    uint32_t rest = (uint32_t)(n % block_size);
    
    if ( parts > 0 )
    {
        printf("parts:\n");
        if ( block_size == 8 )
        {
            for ( i = 0, j = 0; i < parts; i++, j+=block_size )
            {
                //printf("%.*s ", block_size, (char*)(uint64_t*)&bytes[j]);
                printf("%c%c%c%c%c%c%c%c ", 
                    bytes[j+7], bytes[j+6], bytes[j+5], bytes[j+4], bytes[j+3], bytes[j+2], bytes[j+1], bytes[j]);
            }
        }
        else if ( block_size == 4 )
        {
            for ( i = 0, j = 0; i < parts; i++, j+=block_size )
            {
                //printf("%.*s ", block_size, (char*)(uint32_t*)&bytes[j]);
                printf("%c%c%c%c ", 
                    bytes[j+3], bytes[j+2], bytes[j+1], bytes[j]);
            }
        }
        else if ( block_size == 2 )
        {
            for ( i = 0, j = 0; i < parts; i++, j+=block_size )
            {
                //printf("%.*s ", block_size, (char*)(uint16_t*)&bytes[j]);
                printf("%c%c ", 
                    bytes[j+1], bytes[j]);
            }
        }
        printf("\n");
    }
    if ( rest > 0 )
    {
        printf("rest:\n");
        for ( i = j; i < n; i++ )
        {
            printf("%c ", bytes[i]);
        }
        printf("\n");
    }
}

void printUsage()
{
    printf("Usage: String2Hex "
        "(-a <string>|-u <string>|-na|-nu <value>) "
        "[-f <format>]\n");
}

void printHelp()
{
    printUsage();
    printf("\n");
    printf("Options:\n");
    printf("-a: Convert ascii string into hex.\n");
    printf("-u: Convert unicode (utf-16) string into hex.\n");
    printf("-na: Convert hex byte string into ascii string/chars. If preceded with '0x', it will be interpreted as a hex uint64 (i.e. mostly as little endian).\n");
    printf("-nu: Convert hex byte string into unicode string/chars. If preceded with '0x', it will be interpreted as a hex uint64 (i.e. mostly as little endian).\n");
    printf("-f: Number format of output. Used with the -a or -u option. 1: bytes, 2: uint16, 4: uint32, 8: uint64\n");
}

int parseParams(
    int argc, 
    char** argv, 
    PBUFFER Data, 
    uint32_t* format, 
    PFLAGS flags, 
    int start_i
)
{
    int i;
    char* arg;
    char* val;
    int s = 0;

    for ( i = start_i; i < argc; i++ )
    {
        arg = argv[i];
        val = ( i < argc - 1 ) ? argv[i+1] : NULL;

        if ( IS_1C_ARG(arg, 'f') )
        {
            BREAK_ON_NOT_A_VALUE(val, s, "[e] missing format value\n");

            *format = strtoul(val, NULL, 0);

            i++;
        }
        else if ( IS_2C_ARG(arg, 'na') )
        {
            if ( flags->Mode )
            {
                continue;
            }
            BREAK_ON_NOT_A_VALUE(val, s, "[e] missing data bytes\n");

            if ( val[0] == '0' && val[1] == 'x' )
            {
                Data->Size = 8;
                Data->Bytes = malloc(Data->Size);
                if ( !Data->Bytes )
                {
                    s = ERROR_NOT_ENOUGH_MEMORY;
                    break;
                }
                *(uint64_t*)(Data->Bytes) = strtoull(val, 0, 16);
            }
            else
            {
                Data->Size = 0;
                s = parsePlainBytes(val, &Data->Bytes, &Data->Size, 0xFFFF);
                if ( s != 0 )
                {
                    EPrint("parse data bytes failed\n");
                    break;
                }
            }
                
            flags->Mode = MODE_NUMBER_A;

            i++;
        }
        else if ( IS_2C_ARG(arg, 'nu') )
        {
            if ( flags->Mode )
            {
                continue;
            }
            BREAK_ON_NOT_A_VALUE(val, s, "[e] missing data bytes\n");

            if ( val[0] == '0' && val[1] == 'x' )
            {
                Data->Size = 8;
                Data->Bytes = malloc(Data->Size);
                if ( !Data->Bytes )
                {
                    s = ERROR_NOT_ENOUGH_MEMORY;
                    break;
                }
                *(uint64_t*)(Data->Bytes) = strtoull(val, 0, 16);
            }
            else
            {
                Data->Size = 0;
                s = parsePlainBytes(val, &Data->Bytes, &Data->Size, UINT16_MAX);
                if ( s != 0 )
                {
                    printf("[e] parse data bytes failed\n");
                    break;
                }
            }
                
            flags->Mode = MODE_NUMBER_U;

            i++;
        }
        else if ( IS_1C_ARG(arg, 'a') )
        {
            if ( flags->Mode )
            {
                continue;
            }
            BREAK_ON_NOT_A_VALUE(val, s, "[e] missing string\n");

            Data->Size = (uint32_t)strlen(val) + 1;
            Data->Bytes = (uint8_t*)malloc(Data->Size);
            if ( !Data->Bytes )
            {
                s = ERROR_INVALID_PARAMETER;
                break;
            }
            memcpy(Data->Bytes, val, Data->Size);
                
            flags->Mode = MODE_STRING_A;

            i++;
        }
        else if ( IS_1C_ARG(arg, 'u') )
        {
            if ( flags->Mode )
            {
                continue;
            }
            BREAK_ON_NOT_A_VALUE(val, s, "[e] missing string\n");

            Data->Size = (uint32_t)((strlen(val) + 1)*2);
            Data->Bytes = (uint8_t*)malloc(Data->Size);
            if ( !Data->Bytes )
            {
                Data->Size = 0;
                s = ERROR_NOT_ENOUGH_MEMORY;
                break;
            }
            swprintf_s((wchar_t*)Data->Bytes, Data->Size/2, L"%hs", val);
                
            flags->Mode = MODE_STRING_U;

            i++;
        }
        else if ( IS_1C_ARG(arg, 'v') )
        {
            flags->Verbose = 1;
        }
    }

    return s;
}

int checkParams(
    PBUFFER Data, 
    uint32_t format, 
    PFLAGS flags
)
{
    int s = 0;


    if ( !flags->Mode )
    {
        EPrint("No mode specified!\n");
        s = ERROR_INVALID_PARAMETER;
    }
    
    uint32_t f = format & (FORMAT_ALL);
    if ( !f )
    {
        EPrint("No valid format specified!\n");
        s = ERROR_INVALID_PARAMETER;
    }

    if ( Data->Bytes == NULL )
    {
        EPrint("No data passed!\n");
        s = ERROR_INVALID_PARAMETER;
    }
#ifdef ERROR_PRINT
    if ( s != 0 )
        printf("\n");
#endif

    return s;
}
