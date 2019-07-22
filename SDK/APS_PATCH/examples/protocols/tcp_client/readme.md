# Function
This example shows how to establish TCP connection in local network. In this demo OPL1000 acts as TCP client, PC runs "NetAssist.exe" program  acting as TCP server. 

OPL1000 calls lwip socket API to establish TCP connection. 

# Work Principle

- OPL1000 implements scan operation and try to connect "Opulinks-TEST-AP" once it is found.  
- PC side runs "NetAssist.exe" program and start TCP server service. Notes PC shall also join up "Opulinks-TEST-AP" AP. Then both PC and OPL1000 are in one same local network. 
- OPL1000 setup TCP connection with PC. And then send "Hello from OPL1000"  to server. 
- PC receives "Hello from OPL1000" , send back an ACK message "ACK from Server "
- OPL1000 side receives "ACK from Server " message from server. 

# Notes
-  In  tc_client.h TCP server IP address and port shall be same as  "NetAssist.exe" program setting. 
-  Refer Demo\TCP_Client\OPL1000-Demo-tcp-client-guide.pdf to know detailed information.   