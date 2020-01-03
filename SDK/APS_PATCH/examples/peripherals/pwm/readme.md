# Function
This example introduces how to define IO peripherals as PWM ports.  

OPL1000 supports two configuration methods, one is simple mode, the other one is complex mode.

In this demo, IO19 and IO18 are configured as PWM ports in simple mode and output different frequency waveform(Duty Ratio is 20% and 50% respectively), while IO20 is configured as PWM port in complex mode. The Duty Ratio of this PWM port would be switched between 20% and 80% regularly.
  

# Notes
- All PWM ports should use same clock source. 
- PWM work mode and pin resource allocation can be set by PinMux tool. It is provided under Tool\PinMux folder. Currently only simple mode setting is supported by tool. 
- Refer Doc\OPL1000-pinmux-tool-user-guide.pdf to know how to use PinMux tool. 
- Refer Doc\OPL1000_peripheral_PWM_application_notes.pdf to know more detailed application notes. 