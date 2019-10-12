SET Project=opl1000
SET TargetPath=..\..\targets\%Project%\Output\Objects

C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe %TargetPath%\opl1000_boot_agent_m3.axf --m32 --output %TargetPath%\opl1000_boot_agent_m3.s37
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe %TargetPath%\opl1000_boot_agent_m3.axf --bin --output %TargetPath%\opl1000_boot_agent_m3.bin
