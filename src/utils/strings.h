#pragma once

#include <stdint.h>
#include <malloc.h>

#include "../inc/cross.h"

#define IS_LC_CHAR(__char__) \
    ( (__char__) >= 'a' && (__char__) <= 'z' )



int stripWhiteSpace(char* String, uint32_t MaxSize)
{
    int s = 0;
    char* bufferPtr = NULL;

    if ( !String || *String == 0 )
        return -1; // no string

    if ( !MaxSize )
        return -2; // no size

    char* buffer = (char*)malloc(MaxSize + 1);
    if ( !buffer )
        return -4;
    char* stringPtr = String;
    size_t bufferOffset = 0;
    
    // copy stripped string into buffer
    while ( *stringPtr != 0 )
    {
        if ( *stringPtr != ' ' && *stringPtr != '\t' )
        {
            buffer[bufferOffset] = *stringPtr;
            bufferOffset++;
        }
        stringPtr++;
        if ( bufferOffset >= MaxSize && *stringPtr != 0 )
        {
            s = -3; // binary to big
            goto clean;
        }
    }
    buffer[min(bufferOffset,MaxSize)] = 0;
    
    // copy back
    bufferPtr = buffer;
    stringPtr = String;
    while ( *bufferPtr != 0 )
    {
        *stringPtr = *bufferPtr;

        bufferPtr++;
        stringPtr++;
    }
    *stringPtr = 0;

clean:
    if ( buffer )
        free(buffer);

    return s;
}

// destructive
char* strip(char* String)
{
    if ( !String )
        return NULL; // no string

    char* stringPtr = String;
    char* stringStart = String;
    
    if ( *stringPtr == 0 )
        return String; // string empty

    // lstrip
    while ( *stringPtr != 0 )
    {
        if ( *stringPtr == ' ')
            stringStart++;
        else 
            break;

        stringPtr++;
    }

    // rstrip
    while ( *stringPtr != 0 )
    {
        if ( *stringPtr == ' ')
        {
            *stringPtr = 0;
            break;
        }

        stringPtr++;
    }
    
    return stringStart;
}

/**
 * Split string into its parts, determined by the delimiter.
 * str will be changed!!!
 * TODO: show, if max_bucket is exceeded
 *
 * @param   str char* the source string
 * @param   delimiter char* the splitting delimiter
 * @param   bucket char** the bucket to hold the parts (should be allocated memory)
 * @param   bucket_max size_t the max size of the bucket (if there are more parts, the loop will break before)
 * @return  size_t the size of the actual bucket, i.e. number of found parts.
 */
size_t split(char* str, const char* delimiter, char** bucket, const size_t bucket_max)
{
    char* token = NULL;
    char* next_token = NULL;

    if ( !str || !delimiter || !bucket )
        return 0;

    //token = strtok(str, delimiter);
    token = strtok_s(str, delimiter, &next_token);
    size_t token_id = 0;

    while ( token != NULL )
    {
        bucket[token_id] = token;
        //token = strtok(NULL, delimiter);
        token = strtok_s(NULL, delimiter, &next_token);

        token_id++;
        if ( token_id >= bucket_max )
            break;
    }

    return token_id;
}

int startsWith(char* string, char* prefix)
{
    char* pfxPtr = prefix;
    char* strPtr = string;

    while ( *strPtr != 0 && *pfxPtr != 0 )
    {
        if ( IS_LC_CHAR(*strPtr) )
            *strPtr -= 0x20;

        if ( *strPtr != *pfxPtr )
            break;
        strPtr++;
        pfxPtr++;
    }

    return *pfxPtr == 0;
}

char* strToUC(char* string)
{
    char* strPtr = string;

    while ( *strPtr != 0 )
    {
        if ( IS_LC_CHAR(*strPtr) )
            *strPtr -= 0x20;

        strPtr++;
    }

    return string;
}
