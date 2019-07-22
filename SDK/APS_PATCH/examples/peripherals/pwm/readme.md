# Function
This example introduces how to define IO peripheral as PWM port.  

In this demo IO19 and IO18 are configured as PWM port and ouput different frequency waveform. 
OPL1000 supports two configuration methods, one is simple mode, the other one is complex mode.  

# Notes
- All PWM ports shall use same clock source. 
- PWM work mode and pin resource allocation can be set by PinMux tool. It is provided under Tool\PinMux folder. Currently only simple mode setting is supported by tool. 
- Refer Doc\OPL1000-pinmux-tool-user-guide.pdf to know how to use PinMux tool. 
- Refer Doc\OPL1000_peripheral_PWM_application_notes.pdf to know more detailed application notes. 