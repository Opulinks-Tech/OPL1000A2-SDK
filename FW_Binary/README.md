# Content
This folder contains binary firmware that used to:

- Use AT command to evaluate OPL1000 functions

How to use these binary file and complete firmware pack, download operation, please refer document "OPL1000-patch-download-tool-user-guide.pdf" (Chinese version under .\Doc\zh_CN directory) or "OPL1000-patch-download-tool-user-guide_ENG.pdf" (English version under .\Doc\en directory)  to know detailed information. 

# File list and brief description  
1. opl1000_at.bin: OPL1000 integral firmware file that support AT command.It is packed by m3.bin(generate by compiling blewifi example's transparent target ) and m0 binary file.  

[Note] 

1. In order to understand AT command. Please refer document "OPL1000-AT-instruction-set-and-examples.pdf" (Chinese version under .\Doc\zh_CN directory) or "OPL1000-AT-instruction-set-and-examples_ENG.pdf" (English version under .\Doc\en directory)  to know detailed information. Update firmware through BLE connection (BLE OTA). 
2. When user develops own application, M3 binary will be created by user. And it shall be packed with  M0 binary file.  

