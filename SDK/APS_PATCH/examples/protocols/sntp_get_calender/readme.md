# Function
This example shows how to implement SNTP function by calling lwip API functions. 

# Work Principle

- OPL1000 implements scan operation and try to connect "Opulinks-TEST-AP" once it is found.  
- Once AP connection is established, OPL1000  will call socket API to connecte SNTP_SERVER "1.cn.pool.ntp.org"
- Send SNTP request and then receive response information from SNTP_SERVER.

# Notes
-  "Opulinks-TEST-AP" AP shall has ability to access internet.  