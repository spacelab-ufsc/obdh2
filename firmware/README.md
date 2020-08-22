<h1 align="center">
	<br>
	OBDH2 FIRMWARE
	<br>
</h1>

<h4 align="center">On-board computer module firmware project (sources, configs and documentation).</h4>

<p align="center">
	<a href="">
		<img src="https://img.shields.io/badge/status-development-green?style=for-the-badge">
	</a>
    <a href="">
		<img src="https://img.shields.io/badge/stable%20version-0.5-blue?style=for-the-badge">
	</a>
    <a href="">
		<img src="https://img.shields.io/badge/language-c-9cf?style=for-the-badge">
	</a>
	<a href="">
		<img src="https://img.shields.io/badge/IDE%20tool-CCS%20v9.0-yellow?style=for-the-badge">
	</a>
	<a href="">
		<img src="https://img.shields.io/badge/license-GPL3-red?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/obdh2/tree/dev/doc/build">
		<img src="https://img.shields.io/badge/for%20more-here-lightgray?style=for-the-badge">
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

The OBDH2 module has many tasks, such interfacing peripherals and other MCUs, over distinct protocols and time requirements. Then, it requires a Real Time Operating System (RTOS) to assure that it will deal with its deadlines, even under a fault in some task routine. The RTOS chosen is the FreeRTOS (v10.0.0), since it is designed for embedded systems applications and it was already validated in space applications. The software was made to have abstraction layers through the use drivers to manage internal peripherals, such as GPIO, watchdog timers, timers, clocks, USCIs and ADCs. The following diagram presents this scheme.

#### Firmware abstraction layers diagram
<p align="center">
	<img width="70%" src="https://github.com/spacelab-ufsc/obdh2/blob/master/doc/figures/system_layers.pdf">
</p>


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

#### Toolchain setup:

#### Compiling and building:
	
#### Debugging

## Version

## License

## Notes
Under Development! More information soon.

