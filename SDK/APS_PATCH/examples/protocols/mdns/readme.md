# Function
This example shows how to use mdns API functions which provided by lwip module to setup mDNS service broadcast. 

# Work Principle

- OPL1000 implements scan operation and try to connect "Opulinks-TEST-AP" once it is found.  
- OPL1000 call mdns related APIs to establish mDNS service. And broadcast it in local network.
- Other devices which support mDNS service can query avaliable mDNS service in this local network.

# Notes
- Currently lwip 2.0.3 only provides APIs that support service broadcast function. 
- User can install Bonjour to query OPL1000 mDNS service or use "zeroconf" tool to inquiring local network mDNS services. 