# Function
This example introduces how to set  IO peripheral as I2C master port. I2C port singal and work mode are defined in OPL1000_pin_mux_define.c file. 

In this demo IO10 and IO11 are configured as I2C clock and data signal. I2C address is 7bit width.

As I2C master it can communicate with I2C slave device  uniaxially or receive I2C slave device sending data. In this project "accessing an I2C EEPROM"  and "write/read with an external I2C slave device" examples are all given.  

# Notes
- OPL1000_pin_mux_define.c defines I2C port property . It is created by PinMux tool. 
- PinMux tool is provided under Tool\PinMux folder. 
- Refer Doc\OPL1000-pinmux-tool-user-guide.pdf to know how to use PinMux tool. 
- An external I2C slave device is required to connect when test "data exchange" function. 