SimpleRtos
==========
This is a very simple RTOS for stm32f4 devices. I wrote this after taking Real Time Bluetooth Networks course on EdX by Prof. J Valvano.

## Building
Use this link https://gnu-mcu-eclipse.github.io/ to download and setup toolchain, openocd and Eclipse for stm32f4 development.

To build from commandline use following steps.
1. Setup toolchain in your Path. 
  `export PATH=<TOOLCHAIN_PATH>:$PATH`
2. Goto Main Direcotry. 
  `cd SimpleROTS`
3. Build
  `make`
 
To build and Debug from Eclipse IDE:
1. Export as Makefile project.
2. Follow the steps mentioned in the ARM guide to build and Debug application.

## Sample Application
The sample applicaion demostrates basic API usage. It contains two threads both will toggle different LEDs and yield the CPU. 




