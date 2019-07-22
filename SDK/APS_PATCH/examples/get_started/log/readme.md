# Function
This example shows how to enable/disable user APP and internal module (such as WIFI, BLE modules) log information output. 

This demo is designed based on WPA2_Station example. 

# Work Principle

- Two user app threads are created. One sends out message that taking counter information, the other one  sets log output configuration. 
- Thread1 send out "demo_app1: 1" , "demo_app1: 2" etc. log message every 2 seconds. These message can be shown on debug UART or not is determined by Thread2 operation.  
- In Thread2 a global variable "g_execution_count" is inceased. If it is  even number then both Thread1 app log and internal module log output are disabled. If  g_execution_count is odd number then  Thread1 app and internal module log message output is enabled. User will see log information on debug UART alternately.

# Notes
In order to monitor internal WIFI module log information, better open "Opulinks-TEST-AP" AP and then OPL1000 will connect to this AP.  

