@SET OBJ_PATH=%1
@SET AXF_FULL_PATH=%2
@SET OUTPUT_NAME=%3
@REM #1 [$L] = .\Output\Objects\
@REM #2 [!L] = .\Output\Objects\opl1000_app_m3.axf
@REM #3 [@L] = opl1000_app_m3

@REM #Remove file
@IF EXIST %OBJ_PATH%%OUTPUT_NAME%.s37 DEL %OBJ_PATH%%OUTPUT_NAME%.s37 /Q
@IF EXIST %OBJ_PATH%%OUTPUT_NAME%.bin DEL %OBJ_PATH%%OUTPUT_NAME%.bin /Q

@SET AUTO_GEN_RELEASE=0
@SET FROM_ELF=C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe
@SET SREC_CAT=..\..\..\..\APS\tools\srec_cat.exe

%FROM_ELF% %AXF_FULL_PATH% --m32combined --output %OBJ_PATH%%OUTPUT_NAME%.s37
%FROM_ELF% %AXF_FULL_PATH% --bin --output %OBJ_PATH%%OUTPUT_NAME%.bin
