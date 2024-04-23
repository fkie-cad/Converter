#pragma once

int stripWhiteSpace(char* String)
{
    if ( !String )
        return -1; // no string

    char buffer[0x41] = {0};
    char* stringPtr = String;
    size_t bufferOffset = 0;
    
    if ( *stringPtr == 0 )
        return -2; // string empty

    // copy stripped string into buffer
    while ( *stringPtr != 0 )
    {
        if ( *stringPtr != ' ' && *stringPtr != '\t' )
        {
            buffer[bufferOffset] = *stringPtr;
            bufferOffset++;
        }
        stringPtr++;
        if ( bufferOffset >= 0x40 && *stringPtr != 0 )
            return -3; // binary to big
    }
    
    // copy back
    char* bufferPtr = buffer;
    stringPtr = String;
    while ( *bufferPtr != 0 )
    {
        *stringPtr = *bufferPtr;

        bufferPtr++;
        stringPtr++;
    }
    *stringPtr = 0;

    return 0;
}
