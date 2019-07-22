# Function
This example shows how to complete OTA download firmware via WIFI path. 

# Work Principle

- OPL1000 implements wifi scan operation and try to connect "Opulinks-TEST-AP" once it is found.  
- Set up a "http server" on an existing machine and fill server IP and port information in project source code.  
- When execute this example OPL1000 call httpclient_connect  to connect http server.   
- Then use httpclient_send_request and httpclient_recv_response to obtain OTA image file from http server. 
- Received OTA image is saved into client_data.response_buf.

# Operation Flow

- Setup http server, place OTA image file (which need to update) under its "web" folder. Make sure it can be accessed by Opulinks-TEST-AP. 
- Compile this example project and generate m3 bin file. Pack it with m0 bin file which under FW_Binary folder. Now we have opl1000 firmware bin file (file name opl1000.bin). 
- Use download tool "OTA" function to pack opl1000 ota loader,  opl1000 firmware bin file and its header information to an OTA image file. Use download tool to download it into Devkit board. 
- Run http server service. Note its IP and port shall be same as that defined in example source code.  
- Run OPL1000  firmware by click reset button. Then OPL1000 will connected to Opulinks-TEST-AP, and then search http server, obtain OTA image file from server.      

# Notes

"Opulinks-TEST-AP" AP shall has ability to access http server.  

