##v3.2
1. fixed bug in writing MAC in RF_Test_Tool_with_iTest
2. add wifi scan ap test item in RF_Test_Tool

##v2.3 
improve RF test time 

##v2.2 
modified AT command from at+bleaddr to at+mpbleaddr

##v2.1 
a. add OTA FW Check which detect OTA FW support or not  
b. add XTAL Check  
c. add CR+LF to end of AT command

##v2.0 
a. default sysmode is user mode after RF test  
b. add MAC address feature in RF test with iTest  
c. Add MAC address source setting after MAC writing. Default is from Flash

##v1.9 
add a flag to change user mode or not after finishing RF Test

##v1.8 
Modify Rx test flow in Rx failed

##v1.7 
Add SEM criteria parser

##v1.6 
Fixed bug for parameter parser

##v1.5 
Add at+mprst command after switching to MP mode

##v1.4 
reBuild .exe file

##v1.3 
a. fixed bug when disabling MAC function, the tool won't show PASS when all test pass  
b. Before start to test RF, the tool will switch DUT to RF mode, after testing RF, the tool will switch to init mode.

##v1.2
fixed bug when reading ble address, tool shows error message in MAC.py

##v1.1 
add delay and retry in BLE MAC reading of MAC.py

##v1.0 
first Drift







