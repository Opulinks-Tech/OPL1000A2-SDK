# Function
This example shows the procedure of OPL1000 using BLE to configure WIFI AP and then connect to specified AP

- It needs to co-work with Opulinks dedicate mobile APP "OPL1000 netstrip" 
- Use this feature OPL1000 can log in network and connected to internet easily 

# Work Flow

1. OPL1000 broadcast advertising message sostenuto 
2. open "OPL1000 netstrip" APP on mobile , it will scan OPL1000 device automatically. 
3. Once OPL1000 device (MAC address shall be default value 11:22:33:44:55:66) is found, app will ask OPL1000 to do scan operation.
4. OPL1000 send scan result to APP program and disply them on mobile. 
5. User choose one AP and enter password on mobile. 
6. OPL1000 will connect to specified AP accordingly.     



# Notes

1. When user change any parameters in blewifi_configuration.h,  FIM version "MW_FIM_VER11_PROJECT" shall be increased with 1 to ensure the change is effective. 

   #define MW_FIM_VER11_PROJECT            0x03    // 0x00 ~ 0xFF

2. Refer Demo\BLE_Config_AP\OPL1000-Demo-BLE-setup-network-guide.pdf to know detailed processing flow.

3. Refer Doc\OPL1000-BLEWIFI-Application-Dev-Guide.pdf  to know BLEWIFI example working principle. 


