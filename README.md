<h1 align="center">
	ON-BOARD DATA HANDLING 2.0
	<br>
</h1>

<h4 align="center">On-board computer module designed and developed by SpaceLab.</h4>

<p align="center">
	<a href="https://github.com/spacelab-ufsc/spacelab#versioning">
		<img src="https://img.shields.io/badge/status-in%20development-red?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/obdh2/releases">
		<img alt="GitHub release (latest by date)" src="https://img.shields.io/github/v/release/spacelab-ufsc/obdh2?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/obdh2/releases">
		<img alt="GitHub commits since latest release (by date)" src="https://img.shields.io/github/commits-since/spacelab-ufsc/obdh2/latest?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/obdh2/commits/master">
		<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/spacelab-ufsc/obdh2?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/obdh2/issues">
		<img alt="GitHub issues" src="https://img.shields.io/github/issues/spacelab-ufsc/obdh2?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/obdh2/graphs/contributors">
		<img alt="GitHub contributors" src="https://img.shields.io/github/contributors/spacelab-ufsc/obdh2?color=yellow&style=for-the-badge">
	</a>
	<a href="#license">
		<img src="https://img.shields.io/badge/open--source-project-lightgray?style=for-the-badge">
	</a>
	<a href="https://github.com/spacelab-ufsc/obdh">
		<img src="https://img.shields.io/badge/flight-heritage-lightgray?style=for-the-badge">
	</a>
</p>

<p align="center">
  	<a href="#overview">Overview</a> •
  	<a href="#repository-organization">Repository Organizarion</a> •
  	<a href="#license">License</a> •
  	<a href="#releases">Releases</a> •
  	<a href="#notes">Notes</a>
</p>

<p align="center">
	<img width="70%" src="https://github.com/spacelab-ufsc/obdh2/blob/master/doc/figures/obdh2-pcb-3d.png">
</p>

## Overview

The SpaceLab OBDH2 (On-Board Data Handling 2.0) is one of the service modules developed for GOLDS-UFSC Cubesat Mission. The OBDH2 is responsible to synchronize actions and the data flow between other modules (ie. EPS, Payloads) and the Earth segment. It packs the generated data into data frames and transmit back to Earth through TTC module, or stores it on a non-volatile memory for later retrieval. Commands sent from Earth segment to the cubesat will be received by the radio transceivers located in the TTC module and redirected to the OBDH2, which takes the appropriate action or forward them to the responsible module.

## Repository Organization
	- doc: Technical documentation (including firmware, hardware, user guide, and datasheet).
	- firmware: On-board computer module firmware project (sources and configs)
	- hardware: On-board computer module hardware project (sources and outputs).

## License

This project is open-source under three different licenses: GNU General Public License v3.0 for firmware sources, CERN Open Hardware License v2.0 for hardware files, and CC BY-SA 4.0 for the documentation. Some third-part files and libraries are subjected to their specific terms and licenses.

## Releases

The OBDH2 software and hardware releases are synchronized in order to garantee compatibility. Then, using diferent versions might lead to unpredictable behavior. Refer to the [documentation](https://github.com/spacelab-ufsc/obdh2/tree/master/doc) for compatibility notes.

## Notes

More info about the OBDH2 target mission: [GOLDS-UFSC](https://github.com/spacelab-ufsc/golds-ufsc-doc) 

More info about the SpaceLab: [GitHub](https://github.com/spacelab-ufsc/spacelab) and [Website](https://spacelab.ufsc.br/en/home/)
