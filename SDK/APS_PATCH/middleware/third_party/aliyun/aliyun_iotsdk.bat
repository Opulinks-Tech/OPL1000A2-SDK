SET OBJ_PATH=%1
SET OBJ_FULL_PATH=%2
SET OUTPUT_NAME=%3
@REM #1 [$L] = .\Output\Objects\
@REM #2 [!L] = .\Output\Objects\libiot_sdk.lib
@REM #3 [@L] = libiot_sdk

SET LIB_SRC=..\..\..\..\APS\targets\opl1000\Output\Objects\
SET LIB_DST=.\lib\

IF EXIST %LIB_DST%%OUTPUT_NAME%.lib ( DEL %LIB_DST%%OUTPUT_NAME%.lib )

copy %LIB_SRC%%OUTPUT_NAME%.lib %LIB_DST%
