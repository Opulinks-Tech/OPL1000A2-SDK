# Function
This example introduces how to use GPIO peripheral. GPIO resource and their property are defined in OPL1000_pin_mux_define.c file. 

In this demo IO2 and IO3 are configured as GPO (General Purpose Output) pin, and their level are set are high and low alternately.
IO23 (connected to D1 LED) is configured as GPO pin, it is light up and turn off alternately.

# Notes
- OPL1000_pin_mux_define.c defines GPIO port property . It is created by PinMux tool. 
- PinMux tool is provided under Tool\PinMux folder. 
- Refer Doc\OPL1000-pinmux-tool-user-guide.pdf to know how to use PinMux tool. 