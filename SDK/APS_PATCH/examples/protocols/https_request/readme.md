# Function
This example shows how to use mbedtls module to realize https request function. 

# Work Principle

- OPL1000 implements scan operation and try to connect "Opulinks-TEST-AP" once it is found.  
- OPL1000 tried to establish TLS connection with "www.howsmyssl.com" server by calling mbedtls module API.   
- Once connection is successful, then OPL1000 send out https request and get response from www.howsmyssl.com

# Notes
"Opulinks-TEST-AP" AP shall has ability to access internet. 

