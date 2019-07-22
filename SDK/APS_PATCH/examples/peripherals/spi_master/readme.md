# Function
This example introduces how to define IO peripheral as SPI master port. OPL1000 only supports SPI master mode.     

OPL1000 supports two SPIs port definition, SPI1 and SPI2. They can be 4 signal simple mode or 6 signal quad mode. SPI0 is reserved to access SPI flash device. 

In this example IO2~IO5 4 pins are defined as SPI2 port simple mode. 

In this Demo SPI0 access flash and SPI2 communicate with external SPI slave device examples are provided.  

# Notes
- SP pin selection and work mode setting are defined by PinMux tool. 
- PinMux tool is provided under Tool\PinMux folder.
- Refer Doc\OPL1000-pinmux-tool-user-guide.pdf to know how to use PinMux tool. 
- Refer Doc\OPL1000-Flash-User-Guide.pdf to know how to program on board SPI flash. 