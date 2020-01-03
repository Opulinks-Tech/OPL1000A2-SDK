## Release Version:
SDK Package: MP_2.16
Patch_Lib : 4208

## Release Date: 2020/1/3

## Function & Feature Update 
1. Documents updating:
   1.1 OPL1000-AT-instruction-set-and-examples,R05-v0.42,Add commands for AUXADC.
   1.2 OPL1000-WIFI-BLE-API-guide.pdf,MP2.16, update document.
   1.3 OPL1000_peripheral_PWM_application_notes.pdf, V0.4, add PWM example result in complex mode.
   1.4 OPL1000-Power-Saving-Introduction, R01-V03, Update wake-up time and section 4.1
   1.5 OPL1000-Demo-ota-wifi-guide.pdf, R01-V04, update http_server.exe user guide.
2. Tool version information and update:
   2.1 pin-mux tool: v0.8
   2.2 MP multi-dev download tool: update to v2.13.
3. Demo update:
   3.1 http_server.exe for OTA_WiFi demo, v1.2, add some options.
4. Examples update:
   4.1 auxadc: update based on new HAL API.
   4.2 awsIOT_blewifi: move it as a reference - OPL1000A2-Sensor-Device-Reference-Code-Aws-Cloud-with-MQTT.
   4.3 pwm: realize duty cycle gradual change waveform with timer and complex mode.
5. Added auto-calibrate method.
6. OPL1000A2 setting of TX+2db adjustment.
7. Add LE gain control function.
8. Add at+tx command to transmit every packets with RF TX/RX switch.


## Bug Fix List 
1. fix "DebugUART wrong handler after warmboot".
2. fix "Timer-sleep duration overflow issue".
3. fix "memory leak in AT+CWMODE=4 command".
4. fix "Device not awakened by expected timing".

## Notes List 

*****

## Release Version:
SDK Package: MP_2.15
Patch_Lib : 4132
ROM_CODE: 1655 

## Release Date: 2019/11/29

## Function & Feature Update 
1. Documents updating:
   1.1 OPL1000-AT-instruction-set-and-examples,R05-v0.41,Change default AT+RFHP to 176.
   1.2 OPL1000-WIFI聯通性測試報告,new document about  WIFI IOP result that done in TestIn.
2. Tool version information and update:
   2.1 download tool: v0.27, No update.
   2.2 pin-mux tool: v0.8,No update.
   2.3 MP RF test tool:  v3.2,No update.
   2.4 MP multi-dev download tool: update to v2.11.
3. Demo update:
   3.1 opulinks_iot_app.apk, Android app update to v1.6.0.
4. Examples update:
   4.1 mqtt: default RF value is changed to 0x20.
   4.2 awsIOT_blewifi: default RF value is changed to 0x20.
   4.3 ali_blewifi: Add MAC address source setting and read function. Change BLE adv interval time as 30 minutes.
   4.4 blewifi: Add MAC address source setting and read function.
5. Add static power voltage checking in main loop at M0.
6. Add new feature for calibrate the parameters relative to temperature(TCA).
7. Setting changed for AOS_024: Control for PMU_SF (used in SEQ) output.
8. Add DCOC in warm-boot stage.
9. Add optional of "Boosting 2db/3db on Wi-Fi HPA".
10. optimize tcp pcb allocation algorithm.
11. Skip DTIM enhancement - Set skip DTIM to 0 when last Beacon lost



## Bug Fix List 
1. fix "deep sleep wakeup failed by IO".
2. fix "automatic voltage checking on ext ldo board".
3. fix "using Iphone XS(IOS 13) as wifi hotspot failed to connect network".
4. fix "iperf performance degraded after temperature calibaration(TCA) triggered".

## Notes List 

*****

## Release Version:
SDK Package: MP_2.13
Patch_Lib : 4073

## Release Date: 2019/11/3

## Function & Feature Update 
1. Documents updating:
   1.1 OPL1000-AT-instruction-set-and-examples,R05-v0.39,Add SMPS_RF tuning for AT+RFTM.
   1.2 OPL1000-WIFI聯通性測試報告,new document about  WIFI IOP result that done in TestIn.
   1.3 OPL1000-BLE聯通性測試報告R2,new document about BLE IOP result that done in TestIn.
2.  Tool version information and update:
      2.1 download tool: v0.27, No update.
      2.2 pin-mux tool: v0.8. No update.
      2.3 MP RF test tool: update to v3.2, add a  new "RF test tool" that support iTest instrument.
      2.4 MP multi-dev download tool: update to v2.9, support Ali 5 unit writing fuction.
3. Demo update:
   3.1 Expand_M3_RAM，new demo that shows how to use M3 shared memory to enlarge M3 MCU avaliable RAM size for application.
4. Examples update:
   4.1 https_request:  add preference ciphersuites setup operation to speed up SSL handshake procedure.
5. Open block 0 of shared-memory to let user use this part of memory in application.
6. Add function of convert little/big endian for OTP mac address.
7. Add an API and command for boost VDD_RF(SMPS_RF) from 1.2(default) to 1.4.



## Bug Fix List 
1. clean up the Ethernet ARP table after WiFi disconnect
2. fix "writing a FIM record whose size is more than 2048 bytes will overwrite other FIM group" issue
3. fix "sometimes fail to write BLE bd_addr to flash using "at+mpbleaddr" command" issue
4. fix "RSSI always show -104 in BLE RX mode" issue


## Notes List 

*****

## Release Version:
SDK Package: MP_2.12
Patch_Lib : 4061

## Release Date: 2019/10/12

## Function & Feature Update 
1. Documents updating:
   1.1 OPL1000-multiple-dev-download-tool-user-guide,R01-v07,updated according to MP download tool v2.8 new functions.
   1.2 OPL1000-SDK-Development-guide,R01-v17, Add a chapter to introduce how to adjust heap size.

   1.3 OPL1000-AT-instruction-set-and-examples,R05-v38,Add Add AT+RFHP = 32 ,Wi-Fi LPA boost 2db and BLE LPA

2. Tool version information and update:
   2.1 download tool: v0.27. no update.
   2.2 pin-mux tool: v0.8. No update.
   2.3 MP RF test tool: v2.3. no update.
   2.4 MP multi-dev download tool: v2.8, resource FW download;mac address gets from csv file.

3. Add 32k Real Clock support API 

4. AT instruction support Low-Power WiFi TX setting


## Bug Fix List 
1. fix "Dropping packets frequently by heavy interference" issue
2. fix "OPL1000A2 TX registers adjustment (+2db)"issue
3. fix "Sleep wakeup CPOR_N release time" issue


## Notes List 

*****

## Release Version:
SDK Package: MP_2.11
Patch_Lib : 4048

## Release Date: 2019/9/27

## Function & Feature Update 
1. Documents updating:
   1.1 OPL1000-multiple-dev-download-tool-user-guide,R01-v06, add pc binding, flash write protection. 
   1.2 add OPL1000-Demo-MQTT-guide_ENG,R01-v01.
   1.3 add OPL1000-Demo-tcp-client-guide_ENG,R01-v04.
   1.4 add OPL1000-Demo-ota-wifi-guide_ENG,R01-v03.
   1.5 add OPL1000-Demo-BLE-setup-network-and-BLE-OTA-guide_ENG,R01-v07.
   1.6 update OPL1000-Power-Consumption-Measurement-Guide_ENG and OPL1000-Power-Consumption-Measurement-Guide,update figure 10,R01-v04.
2. Tool version information and update:
   2.1 download tool: v0.27. no update.
   2.2 pin-mux tool: v0.8. No update.
   2.3 MP RF test tool: v2.3. no update
   2.4 MP multi-dev download tool: v2.6, add pc binding, flash write protection.
3. LWIP:optimize tcp pcb allocation algorithm.
4. Improve offset channel performance.
5. opulinks_iot_app.apk update to v1.51.

## Bug Fix List 
1. fix "when customer post data,sometimes there may be unable to go to sleep condition for 5s" issue.
2. fix" httpclient  get file failed in cumstomer's Server" issue
3. fix "mqtt example GCC bin: OPL1000 may be hung when connecting wifi" issue
4. fix "RSSI vary with different scan mode" issue
5. fix "awsIOT_blewifi example wifi mac addr fail to update" issue



## Notes List 

*****

## Release Version:
SDK Package: MP_2.10
Patch_Lib : 4024

## Release Date: 2019/9/7

## Function & Feature Update 
1. Documents updating:
   1.1 OPL1000-multiple-dev-download-tool-user-guide,R01-v05, fix some typo. 
2. Tool version information and update:
   2.1 download tool: v0.27. no update.
   2.2 pin-mux tool: v0.8. No update.
   2.3 MP RF test tool: v2.3. no update
   2.4 MP multi-dev download tool: v2.2, support A2 chip firmware download and BLE/WIFI MAC programming.
3. Add ali_blewifi example.


## Bug Fix List 
1. fix "BLE scan fial in MP2.9 ( SCA VS VCO output bevavir abnormal)" issue
2. fix "there is no linking error report when the size is overflow" issue.
3. fix "the initial flow of driver part" issue
4. fix "RF power setting failed at BleWifiAppInit" issue
5. fix "UART rx interrupt triggered wrongly" issue.

## Notes List 

*****

## Release Version:
SDK Package: MP2.9
Patch_Lib : 4004

## Release Date: 2019/8/30

## Function & Feature Update 
1. Documents updating:
   1.1 OPL1000-SDK-Development-guide,R01-v16, Add  Setting up GUN ARM GCC toolchain and build tool Section. 
   1.2 OPL1000-AT-instruction-set-and-examples,R05-v37, Modify AT+CWLAP section’s description
   1.3 OPL1000-WIFI-BLE-API-guide,MP2.9, Support specific ssid/bssic for wifi scan
2. Tool version information and update:
   2.1 download tool: v0.27. Enhance compatibility of different boards.
   2.2 pin-mux tool: v0.8. No update.
   2.3 MP_Tool: v2.3. Fix ble mac address writing issue.
3. Add ext LDO binary to SDK.
4. support building project by GNU ARM GCC
5. Add a new MQTT example under SDK\APS_PATCH\examples\protocols\mqtt folder; This example shows how to establish a private MQTT service in local area network.
6. Add a new demo "MQTT" under Demo\MQTT to show how to use "MQTT" example to establish a local MQTT service in LAN. Here EMQ is used as MQTT broker. 
7. Support specific ssid/bssic for wifi scan in WIFI-API.

## Bug Fix List 
1. fix "peer address type is wrong for LE Enhanced Connection Complete Event" issue
2. fix "M3 may send BLE commands before M0 IPC initialization done" issue.
3. fix "Even AT command echo is off, AT task still prompts unexpected CR+LF before processing command" issue
4. fix "AP is far from LM80, DTIM will have more power consumption" issue
5. fix "peak current exceeding to 40mA during system initializing" issue.
6. fix "the size of M3 Bin built with GCC is too large" issue
7. fix "update write Wi-Fi MAC address format in OTP" issue

## Notes List 

*****

## Release Version:

SDK Package: MP v2.6
Patch_Lib : 3840

## Release Date: 2019/07/22

## Function & Feature Update 
​    1. Documents updating:

​        1.1 OPL1000-Iperf-Measurement-Guide,R01-v01, Initial Release 

​        1.2 OPL1000-Demo-BLE-setup-network-guide,R01-v05, introduce IOS APP operation and change  BLEWIFI example location file path from Bluetooth to System                     

​        1.3 OPL1000-BLEWIFI-Application-Dev-Guide, R01-v07,Modify blewifi example location path

​        1.4 OPL1000-DEVKIT-getting-start-guide, R02-v06,Update Fw_binary folder to FW_Pack folder

​        1.5 OPL1000-SDK-getting-start-guide,R02-v07, modify fw_binary folder to fw_pack folder

​        1.6 OPL1000-Demo-ota-wifi-guide,R01-v03, modify fw_binary folder to fw_pack folder

2. Tool version information and update:

​       2.1 download tool: v0.25.

​       2.2 pin-mux tool: v0.8. 

​       2.3 MP_Tool: v2.2.

3. Add IO state control before entering sleep mode.

4. Sync. AT+BLECONNPARAM to A2;

5. AT+SAVETRANSLINK set mode ERROR check.

6. Support massive and continuous parameters for AT command.


## Bug Fix List 

## Notes List 
1. Documents and Tools are inherited and updated from OPL1000-SDK MP1.8 



