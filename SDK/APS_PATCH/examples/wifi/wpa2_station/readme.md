# Function
This example shows OPL1000 WIFI basic function: how to do scan operatio and connected to a WPA2 AP. 

# Work Principle

- OPL1000 implements scan operation and get scaned AP list. 
- OPL1000 connects "Opulinks-TEST-AP" once it is in AP list.  

# Notes
-  WPA2 AP SSID and password is defined in wpa2_station_app.h
-  Make sure  "Opulinks-TEST-AP" AP is open when run this demo.
-  Using mixed mode (passive and active together) to scan AP.  