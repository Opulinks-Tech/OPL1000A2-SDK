# OPL1000A2-SDK
- OPL1000A2-SDK is Opulinks official software development kit for OPL1000 A2 chip.
- Provide 33 documents under "Doc" folder, includes Chinese and English version. Content covers DEVKIT use, SDK getting start and development, Chip boot mode, Flash use, Adaptive DTIM solution, API use introduction etc. 
- Provide three sets of tools for Pin multiplexing Settings, firmware download and mass production. 
- Provide 31 examples include WIFI, BLE, Peripheral, protocols and OTA examples. 
- Provide 7 dedicate Demo for user to evaluate OPL1000 functions/features.    

# Developing With the OPL1000A2-SDK
OPL1000A2-SDK provides complete guide documents, download and pin-mux tool, driver, SDK source code and example projects.
User can develop application based on OPL1000 HW reference and SW example projects easily.  

# Folder introduction  
- Demo: Contains 7 demos, they are:  
  - BLE_Config_AP:  Demonstrate how to use (Android) mobile APP to control OPL1000 complete WIFI AP connection via BLE path.  Please refer to [BLE_Config_AP readme](https://github.com/Opulinks-Tech/OPL1000A1-SDK/blob/master/Demo/BLE_Config_AP/README.md)
  - Iperf: Demonstrate how to use opl100 internal iperf  function to evaluate WIFI communication bandwidth. Please refer to [Iperf readme](https://github.com/Opulinks-Tech/OPL1000A1-SDK/blob/master/Demo/iperf/README.md)
  - OTA_WIFI: Demonstrate how to setup a TCP server on PC and complete OTA through WIFI path. Please refer to [OTA_WIFI readme](https://github.com/Opulinks-Tech/OPL1000A1-SDK/blob/master/Demo/OTA_WIFI/README.md)
  - TCP_Client: Demonstrate TCP data communication between OPL1000 (TCP client) and TCP server while enable OPL1000 smart sleep work mode. Please refer to [TCP_Client readme](https://github.com/Opulinks-Tech/OPL1000A1-SDK/blob/master/Demo/TCP_Client/README.md)
  - current_measure: Provide a tool for user to measure OPL1000 power consumption while keep connection with an AP. Please refer to [current_measure readme](https://github.com/Opulinks-Tech/OPL1000A1-SDK/blob/master/Demo/current_measure/readme.md)  
  - MQTT: Demonstrate how to establish a private MQTT service in local area network (LAN). Please refer to [MQTT readme](https://github.com/Opulinks-Tech/OPL1000A1-SDK/blob/master/Demo/MQTT/readme.md)  
  - ali_lighting: Demonstrate how to establish a MQTT connection with Ali cloud and use cloud intelligence APK to control lighting device. Please refer to [ali_lighting readme](https://github.com/Opulinks-Tech/OPL1000A1-SDK/blob/master/Demo/ali_lighting/readme.md)
- Doc: Contains 33 documents, include getting start, guide and application notes type documents. Have Chinese and English two languages version. 
- FW_Binary: Contains integrated OPL1000 firmwares. User can download it into OPL1000 device and evaluate functions directly.
- FW_Pack: Contains intermediate files that used for build integrated OPL1000 firmware.
- SDK: Contains OPL1000 SDK source code and examples. 
- Tool: Contains download, mass production tool and evaluation board drivers.  

# Use DEVKIT 
Refer OPL1000-DEVKIT-getting-start-guide.pdf under "Doc" folder to know how to use DEVKIT board.  

# Evaluate OPL1000 functionality 
Refer OPL1000-AT-instruction-set-and-examples.pdf to know how to use AT command to evaluate OPL1000 features. Please download "opl1000_at.bin" firmware under FW_Binary folder and execute AT command to understand OPL1000 functionality.

# Download firmware  
- Refer OPL1000-patch-download-tool-user-guide.pdf under "Doc" folder to know how to download firmware. 
- If DEVKIT board AT UART port cannot work, please install USB driver according to "OPL1000-DEVKIT-getting-start-guide" document section 3.1 

# Define external peripheral resource
Refer OPL1000-pinmux-tool-user-guide.pdf under "Doc" folder to know how to configure OPL1000 external peripheral resource. 

# Compiling user application project
Refer OPL1000-SDK-getting-start-guide.pdf and OPL1000-SDK-Development-guide.pdf to understand how to develop own application. 

# Development tool 
User can use two methods to develop application. 

- use Keil uVision 5 on Windows system. "opl1000_app_m3.uvprojx" project file is provided for each individual example under SDK\APS_PATCH\examples folder. User can open it by KeilC and add own function freely.     
- use GNU arm GCC tool. "Makefile" is provided for each individual example under SDK\APS_PATCH\examples folder. User can use Make tool to compile and link own application. How to install GCC development environment under Windows system please refer chapter4 of "OPL1000-SDK-Development-guide.pdf" document.