# Project_I2C
I2C code

Current Project Goal:  Communicate over I2C between the MSP430 and the LIS3DH accelerometer.
- We want to read the status register in the accelerometer to determine if new data has been acquired.
- If new data is acquired, we will read all of the new data from the accelerometer.

Project Files:
- TimersB1/B2/B3.c are currently not in use, and the code currently in those files has no correlation to this project.
- Clocks.c is currently not in use and is left over from a previous project.

Work Environment:
- We have been working on this code in the IAR Embedded Workbench IDE.

Problems encountered log:
1. Start bit is not being transmitted MSP430 to accelerometer.  As result, code gets stuck waiting for transmission.
  a. Further investigation led us to discover that the SDA Line is never high when the program is running.
  b. the SCL line is high at the start of the program, but gets stuck low once the code gets stuck.
