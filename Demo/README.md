## Demo includes:

1. BLE_Config_AP:  Demonstrate how to use (Android) mobile APP to control OPL1000 complete WIFI AP connection via BLE path. 
2. Iperf: Demonstrate how to use opl100 internal iperf  function to evaluate WIFI communication bandwidth.
3. OTA_WIFI: Demonstrate how to setup a TCP server on PC and complete OTA through WIFI path.
4. TCP_Client: Demonstrate TCP data communication between OPL1000 (TCP client) and TCP server while enable OPL1000 smart sleep work mode. 
5. current_measure: Provide a tool for user to measure OPL1000 power consumption while keep connection with an AP.  
6. MQTT: Demonstrate how to establish a private MQTT service in local area network (LAN). 

## BLE_Config_AP

This demo shows how to use (Android) mobile APP to control OPL1000 completes below functions 

1. Use BLE to configure wireless AP. 
2. Complete AP connection. 
3. Update firmware through BLE connection (BLE OTA). 

Note: 

(1) Refer "OPL1000-Demo-BLE-setup-network-and-BLE-OTA-guide.pdf" to know how to execute this demo 

(2) To evaluate BLE OTA function mobile app needs to store an OTA image file in advance     

## Iperf

This demo shows how to use opl1000 internal iperf  function to evaluate WIFI communication bandwidth.

This demo supports TCP/UDP transfer while OPL1000 acting as server or client.    

Please refer OPL1000-Iperf-Measurement-Guide.pdf to know how to setup evaluation environment and execute demo. 

## OTA_WIFI 

This demo shows how to setup a TCP server on PC and complete OTA through WIFI path.  Notes:

1. AP ssid, password and TCP server IP, port etc. parameters need to be defined in "ota_wifi" example source code
2. After "ota_wifi" example is compiled, M3 bin file needs to be packed with M0 binn file and formed as OTA format image file. 

 Please refer "OPL1000-Demo-ota-wifi-guide.pdf" to know detailed information of how to build image file and execute demo. 

## TCP_Client 

This demo shows OPL1000 establish data communication with a TCP server while enable smart sleep work mode. By this demonstration user can

1. Evaluate OPL1000 power consumption while keep TCP data exchanging 
2. Know how to establish TCP communications between OPL1000 and TCP server 

Note: AP ssid, password and TCP server IP, port etc. parameters need to be defined in "ota_wifi" example source code

Please refer "OPL1000-Demo-tcp-client-guide.pdf" to know detailed information of how to build image file and execute demo. 

## current_measure 

This demo provides user with a quick method to measure power consumption of OPL1000 with different options. By this demonstration user can

1. Configure and update the parameters of  wpa2_station project with special requirements 
2. Download opl1000.bin file for measuring power consumption of OPL1000 with different configurations. 

Please refer to "readme.md" and "OPL1000-Demo-low-power-current-measure-guide.pdf" files in current_measure folder for more information of how to use it. 

## MQTT

This demo shows how to establish MQTT service (publish and subscribe) in a local area network. By this demonstration user can

1. Use  MQTT example (under SDK\APS_PATCH\examples\protocols\mqtt folder) to build an firmware image to support private MQTT service.
2. Install EMQ tool on a PC act as MQTT broker. And then OPL1000 device can establish MQTT service with broker. 

Please refer "OPL1000-Demo-MQTT-guide.pdf" document  in MQTT folder for more information of how to use it. 

