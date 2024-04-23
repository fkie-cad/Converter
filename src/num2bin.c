#include "inc/errors.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
    #include <conio.h>
    #include <io.h>
    #include <windows.h>
#endif

#include "Args.h"
#include "print.h"
#include "utils/strings.h"


#define EXE_NAME "Num2Bin"
#define EXE_VS "1.0.4"
#define EXE_LC "22.04.2024"


#define WIDTH_INT8    (0x8)
#define WIDTH_INT16   (0x10)
#define WIDTH_INT32   (0x20)
#define WIDTH_INT64   (0x40)

#define MIN_BLOCK_SIZE (2)
#define DEF_BLOCK_SIZE (4)
#define MAX_BLOCK_SIZE (0x40)

#define FLAG_VERBOSE    (0x01)

#define DEFAULT_ALIGN (0x8)

#ifndef ALIGN_UP_BY
#define ALIGN_UP_BY(Address, Align) ( ((ULONG_PTR)(Address) + (Align) - 1) & ~((Align) - 1) )
#endif


typedef struct _CMD_PARAMS {
    char* BinaryValue;
    uint64_t Value;
    char* Highlight;
    uint8_t BlockSize;
    uint8_t Width;
    struct {
        uint32_t Verbose:1;
        uint32_t NumberSet:1;
        uint32_t BinarySet:1;
        uint32_t Reserved:29;
    } Flags;
} CMD_PARAMS, *PCMD_PARAMS;

int bin2num(
    PCMD_PARAMS Params,
    uint64_t* Value
);

int num2bin(
    PCMD_PARAMS Params
);

uint64_t getHighlightMask(
    char* Input,
    uint32_t* MaxHighlightBit
);

void printUsage();
void printHelp();


static int parseParams(
    int argc, 
    char** argv, 
    PCMD_PARAMS Params, 
    int start_i
);

static int checkParams(
    PCMD_PARAMS Params
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
    
    CMD_PARAMS params = { 0 };
    
    int s = parseParams(argc, argv, &params, 1);
    if ( s != 0 )
    {
        printUsage();
        goto clean;
    }

    s = checkParams(&params);
    if ( s != 0 )
    {
        printUsage();
        goto clean;
    }
    
    
    if ( params.Flags.BinarySet )
    {
        s = bin2num(&params, &params.Value);
        params.Flags.NumberSet = 1;
    }

    if ( params.Flags.NumberSet )
    {
        num2bin(&params);
    }
    
    
clean:
    
    return s;
}

int num2bin(
    PCMD_PARAMS Params
)
{
    int s = 0;

    printf("decimal: %llu\n", Params->Value);
    printf("hex: 0x%llx\n", Params->Value);
    printf("binary: ");

    uint32_t maxHighlightBit = 0;
    uint64_t highlights = getHighlightMask(Params->Highlight, &maxHighlightBit);
    uint8_t valueHCb = 0;
    HEX_CHAR_WIDTH(Params->Value, valueHCb);
    Params->Width = max(Params->Width, valueHCb*4);
    uint32_t end = max(Params->Width, maxHighlightBit+1);
    end = ALIGN_UP_BY(end, DEFAULT_ALIGN);
    
#if defined(_WIN32)
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;
#endif

    for ( int32_t i = end-1; i >= 0; i-- )
    {
#if defined(_WIN32)
        if ( !( (highlights >> i) & 1 ) )
            SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY);
#endif

        if ( ( (Params->Value >> i) & 1 ) )
            putchar('1');
        else
            putchar('0');

        if ( i && i % Params->BlockSize == 0 )
        {
            putchar(' ');
        }

#if defined(_WIN32)
        if ( !( (highlights >> i) & 1 ) )
            SetConsoleTextAttribute(hStdout, wOldColorAttrs);
#endif
    }

    return s;
}

int bin2num(
    PCMD_PARAMS Params,
    uint64_t* Value
)
{
    int s = 0;
    size_t cb = strlen(Params->BinaryValue);
    uint64_t value = 0;
    *Value = 0;

    FPrint();

    // rough check of binary string size
    if ( !cb || cb > 64 )
        return ERROR_INVALID_PARAMETER;
    
    // check values in binary string for 0 and 1
    for ( int i = 0; i < (int)cb; i++ )
    {
        if ( Params->BinaryValue[i] != '0' && Params->BinaryValue[i] != '1' )
        {
            s = ERROR_INVALID_PARAMETER;
            EPrint("Not a binary string number! (0x%x)\n", s);
            return s;
        }
    }

    // convert string to int
    for ( int i = 0; i < (int)cb; i++ )
    {
        if ( Params->BinaryValue[i] == '1' )
            value |= (uint64_t)1 << (cb - i - 1);
    }

    *Value = value;

    return s;
}

uint64_t getHighlightMask(
    char* Input,
    uint32_t* MaxHighlightBit
)
{
    if ( !Input )
        return (uint64_t)-1;

    char* ptr = Input;
    char* last = ptr;

    uint32_t val0 = (uint32_t)-1;
    uint32_t val1 = (uint32_t)-1;
    uint32_t temp;
    uint32_t* value = &val0;
    uint32_t mode = 0;
    uint32_t maxHb = 0;

    uint64_t highlights = 0;
    *MaxHighlightBit = 0;

    while ( ptr && *ptr != 0 )
    {
        if ( *ptr == ':' )
        {
            *ptr = 0;
            val0 = strtoul(last, NULL, 0);
            last = ptr + 1;

            value = &val1;

            mode = 1;
        }
        else if ( *ptr == ',' 
                || *ptr == ';' )
        {
            *ptr = 0;
            *value = strtoul(last, NULL, 0);
            
            if ( mode == 1 )
            {
                if ( val0 > 63 || val1 > 63 )
                    goto skip;

                if ( val0 > val1 )
                {
                    temp = val1;
                    val1 = val0;
                    val0 = temp;
                }

                DPrint_H(val0, "");
                DPrint_H(val1, "");
                for ( uint32_t i = val0; i <= val1; i++ )
                {
                    highlights |= (1ull<<i);
                }
                if ( val1 > maxHb )
                    maxHb = val1;
            }
            else
            {
                if ( val0 > 63 )
                    goto clean;

                highlights |= (1ull<<val0);
                
                if ( val0 > maxHb )
                    maxHb = val0;
            }

            mode = 0;
            value = &val0;

            val0 = (uint32_t)-1;
            val1 = (uint32_t)-1;

            last = ptr + 1;
        }
        else if ( *ptr < '0' || *ptr > '9' )
            goto clean;

    skip:
        ++ptr;
    }

    if ( last >= ptr )
        goto clean;

    *ptr = 0;
    *value = strtoul(last, NULL, 0);
    DPrint_C(*last, "");

    if ( mode == 1 )
    {
        if ( val0 > 63 || val1 > 63 )
            goto clean;

        if ( val0 > val1 )
        {
            temp = val1;
            val1 = val0;
            val0 = temp;
        }

        for ( uint32_t i = val0; i <= val1; i++ )
        {
            highlights |= (1ull<<i);
        }
                
        if ( val1 > maxHb )
            maxHb = val1;
    }
    else
    {
        if ( val0 > 63 )
            goto clean;

        highlights |= (1ull<<val0);
                
        if ( val0 > maxHb )
            maxHb = val0;
    }

    *MaxHighlightBit = maxHb;

clean:
    return highlights;
}

void printVersion()
{
    printf("Version: %s\n", EXE_VS);
    printf("Last changed: %s\n", EXE_LC);
}

void printUsage()
{
    printf("Usage: %s -n <number>|-x <hex>|-b <binary> [-m <values>] [-w <value>] [-s <value>]\n", EXE_NAME);
}

void printHelp()
{
    printUsage();
    printf("\n");
    printVersion();
    printf("\n");
    printf("Input:\n");
    printf("-n: Input number to convert to binary.\n");
    printf("-x: Input hex number to convert to binary. No prefix needed.\n");
    printf("-b: Input binary string to convert to a hex and decimal number.\n");
    printf("Options:\n");
    printf("-m: Highlight bits at given offsets. Offsets begin with 0 from right to left. Separate with comma (1,3,4), give ranges with colon (4:7,12:15).\n");
    printf("-w: Width of displayed bits. Valid values are multiples of 8 <= 64. Default: 32\n");
    printf("-s: Size of bit blocks. Valid values are powers of 2 <= 64. Default: 4\n");
}

int parseParams(
    int argc, 
    char** argv, 
    PCMD_PARAMS Params,
    int start_i
)
{
    int i;
    char* arg = NULL;
    char* val = NULL;
    int s = 0;

    Params->Width = WIDTH_INT32;
    Params->BlockSize = DEF_BLOCK_SIZE;

    for ( i = start_i; i < argc; i++ )
    {
        arg = argv[i];
        val = GET_ARG_VALUE(argc, argv, i, 1);

        if ( IS_1C_ARG(arg, 'n') )
        {
            if ( Params->Flags.BinarySet )
            {
                s = ERROR_INVALID_PARAMETER;
                EPrint("You've already set a binary string!\n");
                break;
            }

            BREAK_ON_NOT_A_VALUE(val, s, "missing input value!\n");

            Params->Value = strtoull(val, NULL, 0);
            Params->Flags.NumberSet = 1;

            i++;
        }
        else if ( IS_1C_ARG(arg, 'x') )
        {
            if ( Params->Flags.BinarySet )
            {
                s = ERROR_INVALID_PARAMETER;
                EPrint("You've already set a binary string!\n");
                break;
            }

            BREAK_ON_NOT_A_VALUE(val, s, "missing input value!\n");

            Params->Value = strtoull(val, NULL, 0x10);
            Params->Flags.NumberSet = 1;

            i++;
        }
        else if ( IS_1C_ARG(arg, 'b') )
        {
            if ( Params->Flags.NumberSet )
            {
                s = ERROR_INVALID_PARAMETER;
                EPrint("You've already set a number!\n");
                break;
            }

            BREAK_ON_NOT_A_VALUE(val, s, "missing binary value!\n");

            Params->BinaryValue = val;
            Params->Flags.BinarySet = 1;

            i++;
        }
        else if ( IS_1C_ARG(arg, 'm') )
        {
            BREAK_ON_NOT_A_VALUE(val, s, "missing highlight value!\n");

            Params->Highlight = val;

            i++;
        }
        else if ( IS_1C_ARG(arg, 's') )
        {
            BREAK_ON_NOT_A_VALUE(val, s, "missing size value!\n");

            Params->BlockSize = (uint8_t)strtoul(val, NULL, 0);

            i++;
        }
        else if ( IS_1C_ARG(arg, 'w') )
        {
            BREAK_ON_NOT_A_VALUE(val, s, "missing width value!\n");

            Params->Width = (uint8_t)strtoul(val, NULL, 0);

            i++;
        }
        else if ( IS_1C_ARG(arg, 'v') )
        {
            Params->Flags.Verbose = 1;
        }
    }

    return s;
}

int checkParams(
    PCMD_PARAMS Params
)
{
    int s = 0;

    if ( !Params->Flags.NumberSet && !Params->Flags.BinarySet)
    {
        EPrint("No input number given!\n");
        s = -1;
    }
    
    if ( Params->Flags.BinarySet )
    {
        if ( !Params->BinaryValue
          || !Params->BinaryValue[0] )
        {
            EPrint("Binary value too small or too big!\n");
            s = -1;
        }
        else
        {
            s = stripWhiteSpace(Params->BinaryValue);
            if ( s != 0 )
            {
                EPrint("Binary value too small or too big!\n");
            }
        }
    }

    if ( Params->Width % DEFAULT_ALIGN != 0 )
    {
        DPrint("[i] Invalid width specified! Setting to default!\n");
        Params->Width = ALIGN_UP_BY(Params->Width, DEFAULT_ALIGN);
    }
    if ( Params->Width > WIDTH_INT64 )
    {
        DPrint("[i] Width too big! Setting to max value (0x%x)!\n", WIDTH_INT64);
        Params->Width = WIDTH_INT64;
    }
    
    if ( Params->BlockSize < MIN_BLOCK_SIZE
        || (Params->BlockSize & (Params->BlockSize-1)) != 0
        || Params->BlockSize > MAX_BLOCK_SIZE
        )
    {
        DPrint("[i] Invalid block size specified! Setting to default!\n");
        Params->BlockSize = DEF_BLOCK_SIZE;
    }

    if ( s != 0 )
        printf("\n");

    return s;
}
