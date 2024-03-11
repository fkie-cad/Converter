@echo off
setlocal enabledelayedexpansion

set my_name=%~n0
set my_dir="%~dp0"
set "my_dir=%my_dir:~1,-2%"


set /a DP_FLAG=1
set /a EP_FLAG=2


:: project ids
set /a id=0
set /a IOCTL=%id%
set /a id+=1
set /a N2B=%id%
set /a id+=1
set /a S2DE=%id%
set /a id+=1
set /a S2HX=%id%
set /a PROJ_ID_MAX=%id%

:: project file names
set projects[%IOCTL%]=CtlCode
set projects[%N2B%]=Num2Bin
set projects[%S2DE%]=NtStatusToDosError
set projects[%S2HX%]=String2Hex

:: do compile flags
set /a cmpl[%IOCTL%]=0
set /a cmpl[%N2B%]=0
set /a cmpl[%S2DE%]=0
set /a cmpl[%S2HX%]=0

:: batch params
set cmdparams[%IOCTL%]=/ioctl
set cmdparams[%N2B%]=/n2b
set cmdparams[%S2DE%]=/s2dE
set cmdparams[%S2HX%]=/s2h


set proj_dir=.
set proj_ftype=.vcxproj
set proj_name=Converter

set /a all=0

set /a debug=0
set /a release=0

REM set /a bitness=64
set /a pdb=0
set /a debug_print=%EP_FLAG%
set platform=x64
::set pts=WindowsApplicationForDrivers10.0
set pts=v142
set /a er=3
set /a rtl=0
set /a verbose=0

set /a all=0
set /a cln=0



GOTO :ParseParams

:ParseParams

    if [%1]==[] goto main
    if [%1]==[/?] goto help
    if [%1]==[/h] goto help
    if [%1]==[/help] goto help
    
    :: projects
    FOR /L %%i IN (0 1 %PROJ_ID_MAX%) DO  (
        if /i [%~1] == [!cmdparams[%%i]!] (
            set /a cmpl[%%i]=1
            goto reParseParams
        )
    )
    
    IF /i "%~1"=="/all" (
        SET /a all=1
        goto reParseParams
    )

    IF /i "%~1"=="/cln" (
        SET /a cln=1
        goto reParseParams
    )

    IF /i "%~1"=="/d" (
        SET /a debug=1
        goto reParseParams
    )
    IF /i "%~1"=="/r" (
        SET /a release=1
        goto reParseParams
    )
    
    IF /i "%~1"=="/dp" (
        SET /a "debug_print=%~2"
        SHIFT
        goto reParseParams
    )

    IF /i "%~1"=="/pdb" (
        SET /a pdb=1
        goto reParseParams
    )
    IF /i "%~1"=="/rtl" (
        SET /a rtl=1
        goto reParseParams
    )

    IF /i "%~1"=="/pts" (
        SET pts=%~2
        SHIFT
        goto reParseParams
    )

    REM IF /i "%~1"=="/b" (
    REM     SET /a bitness=%~2
    REM     SHIFT
    REM     goto reParseParams
    REM )

    IF /i "%~1"=="/v" (
        SET /a verbose=1
        goto reParseParams
    ) ELSE (
        echo Unknown option : "%~1"
    )
    
    :reParseParams
    SHIFT
    if [%1]==[] goto main

GOTO :ParseParams


:main

    set /a "s=%debug%+%release%"
    if %s% == 0 (
        set /a debug=0
        set /a release=1
    )

    REM set platform=
    REM if %bitness% == 64 (
    REM     set platform=x64
    REM )
    REM if %bitness% == 32 (
    REM     set platform=x86
    REM )
    REM if platform == [] (
    REM     echo [e] Bitness /b has to be 32 or 64!
    REM     EXIT /B 1
    REM )

    if %all% == 1 (
        FOR /L %%i IN (0 1 %PROJ_ID_MAX%) DO  (
            set /a cmpl[%%i]=1
        )
    )
    
    :: check if a project is set
    set /a s=0
    FOR /L %%i IN (0 1 %PROJ_ID_MAX%) DO  (
        if !cmpl[%%i]! == 1 (
            set /a s=1
            goto endloop
        )
    )
    :endLoop
    set /a "s+=%cln%"

    :: no target set
    if %s% == 0 (
        echo [e] No target set!
        exit /b -1
    )

    if %verbose% == 1 (
        FOR /L %%i IN (0 1 %PROJ_ID_MAX%) DO  (
            echo !projects[%%i]!: !cmpl[%%i]!
        )
        echo.
        echo debug: %debug%
        echo release: %release%
        REM echo bitness: %bitness%
        echo pdb: %pdb%
        echo dprint: %debug_print%
        echo rtl: %rtl%
        echo pts: %pts%
    )
    
    :: clean up
    if %cln% == 1 (
        echo removing "%my_dir%\build"
        rmdir /s /q "%my_dir%\build" >nul 2>&1 
    )
    
    :: build projects
    FOR /L %%i IN (0 1 %PROJ_ID_MAX%) DO  (
        if !cmpl[%%i]! == 1 (
            call :build !projects[%%i]! %%i

            if NOT !ERRORLEVEL! == 0 (
                goto buildLoopEnd
            )
        )
    )
    :buildLoopEnd

    echo build finished with code : !ERRORLEVEL!
    exit /B !ERRORLEVEL!

    exit /B 0



:build
    SETLOCAL
        set sub_proj_name=%~1
        set proj=%proj_dir%\%proj_name%%proj_ftype%
        
        if %debug%==1 call :buildEx %proj%,%sub_proj_name%,%platform%,Debug,%debug_print%,%rtl%,%pdb%,%pts%
        if %release%==1 call :buildEx %proj%,%sub_proj_name%,%platform%,Release,%debug_print%,%rtl%,%pdb%,%pts%
    ENDLOCAL
    
    EXIT /B %ERRORLEVEL%
    
:buildEx
    SETLOCAL
        set proj=%~1
        set sub_proj=%~2
        set platform=%~3
        set conf=%~4
        set /a dpf=%~5
        set rtl=%~6
        set pdb=%~7
        set pts=%~8
        
        :: print flags
        set /a "dp=%dpf%&~EP_FLAG"
        set /a "ep=%dpf%&EP_FLAG"
        if not %ep% == 0 (
            set /a ep=1
        )

        if %rtl% == 1 (
            set rtl=%conf%
        ) else (
            set rtl=None
        )

        :: pdbs
        if [%conf%] EQU [Debug] (
            set /a pdb=1
        )
        
        if %verbose% == 1 (
            echo build
            echo  - Project=%proj%
            echo  - SubProject=%sub_proj%
            echo  - Platform=%platform%
            echo  - Configuration=%conf%
            echo  - DebugPrint=%dp%
            echo  - RuntimeLib=%rtl%
            echo  - DebugPrint=%dp%
            echo  - ErrorPrint=%ep%
            echo  - pdb=%pdb%
            echo  - pts=%pts%
            echo.
        )
        
        msbuild %proj% /p:SubProj=%sub_proj% /p:Platform=%platform% /p:Configuration=%conf% /p:DebugPrint=%dp% /p:ErrorPrint=%ep% /p:RuntimeLib=%rtl% /p:PDB=%pdb% /p:PlatformToolset=%pts%
        echo.
        echo ----------------------------------------------------
        echo.
        echo.
    ENDLOCAL
    
    EXIT /B %ERRORLEVEL%




:usage
    echo|set /p="Usage: %my_name% [/all] "
    FOR /L %%i IN (0 1 %PROJ_ID_MAX%) DO  (
        echo|set /p="[!cmdparams[%%i]!] "
    )
    echo|set /p="[/d] [/r] [/dp <value>] [/b 32^|64] [/pdb] [/xrtl]"
    exit /B 0
    

:help
    call :usage
    echo.
    echo Targets:
    echo /ioctl: Build ioctl code converter.
    echo /n2b: Number to binary converter.
    echo /s2dE: Build NTSTATUS code to DOS error code converter.
    echo /s2hx: Build string ^<=^> hex converter.
    echo.
    echo Options:
    echo /d: Build in debug mode.
    echo /r: Build in release mode.
    echo /b: Bitness of exe. 32^|64. Default: 64.
    echo /pdb: Compile with pdbs.
    echo /rtl: Compile with RuntimeLibrary.
    echo /pts: Set the PlatformToolset. Defaults to "v142".
    echo.
    echo Debug print:
    echo /dp: Debug print flag value.
    echo    Debug print flag: %DP_FLAG%. Print debug stuff.
    echo    Error print flag: %EP_FLAG%. Print errors.
    echo.
    echo /h: Print this.
    echo.
    exit /B 0
