# Content
This folder contains intermediate binary firmware that used for generate integrated firmware.

Integrated firmware building procedure: 

- Step1: User develops own application on M3 MCU, create M3 bin file after compile and link operation; 
- Step2: Use download tool pack M3 file and M0 file into pure integrated OPL1000 firmware. 
- If want to create OTA type image, user need to use pack OTA loader with pure OPL1000 firmware. 

For use download tool  please refer document "OPL1000-patch-download-tool-user-guide.pdf" (Chinese version under .\Doc\zh_CN directory) or "OPL1000-patch-download-tool-user-guide_ENG.pdf" (English version under .\Doc\en directory) . 

# File list and brief description
1. opl1000_m0.bin: OPL1000 M0 binary corresponding to use internal PMU power supply. It is fixed and cannot be changed. 
2. opl1000_m0_ldo.bin: OPL1000 M0 binary corresponding to use external LDO power supply. It is fixed and cannot be changed. 
3. opl1000_ota_loader.bin: OTA boot agent binary. It is used to create OTA type firmware. 
4. PatchData.txt: script file that used to pack M0 and M3 binary file together. 

[Note] 

1. OPL1000 hardware setting is different when adopt different power supply method. Hence user shall choose opl1000_m0.bin or opl1000_m0_ldo.bin according to hardware design scheme. 
3. When user develops own application, M3 binary will be created by user. And it shall be packed with  M0 binary file in order to generate integrated firmware.  

