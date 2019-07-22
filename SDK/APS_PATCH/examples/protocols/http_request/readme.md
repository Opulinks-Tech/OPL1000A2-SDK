# Function
This example shows how to use socket function which provided by lwip module to do http request function. 

This demo is based on CMSIS RTOS. 

# Work Principle

- OPL1000 implements scan operation and try to connect "Opulinks-TEST-AP" once it is found.  
- OPL1000 tried to establish TCP socket connection with "example.com" server.   
- Once connection is successful, then OPL1000 send out request and capture web page from "http://example.com/"

# Notes
"Opulinks-TEST-AP" AP shall has ability to access internet. 

