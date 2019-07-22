# Function
This example shows how to connect to Aliyun with keys and use embedded security methods on ROM. 

This demo is based on Ali Cloud linkkit SDK . 

# Work Principle

- OPL1000 implements scan operation and try to connect "Opulinks-TEST-AP" once it is found.  
- OPL1000 tried to establish connection with Ali Cloud.   
- Once connection is successful, then OPL1000 loop for sometime to handle events with registered callback functions.

# Notes
"Opulinks-TEST-AP" AP should has ability to access internet. 

