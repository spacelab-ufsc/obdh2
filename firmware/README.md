<h1 align="center">
	<br>
	OBDH2 FIRMWARE
	<br>
</h1>

<h4 align="center">On-board computer module firmware project (sources, configs and documentation).</h4>

<p align="center">
    <a href="https://github.com/spacelab-ufsc/spacelab#versioning">
        <img src="https://img.shields.io/badge/status-in%20development-red?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh2/releases">
        <img alt="GitHub release (latest by date)" src="https://img.shields.io/github/v/release/spacelab-ufsc/obdh2?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh2/blob/dev_firmware/firmware/version.h">
        <img alt="GitHub commits since latest release (by date) for a branch" src="https://img.shields.io/github/commits-since/spacelab-ufsc/obdh2/latest/dev_firmware?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh2/commits/master">
        <img alt="GitHub last commit (branch)" src="https://img.shields.io/github/last-commit/spacelab-ufsc/obdh2/dev_firmware?style=for-the-badge">
    </a>
    <a href="">
    	<img alt="GitHub issues by-label" src="https://img.shields.io/github/issues/spacelab-ufsc/obdh2/firmware?style=for-the-badge">
	</a>
    <a href="https://github.com/spacelab-ufsc/obdh2/blob/master/firmware/main.c">
        <img src="https://img.shields.io/badge/language-C-green?style=for-the-badge">
    </a>
    <a href="">
        <img src="https://img.shields.io/badge/IDE%20tool-CCS%20v9.0-yellow?style=for-the-badge">
    </a>
    <a href="#license">
        <img src="https://img.shields.io/badge/license-GPL3-yellow?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh">
        <img src="https://img.shields.io/badge/flight-heritage-lightgray?style=for-the-badge">
    </a>
</p>

<p align="center">
  	<a href="#overview">Overview</a> •
  	<a href="#architecture">Architecture</a> •
  	<a href="#dependencies">Dependencies</a> •
  	<a href="#development">Development</a> •
  	<a href="#version">Version</a> •
  	<a href="#license">License</a> •
  	<a href="#notes">Notes</a>
</p>

<br>

## Overview

The OBDH2 board has a MSP430 low-power microcontroller that runs the firmware application. It manages the others sub-modules within the board, synchronizes actions, handles communication with the Earth's segment, and manages the data flow. The programming language used is C and firmware was developed in the Code Composer Studio IDE (a.k.a. CCS). 

## Architecture

The OBDH2 module has many tasks, such interfacing peripherals and other MCUs, over distinct protocols and time requirements. Then, it requires a Real Time Operating System (RTOS) to assure that it will deal with its deadlines, even under a fault in some task routine. The RTOS chosen is the FreeRTOS (v10.0.0), since it is designed for embedded systems applications and it was already validated in space applications. The software was made to have abstraction layers through the use of low-level register manipulation libraries, drivers to manage internal and external peripherals, devices to handle ICs and other modules, and over the top a application layer. Refer to the [documentation](https://github.com/spacelab-ufsc/obdh2/tree/master/doc) in the "Firmware" chapter. 

## Dependencies
	- MSP430 DriverLib v2.91.11.01
	- FreeRTOS v10.2.1
	- NGHam protocol
	- libcsp v1.5.16
	- FatFS R0.14
	- Reliance Edge v2.4
	- FreeMODBUS v1.6
	- JSMN v1.1.0

## Development

#### Toolchain setup

This tutorial is a reference to install and setup the Code Composer Studio (CCS), a suggested tool for the development and deployment of the OBDH2 firmware. The CCSv9.0 (or later) can be downloaded [here](http://software-dl.ti.com/ccs/esd/documents/ccs_downloads.html). After downloaded, start the install process. For the OBDH2, only the `MSP430 Ultra Low Power MCUs` package is required. More instructions for Linux environment, click [here](https://software-dl.ti.com/ccs/esd/documents/ccs_linux_host_support.html). 

#### Compiling and building
	
Clone [this repository](https://github.com/spacelab-ufsc/obdh2.git). After this, import the source project into CCS:
`File -> Import -> Code Composer Studio -> CCS Projects -> Select the source folder: obdh/firmware -> Leave 'Copy projects into workspace' unchecked / Finish`. Then, click in the Build button or press "Ctrl+B" to compile and build the sources. 

> Alternatively, it is possible to use the available makefile to import, compile and build the firmware in the linux environment. It is necessary to update the directories to correspond to those in the running machine, hit "make import" in the linux terminal and hit "make".  

#### Debugging

Before deploying the program into the microcontroller via the JTAG interface, follow the [power-on procedures](https://github.com/spacelab-ufsc/obdh2/tree/dev_hardware/hardware#power-on-procedure). Using the [MSP-FET](http://www.ti.com/tool/MSP-FET) is possible to program the microcontroller and start a debug session, which allows full control of the firmware in execution. For that, with the project opened in the CCS, click in the "Debug" button or press "F11" to upload the code into the OBDH2 board.

Besides the JTAG, there is the debugging performed through a serial UART port, using as default a baud rate of 115200, 1 stop bit, and no parity bit. The interface uses a log system standard to improve readability. Refer to the [documentation](https://github.com/spacelab-ufsc/obdh2/tree/master/doc) in the "Instructions" chapter.

> Alternatively, it is possible to use the available makefile to perform the deployment of the firmware image into the microcontroller in the linux environment using the MSP-FET. It is necessary to follow the power-on procedures, connect the programmer and hit "make flash" in the terminal.  

## Version

Refer to the [releases](https://github.com/spacelab-ufsc/obdh2/releases) page and the [version](https://github.com/spacelab-ufsc/obdh2/blob/master/firmware/version.h) file.

## License

This repository containing firmware files is licensed under GPLv3 license.

## Notes

Project under development!