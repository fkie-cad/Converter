#ifndef ARGS_H
#define ARGS_H


#define LIN_PARAM_IDENTIFIER '-'
#define WIN_PARAM_IDENTIFIER '/'
#define PARAM_IDENTIFIER WIN_PARAM_IDENTIFIER

#define GET_ARG_VALUE(__argc__, __argv__, __i__, __id__) ( __i__ < __argc__ - __id__ ) ? __argv__[__i__+__id__] : NULL
#define IS_PARAM(__arg__) (__arg__ != NULL && ( __arg__[0] == LIN_PARAM_IDENTIFIER || __arg__[0] == WIN_PARAM_IDENTIFIER) )
#define IS_VALUE(__val__) (__val__ != NULL)
#define IS_1C_ARG(_a_, _v_) ( ( _a_[0] == LIN_PARAM_IDENTIFIER || _a_[0] == WIN_PARAM_IDENTIFIER ) && _a_[1] == _v_ && _a_[2] == 0 )
#define IS_2C_ARG(_a_, _v_) \
    ( ( _a_[0] == LIN_PARAM_IDENTIFIER || _a_[0] == WIN_PARAM_IDENTIFIER ) \
    && _a_[1] == ((_v_&0x0000FF00)>>0x08)  \
    && _a_[2] == ((_v_&0x000000FF))  \
    && _a_[3] == 0 )
#define IS_3C_ARG(_a_, _v_) \
    ( ( _a_[0] == LIN_PARAM_IDENTIFIER || _a_[0] == WIN_PARAM_IDENTIFIER ) \
    && _a_[1] == ((_v_&0x00FF0000)>>0x10)  \
    && _a_[2] == ((_v_&0x0000FF00)>>0x08)  \
    && _a_[3] == ((_v_&0x000000FF))  \
    && _a_[4] == 0 )
#define IS_4C_ARG(_a_, _v_) \
    ( ( _a_[0] == LIN_PARAM_IDENTIFIER || _a_[0] == WIN_PARAM_IDENTIFIER ) \
    && _a_[1] == ((_v_&0xFF000000)>>0x18)  \
    && _a_[2] == ((_v_&0x00FF0000)>>0x10)  \
    && _a_[3] == ((_v_&0x0000FF00)>>0x08)  \
    && _a_[4] == ((_v_&0x000000FF))  \
    && _a_[5] == 0 )
#define BREAK_ON_NOT_A_VALUE(__val__, __s__, __info__) \
{ \
    if ( ! IS_VALUE(__val__) ) \
    { \
        __s__ = ERROR_INVALID_PARAMETER; \
        printf("%s", __info__); \
        break; \
    } \
}

int isAskForHelp(int argc, char** argv);
int isArgOfType(const char* arg, const char* type);
int hasValue(const char* type, int i, int end_i);




int isAskForHelp(int argc, char** argv)
{
    int i = 1;
    if ( argc < i+1 )
        return 0;

    return IS_1C_ARG(argv[i], 'h') 
        || IS_1C_ARG(argv[i], '?');
}

int isArgOfType(const char* arg, const char* type)
{
    size_t type_ln;

    type_ln = strlen(type);

    return arg[0] == PARAM_IDENTIFIER 
        && strnlen(&arg[1], 10) == type_ln 
        && strncmp(&arg[1], type, type_ln) == 0;
}

int hasValue(const char* type, int i, int last_i)
{
    if ( i >= last_i )
    {
        printf("INFO: Arg \"%c%s\" has no value! Skipped!\n", PARAM_IDENTIFIER, type);
        return 0;
    }

    return 1;
}

#endif
