@ECHO OFF
SET TARGET_PATH=%1
SET OUTPUT_NAME=%2
SET LST_PATH=%TARGET_PATH%..\Listings\
REM #1 [$L] = ..\..\APS\targets\opl1000\Output\Objects\
REM #2 [@L] = opl1000_app_m3
REM (After build) .\opl1000_app_m3.bat $L @L

REM #Remove file
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.s37 DEL %TARGET_PATH%%OUTPUT_NAME%.s37 /Q
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.bin DEL %TARGET_PATH%%OUTPUT_NAME%.bin /Q
REM #Remove folder
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.s37 RD %TARGET_PATH%%OUTPUT_NAME%.s37 /Q
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.bin RD %TARGET_PATH%%OUTPUT_NAME%.bin /Q

SET FROM_ELF="C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe"

%FROM_ELF% %TARGET_PATH%%OUTPUT_NAME%.axf --m32combined --output %TARGET_PATH%%OUTPUT_NAME%.s37
%FROM_ELF% %TARGET_PATH%%OUTPUT_NAME%.axf --bin --output %TARGET_PATH%%OUTPUT_NAME%.bin
REM If multiple binary file output, force add extension name ".bin"
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.bin\* REN %TARGET_PATH%%OUTPUT_NAME%.bin\* *.bin




REM ====================================================================
REM Dump object sizes
SET SIZE_FILE=%LST_PATH%%OUTPUT_NAME%_size.txt
SET PATCH_LIB=%TARGET_PATH%opl1000_patch_m3.lib
SET AXF_FULL_PATH=%TARGET_PATH%%OUTPUT_NAME%.axf
SET ERRORLEVEL=0
IF EXIST %SIZE_FILE% ( DEL %SIZE_FILE% )
ECHO "%OUTPUT_NAME%" | FINDSTR /C:"at" > NUL && (
    SET SDK_LIB=%TARGET_PATH%opl1000_sdk_at_m3.lib
) || (
    SET SDK_LIB=%TARGET_PATH%opl1000_sdk_m3.lib
)


ECHO. >> %SIZE_FILE%
ECHO %PATCH_LIB% >> %SIZE_FILE%
%FROM_ELF% --info=totals %PATCH_LIB% >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%
ECHO %SDK_LIB% >> %SIZE_FILE%
%FROM_ELF% --info=totals %SDK_LIB% >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%
ECHO %AXF_FULL_PATH% >> %SIZE_FILE%
%FROM_ELF% --info=totals %AXF_FULL_PATH% >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%
