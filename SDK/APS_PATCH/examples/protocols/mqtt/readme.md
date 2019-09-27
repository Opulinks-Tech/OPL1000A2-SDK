# Function
This example shows how to realize MQTT connection to a local network MQTT broker. 

- For "BLE config WIFI" we can either use opulinks_iot_app (Android APP) or OBW APP (IOS APP) that placed under "Demo\BLE_Config_AP" 
- User can establish  private MQTT connection based on this example easily. 

# Work Flow

1. After firmware is downloaded, OPL1000 device will broadcast advertising message sostenuto 
2. Use mobile APP to scan OPL1000 device and complete BLE connection, WIFI  AP connection
3. Firmware will connected to  AWS cloud automatically and publish topic.    



# Notes

1. When user change any parameters in blewifi_configuration.h,  FIM version "MW_FIM_VER11_PROJECT " shall be increased with 1 to ensure the change is effective. 

   #define MW_FIM_VER11_PROJECT 0x03    // 0x00 ~ 0xFF

2. Refer Demo\MQTT\OOPL1000-Demo-MQTT-guide.pdf to know detailed processing flow. 


