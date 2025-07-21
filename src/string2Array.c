#include <windows.h>

#include <stdint.h>
#include <stdio.h>

#include "utils/Converter.h"

#define FLAG_CHAR     (0x1)
#define FLAG_WCHAR    (0x2)
#define FLAG_REVERSED (0x4)
#define FLAG_XOR      (0x8)

int fixLabel(char* Label, size_t* LabelSize, uint32_t Flags);

void toCharEl(char c, size_t i, uint8_t* Key, uint32_t KeySize, uint32_t flags);

void toWCharEl(char c, size_t i, uint8_t* Key, uint32_t KeySize, uint32_t flags);



// compile
// cl strintToArray.c
int main(int argc, char** argv)
{
    int s = 0;

    if ( argc < 3 )
    {
        printf("Usage: %s <type> [-k <key>] string...\n", argv[0]);
        printf("\n");
        printf("type: 1: char, 2: wchar, 4: reversed\n");
        printf("-k: XOR encryption key given as a hex string\n");
        printf("strings: a set of strings to convert\n");

        return -1;
    }

    uint32_t flags = strtoul(argv[1], NULL, 0);
    char label[0x200];
    char* string = NULL;
    size_t stringSize;
    size_t labelSize;
    size_t i;
    int arg_i;
    int start_i = 2;
    uint8_t* key = NULL;
    uint32_t keySize = 0;

    // check mode
    uint32_t f = flags&(FLAG_CHAR|FLAG_WCHAR);
    if ( !f )
    {
        printf("Set Char (1) or WChar (2) mode!\n");
        return -1;
    }

    // get key
    if ( (argv[start_i][0] == '-' || argv[start_i][0] == '/') && argv[start_i][1] == 'k' && argv[start_i][2] == 0 )
    {
        s = parsePlainBytes(argv[start_i+1], &key, &keySize, UINT32_MAX);
        if ( s != 0 )
        {
            printf("ERROR: parsePlainBytes failed\n");
            return -1;
        }
        flags |= FLAG_XOR;
        start_i += 2;
    }

    for ( arg_i = start_i; arg_i < argc; arg_i++ )
    {
        string = argv[arg_i];
        stringSize = strlen(string);

        if ( stringSize < 1 )
            continue;

        label[0] = 0;
        labelSize = stringSize;

        if ( flags & FLAG_CHAR )
        {
            strcpy_s(label, 0x200, string);
            fixLabel(label, &labelSize, flags);
            
            printf("CHAR %s[%zu] = { ", label, stringSize+1);

            if ( flags & FLAG_REVERSED )
            {
                printf("0, ");
                for ( i = stringSize-1; i >= 1; i-- )
                {
                    toCharEl(string[i], i,key, keySize, 0);
                }
                if ( string[i] == '\\' )
                    printf("'\\\\' }");
                else
                    printf("'%c' };", string[i]);
            }
            else
            {
                for ( i = 0; i < stringSize; i++ )
                {
                    toCharEl(string[i], i, key, keySize, 0);
                }
                toCharEl(0, i, key, keySize, 1);
                //if ( !(flags&FLAG_XOR) )
                //    printf("0 };");
                //else
                //    printf("0x%02x };", (0^))
            }
        }
        
        if ( flags & (FLAG_CHAR|FLAG_WCHAR) )
            printf("\n");

        if ( flags & FLAG_WCHAR )
        {
            strcpy_s(label, 0x200, string);
            fixLabel(label, &labelSize, flags);

            printf("WCHAR %s[%zu] = { ", label, stringSize+1);
            
            if ( flags & FLAG_REVERSED )
            {
                printf("0, ");
                for ( i = stringSize-1; i >= 1; i-- )
                {
                    toWCharEl(string[i], i, key, keySize, 0);
                }
                if ( string[i] == '\\' )
                    printf("L'\\\\' }");
                else
                    printf("L'%c' };", string[i]);
            }
            else
            {
                for ( i = 0; i < stringSize; i++ )
                {
                    toWCharEl(string[i], i, key, keySize, 0);
                }
                toWCharEl(0, i, key, keySize, 1);
                //if ( !(flags&FLAG_XOR) )
                //    printf("0 };");
            }
        }
        printf("\n");
    }

    if ( key )
        free(key);

    return s;
}

void toCharEl(char c, size_t i, uint8_t* Key, uint32_t KeySize, uint32_t flags)
{
    if ( Key == NULL )
    {
        if ( flags == 1 )
            printf("0 };");
        else if ( c == '\\' )
            printf("'\\\\', ");
        else
            printf("'%c', ", c);
    }
    else
    {
        c = c ^ Key[i%KeySize];

        if ( flags == 1 )
            printf("0x%02x };", c);
        else
            printf("0x%02x, ", c);
    }
}

void toWCharEl(char c, size_t i, uint8_t* Key, uint32_t KeySize, uint32_t flags)
{
    if ( Key == NULL )
    {
        if ( flags == 1 )
            printf("0 };");
        else if ( c == '\\' )
            printf("L'\\\\', ");
        else
            printf("L'%c', ", c);
    }
    else
    {
        c = c ^ Key[i%KeySize];
        
        if ( flags == 1 )
            printf("0x%02x };", c);
        else
            printf("0x%02x, ", c);
    }
}

int fixLabel(char* Label, size_t* LabelSize, uint32_t Flags)
{
    int s = 0;
    size_t i;
    char* c;

    for ( i = 0; i < *LabelSize; i++ )
    {
        c = &Label[i];

        if (
            !((0x30 <= *c && *c <= 0x39) || (0x41 <= *c && *c <= 0x5A) || (0x61 <= *c && *c <= 0x7A))
            //*c == '.'
            //|| *c == ' '
            //|| *c == '\\'
            //|| *c == '/'
            //|| *c == '%'
            )
        {
            *c = '_';
        }
    }

    if ( Flags&FLAG_CHAR )
        *(PUINT16)&Label[*LabelSize] = 0x615f; // _a
    else if ( Flags&FLAG_WCHAR )
        *(PUINT16)&Label[*LabelSize] = 0x775f; // _w
    *LabelSize += 2;
    Label[*LabelSize] = 0;

    if ( 0x41 <= Label[0] && Label[0] <= 0x5A  )
        Label[0] += 0x20;

    return s;
}