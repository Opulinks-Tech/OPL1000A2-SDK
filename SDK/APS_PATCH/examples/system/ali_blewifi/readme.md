# Function
This example shows how to use OPL1000 connect to Ali Cloud via MQTT protocol. 

- For "BLE config WIFI" we can either use opulinks_iot_app (Android APP), OBW APP (IOS APP) that placed under "Demo\BLE_Config_AP" or Ali 云智能 APP to complete this function. 
- This example shows how to establish MQTT connection between OPL1000 and Ali cloud.  

# Work Flow

1. After firmware is downloaded, OPL1000 device will broadcast advertising message sostenuto 
2. Use mobile APP to scan OPL1000 device and complete BLE connection, WIFI  AP connection. 
3. Firmware will connect to Ali cloud automatically.



# Notes

1. In order to ensure Ali cloud connection, "Ali quintile" shall be applied and filled in below macro-definition that defined in blewifi_configuration.h 

   #define ALI_PRODUCT_ID              (0000000)
   #define ALI_PRODUCT_KEY             "xxxxxxxxxxx"
   #define ALI_PRODUCT_SECRET          "xxxxxxxxxxx" 
   #define ALI_DEVICE_NAME             "xxxxxxxxxxx"
   #define ALI_DEVICE_SECRET           "xxxxxxxxxxx"

2. After "Ali quintile" is updated,  FIM version "MW_FIM_VER12_PROJECT" number shall be increased 1 to enable above parameter change effective. 

   #define MW_FIM_VER12_PROJECT        0x02   // 0x00 ~ 0xFF

3. Refer Demo\BLE_Config_AP\OPL1000-Demo-BLE-setup-network-guide.pdf to know how to use Android App opulinks_iot_app or IOS OBW application to complete WIFI connection. 


