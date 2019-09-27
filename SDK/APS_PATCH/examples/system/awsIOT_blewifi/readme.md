# Function
This example shows how to use OPL1000 to connect AWS cloud 

- For "BLE config WIFI" we can either use opulinks_iot_app (Android APP), OBW APP (IOS APP) that placed under "Demo\BLE_Config_AP" folder.  
- This example shows how to establish MQTT connection between OPL1000 and AWS cloud. 

# Work Flow

1. After firmware is downloaded, OPL1000 device will broadcast advertising message sostenuto 
2. Use mobile APP to scan OPL1000 device and complete BLE connection, WIFI  AP connection. 
3. Firmware will connect to AWS cloud automatically.



# Notes

1. When user change any parameters in blewifi_configuration.h,  FIM version "MW_FIM_VER08_PROJECT" shall be increased with 1 to ensure the change is effective. 

   #define MW_FIM_VER08_PROJECT            0x03    // 0x00 ~ 0xFF

2. Refer Demo\BLE_Config_AP\OPL1000-Demo-BLE-setup-network-guide.pdf to know detailed processing flow.

3. Refer Doc\OPL1000-BLEWIFI-Application-Dev-Guide.pdf  to know BLEWIFI example working principle. 


