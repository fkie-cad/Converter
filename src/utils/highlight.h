#pragma once

#include <stdio.h>

#define HIGHLIGHT_HEX_STYLE "\033[1;42m"
#define NORMAL_STYLE "\033[0m"
#define POS_HEX_STYLE "\033[1m"
#define LIGHT_STYLE "\033[38;2;150;150;150m"

void setAnsiFormat(char* format)
{
    printf("%s", format);
}

void resetAnsiFormat()
{
    printf(NORMAL_STYLE);
}