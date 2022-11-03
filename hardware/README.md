<h1 align="center">
    OBDH 2.0 HARDWARE
    <br>
</h1>

<h4 align="center">On-board computer module hardware project (sources, outputs, and documentation).</h4>

<p align="center">
    <a href="https://github.com/spacelab-ufsc/spacelab#versioning">
        <img src="https://img.shields.io/badge/status-in%20development-red?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh2/releases">
        <img alt="GitHub release (latest by date)" src="https://img.shields.io/github/v/release/spacelab-ufsc/obdh2?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh2/releases">
        <img alt="GitHub commits since latest release (by date) for a branch" src="https://img.shields.io/github/commits-since/spacelab-ufsc/obdh2/latest/dev_hardware?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh2/commits/master">
        <img alt="GitHub last commit (branch)" src="https://img.shields.io/github/last-commit/spacelab-ufsc/obdh2/dev_hardware?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh2/issues">
        <img alt="GitHub last commit (branch)" src="https://img.shields.io/github/issues/spacelab-ufsc/obdh2/hardware?style=for-the-badge">
    </a>
    <a href="">
        <img src="https://img.shields.io/badge/CAD%20tool-altium%20v19.2-yellow?style=for-the-badge">
    </a>
    <a href="#license">
        <img src="https://img.shields.io/badge/license-cern%20ohl%202-yellow?style=for-the-badge">
    </a>
</p>

<p align="center">
    <a href="#overview">Overview</a> •
    <a href="#architecture">Architecture</a> •
    <a href="#development">Development</a> •
    <a href="#version">Version</a> •
    <a href="#license">License</a> •
    <a href="#notes">Notes</a>
</p>

<p align="center">
    <img width="45%" src="https://github.com/spacelab-ufsc/obdh2/blob/master/doc/user_manual/figures/obdh2-pcb-top.png">
    <img width="45%" src="https://github.com/spacelab-ufsc/obdh2/blob/master/doc/user_manual/figures/obdh2-pcb-bottom.png">
</p>


## Overview

The OBDH 2.0 board is composed of the following main components: MCU, non-volatile memory, voltage monitor with watchdog timer, TTC interface, EPS interface, payloads interface, daughterboard interface, I2C buffers, RS-485 transceiver, and current sensor. It is a FR-4 standard 2 layers PCB with a CubeSat form factor shape and size (90 x 93 mm).

## Architecture

The board design is a microcontroller based hardware connecting different interfaces and peripherals. For more information refer to the [documentation](https://github.com/spacelab-ufsc/obdh2/tree/master/doc) in the "Hardware" chapter.

<p align="center">
    <img width="70%" src="https://github.com/spacelab-ufsc/obdh2/blob/master/hardware/images/hardware_architecture_v05.png">
</p>

> This image refers to the v0.5 release.

## Development

#### Manufacture

The folder [fabrication](https://github.com/spacelab-ufsc/obdh2/tree/master/hardware/fabrication) contain 3 "ready to go" files: the gerbers and nc_drills for manufacturing the board, the BOM with all required components, and the pick_place file for automated assembly. Additional files are avaliable in the [outputs](https://github.com/spacelab-ufsc/obdh2/tree/master/hardware/outputs) folder, which contain several useful files and documents, such as: 3D models, bill of materials, schematics, layout prints, and draftsman.

#### Assembly

The board has components that should not be soldered simultaneously. Refer to the [documentation](https://github.com/spacelab-ufsc/obdh2/tree/master/doc) in the "Assembly" chapter.

#### Power-on procedure

The board requires external supply from the PC-104 connector or the JTAG programmer interface. The following diagram present a simplified diagram of the board power. Refer to the [documentation](https://github.com/spacelab-ufsc/obdh2/tree/master/doc) in the "Instructions" chapter.

<p align="center">
    <img width="70%" src="https://github.com/spacelab-ufsc/obdh2/blob/master/doc/user_manual/figures/power-diagram.png">
</p>

> This image refers to the v0.5 release.

#### Debugging

The debugging is performed through a serial UART port, using as default a baud rate of 115200 bps, 1 stop bit, and no parity bit. The interface uses a log system standard to improve readability. Refer to the [documentation](https://github.com/spacelab-ufsc/obdh2/tree/master/doc) in the "Instructions" chapter.

#### Testing

Automated testing is not avaliable yet.

## Version

Refer to the [releases](https://github.com/spacelab-ufsc/obdh2/releases) page.

## License

This repository containing hardware files is licensed under CERN Open Hardware License, version 2.

## Notes

Project under development!
