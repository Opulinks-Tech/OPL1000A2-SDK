# Function
This example shows thread create and message exchaning work principle.

# Work Flow

- A message queue is created. 
- Two threads are created. Thread1 sends message , the other one Thread2 receives message.
- A counter is inceasing  in Thread1 and its value is send to message queue. Thread2 received message , get counter's value and print it out.  

# Notes
1. Thread and message working mechanism is based on CMSIS RTOS.  

