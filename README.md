# Project_I2C
Project changed over to Seeeduino using SPI for usability and increased datarate.

Current Project Goal:  ~Communicate over I2C between the MSP430 and the LIS3DH accelerometer.~
- ~We want to read the status register in the accelerometer to determine if new data has been acquired.~
- ~If new data is acquired, we will read all of the new data from the accelerometer.~
- Fix code upload issue
- Tune threshold for bearing failure

Project Files:
- TimersB1/B2/B3.c are currently not in use, and the code currently in those files has no correlation to this project.
- Clocks.c is currently not in use and is left over from a previous project.
- MSP430 retired for this project.
- Arduino_LIS3DH contains working data collection, FFT transform, and failed bearing detection algorithm based on running average of amplitudes past 2KHz with LEDs triggering past a threshold value determined by Excel data analysis.
Work Environment:
- ~We have been working on this code in the IAR Embedded Workbench IDE.~
- ArduinoIDE2.0.0-rc5

Problems encountered log:
1. Start bit is not being transmitted MSP430 to accelerometer.  As result, code gets stuck waiting for transmission.
  
  a. Further investigation led us to discover that the SDA Line is never high when the program is running.
  
  b. the SCL line is high at the start of the program, but gets stuck low once the code gets stuck.
2. Complications with MSP430 and approacing deadlines, switched to Arduino Pro Mini for faster integration.
3. Exceded Arduino Pro Mini's onboard memory with Digital FFT calculations, switched to Seeeduino to support 128 sample FFT of acceleration data from LIS3DH
4. Seeeduino becomes unrecognied and code upload fails every other time.
  a. Temporarily solved with double tapping rst pads on Seeeduino to enter bootloader mode.
  
 
