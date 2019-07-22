## Function
This demo is used to evaluate OPL1000 device OTA function through WIFI path.  User needs to run http_server.exe program on PC and make it as http server, OPL1000 connects http server and obtains OTA bin file, complete firmware upgrade operation . In order to use this function, user needs to implement below steps:    

1. Change AP IP /password and http server IP / Port according to real environment. 
2. Compile SDK\APS_PATCH\examples\system\ota_wifi example, generate m3 bin file. 
3. Pack m3 bin file with m0 bin file and build OTA image file. 
4. Download it into OPL1000 device and connect http server to upgrade firmware. 

## Content
1. OPL1000-Demo-ota-wifi-guide.pdf: Guide document of how to run this demo. 
2. http_server.exe:  PC program that provide http server function.  
3. opl1000_app_m3_ota_wifi.bin: M3 bin file that build from SDK\APS_PATCH\examples\system\ota_wifi example.
4. opl1000_m0.bin: M0 bin file. It is needed when pack M3  bin and M0 bin file to a whole firmware. This file is copied from FW_Pack folder. 
5. opl1000_ota_loader.bin: OPL1000 boot agant bin file. It is used when creating OTA format image file.  This file is copied from FW_Pack folder. 
6. opl1000_ota.bin: OTA upgrade example image file. It shall be placed under the folder where http_server.exe locates. 
7. PatchData.txt: Pack script file. 

## Note 
PC and OPL1000 device shall connect to same AP.  In other words they shall locate at local area network.     




