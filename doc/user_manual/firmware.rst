.. firmware.rst

   Copyright The OBDH 2.0 Contributors.

   OBDH 2.0 Documentation

   This work is licensed under the Creative Commons Attribution-ShareAlike 4.0
   International License. To view a copy of this license,
   visit http://creativecommons.org/licenses/by-sa/4.0/.

.. _ch:firmware:

Firmware
========

Product tree
------------

The product tree of the firmware part of the OBDH 2.0 module is available in :numref:`fig:product-tree-fw`.

.. figure:: img/product-tree-fw.*
   :align: center
   :name: fig:product-tree-fw

   Product tree of the firmware of the OBDH 2.0 module.

Dependencies
------------

The firmware depends on external libraries to access the embedded hardware or to communicate with other modules. A list of these libraries and the used version is available in :numref:`tab:fw-dependencies`.

.. container:: compacttable60

   .. table:: External libraries and dependencies of the firmware.
      :name: tab:fw-dependencies
      :widths: 70 30

      ================ ===========
      **Library**      **Version**
      ================ ===========
      MSP430 DriverLib v2.91.11.01
      FreeRTOS         v10.2.1
      ================ ===========

Tasks
-----

A list of the firmware tasks can be seen in the :numref:`tab:firmware-tasks`. A complete description of them is provided below.

.. container::

   .. table:: Firmware tasks.
      :name: tab:firmware-tasks
      :widths: 30 14 18 18 20

      +------------------------+--------------+------------------------+-----------------+-------------------+
      | **Name**               | **Priority** | **Initial delay [ms]** | **Period [ms]** | **Stack [bytes]** |
      +========================+==============+========================+=================+===================+
      | Antenna deployment     | Highest      | 0                      | Aperiodic       | 150               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Antenna reading        | Medium       | 5000                   | 60000           | 150               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Data log               | Medium       | 3005000                | 600000          | 225               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | General Telemetry      | High         | 15000                  | 60000           | 225               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | EDC reading            | Medium       | 5000                   | 60000           | 300               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | EPS reading            | Medium       | 5000                   | 60000           | 384               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Heartbeat              | Lowest       | 2000                   | 500             | 160               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Housekeeping           | Medium       | 5000                   | 60000           | 225               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Mission Manager        | High         | 5000                   | Aperiodic       | 512               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Payload X reading      | Medium       | 5000                   | 60000           | 300               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Position Determination | Low          | 3000                   | 60000           | 1024              |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Read sensors           | Medium       | 5000                   | 60000           | 140               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Startup (boot)         | Highest      | 0                      | Aperiodic       | 350               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | System reset           | Medium       | 0                      | 36000000        | 128               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Telecommand processing | High         | 16500                  | 1000            | 1024              |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Time control           | High         | 1000                   | 1000            | 128               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | TTC reading            | Medium       | 15500                  | 60000           | 384               |
      +------------------------+--------------+------------------------+-----------------+-------------------+
      | Watchdog reset         | Lowest       | 0                      | 100             | 150               |
      +------------------------+--------------+------------------------+-----------------+-------------------+

Antenna deployment
~~~~~~~~~~~~~~~~~~

This task deploys the Antenna module at the start of the mission. It basically implements the previously shown antenna deploy routine flowchart in :numref:`fig:flowchart_antenna_deploy_routine`.

Antenna reading
~~~~~~~~~~~~~~~

This task initializes the antenna device and reads the antenna housekeeping data, which includes deployment status. It also stores the reading timestamp together with housekeeping data.

.. _sec:data-log-task:

Data log
~~~~~~~~

This task saves the all subsystems last read housekeeping data to flash memory. Since there is no proper file system to manage the memories each subsystem got a predetermined number of flash pages to write in a ring buffer fashion. The address and page numbers for each subsystem can be seen in `firmware/config/config.h <https://github.com/spacelab-ufsc/obdh2/blob/master/firmware/config/config.h>`__ from :cite:`obdh2-repo`.

General Telemetry
~~~~~~~~~~~~~~~~~

The General Telemetry task transmits a data package containing the satellite’s basic telemetry data every 60 seconds. The complete content of the packet can be seen at :numref:`tab:downlink-packets`. The transmission is controlled through a flag on the OBDH parameters (see :numref:`tab:vars-and-pars`), accessible through telecommands.

EDC reading
~~~~~~~~~~~

This task reads the state and housekeeping EDC frames, if there are available PTT packets on EDC’s FIFO, it also attempts to read them. After every sucessfull PTT packet read the data is stored in flash memory, following the same scheme described in :ref:`sec:data-log-task`. A flowchart of how the data collection procedure works can be seen in :numref:`fig:edc-reading-flowchart`.

.. figure:: img/edc_reading_flowchart.*
   :align: center
   :name: fig:edc-reading-flowchart

   EDC reading procedure flowchart.

EPS reading
~~~~~~~~~~~

This task initializes the EPS device and reads the EPS housekeeping data. The communication with EPS has a simple retry scheme, basically the task will attempt to resend the command five times, after five consecutive errors it goes to the next operation.

Heartbeat
~~~~~~~~~

The heartbeat task keeps blinking a LED (“*System LED*” in :numref:`fig:status-leds`) at a rate of 1 Hz during the execution of the system. Its purpose is to give visual feedback on the execution of the scheduler. This task does not have a specific purpose on the flight version of the module (the flight version of the PCB does not have LEDs).

Housekeeping
~~~~~~~~~~~~

This task saves the OBDH data parameters to FRAM every 60 seconds. It also keep track of hibernation duration and timeouts, notifying the Mission Manager task in case of a timeout.

Mission Manager
~~~~~~~~~~~~~~~

This task controls all mission specific behavior, specially payload control. After task creation Mission Manager will attempt to restore the satellite state from the previous boot through the OBDH parameters, making sure the operation is consistent through boots. The task has an Event Queue implemented using the FreeRTOS Queue, in which the task blocks waiting for event notifications, this way the task does not waste any compute time while idle. The implemented events can be seen in :numref:`tab:mission-man-events`.

.. container::

   .. table:: Mission Manager events.
      :name: tab:mission-man-events
      :widths: 28 62 10

      +-------------------------+-------------------------------------------------------+--------+
      | **Name**                | **Description**                                       | **ID** |
      +=========================+=======================================================+========+
      | In Brazil               | Satellite’s position is inside brazilian territory    | 0      |
      +-------------------------+-------------------------------------------------------+--------+
      | Out of Brazil           | Satellite got out of brazilian territory              | 1      |
      +-------------------------+-------------------------------------------------------+--------+
      | PX Finished             | Payload X finished its experiment                     | 2      |
      +-------------------------+-------------------------------------------------------+--------+
      | Mode Change Req.        | Received a mode change request via telecommand        | 3      |
      +-------------------------+-------------------------------------------------------+--------+
      | Activate Payload Req.   | Received an activate payload request via telecommand  | 4      |
      +-------------------------+-------------------------------------------------------+--------+
      | Deactivate Payload Req. | Received a deactivate payload request via telecommand | 5      |
      +-------------------------+-------------------------------------------------------+--------+

Payload X reading
~~~~~~~~~~~~~~~~~

This task reads Payload X experiment data and status. It only attempts to read data from Payload X if it is active and set as the secondary payload (see :numref:`tab:vars-and-pars`).

Position Determination
~~~~~~~~~~~~~~~~~~~~~~

This task determines the satellite position based on the satellite’s TLE lines using a library that implements a SGP4 propagator. The TLE lines are provided through telecommands and are expected to by update quite regularly. The task also deals with TLE updates, repopulating the orbital elements and SGP4 variables whenever there is an update.

Read sensors
~~~~~~~~~~~~

This task reads the internal sensors of the OBDH. The available readings are: :math:`\mu`\ C temperature, :math:`\mu`\ C current supply and :math:`\mu`\ C voltage supply. The task also updates the sensor reading timestamp parameter.

Startup (boot)
~~~~~~~~~~~~~~

This task is the first executed task when the system starts. All devices, libraries, and data structures are initialized in this task. When the execution is done, the remaining tasks of the system are allowed to execute.

System reset
~~~~~~~~~~~~

This task resets the microcontroller by software every 10 hours. This can be useful to clean up possible wrong values in variables, repeat the antenna deployment routine (limited to :math:`n` times), clean up the RAM, etc.

Telecommand processing
~~~~~~~~~~~~~~~~~~~~~~

This task processes all the telecommand packets received by the TTC device. The supported telemetry and telecommand packets, for both uplink and downlink, can be seen in :numref:`tab:packets-struct`. Also, a description of each telecommand is provided in :ref:`anx:packets`.

Time control
~~~~~~~~~~~~

This task is responsible for the time management of the system. At every second, it increments the system time (epoch). Also, it saves the current system time in the non-volatile memory every minute.

TTC reading
~~~~~~~~~~~

This task initializes the TTC device and reads the TTC housekeeping data. Also, it checks for the number of consecutive decoding errors on TTC, if there are more than 5 consecutive errors the task tries to reset the TTC device. This is done to avoid potential issues, such as the radio being stuck in TX mode or memory corruption.

Watchdog reset
~~~~~~~~~~~~~~

This task resets the internal and external watchdog timer every 100 ms. The internal watchdog has a maximum count time of 500 ms, and the external watchdog has a maximum of 1600 ms (see :ref:`ch:hardware` for more information about the watchdog timers).

To prevent the system to not reset during an anomaly on some task (like an execution time longer than planned), this task has the lowest possible priority: 0.

Variables and Parameters
------------------------

The internal variables and parameters of the OBDH firmware can be seen in :numref:`tab:vars-and-pars`.

.. container::

   .. table:: Variables and parameters of the OBDH 2.0.
      :name: tab:vars-and-pars
      :widths: 8 70 12 10

      +--------+---------------------------------------------------------------------------+-----------+------------+
      | **ID** | **Name/Description**                                                      | **Type**  | **Access** |
      +========+===========================================================================+===========+============+
      | 0      | System time in sec. (Unix epoch)                                          | uint32    | R/W        |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 1      | Temperature of the :math:`\mu`\ C in Kelvin                               | uint16    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 2      | Input current in mA                                                       | uint16    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 3      | Input voltage in mV                                                       | uint16    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 4      | Last reset cause:                                                         | uint8     | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x00 = No interrupt pending                                            |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x02 = Brownout (BOR)                                                  |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x04 = RST/NMI (BOR)                                                   |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x06 = PMMSWBOR (BOR)                                                  |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x08 = Wakeup from LPMx.5 (BOR)                                        |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x0A = Security violation (BOR)                                        |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x0C = SVSL (POR)                                                      |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x0E = SVSH (POR)                                                      |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x10 = SVML_OVP (POR)                                                  |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x12 = SVMH_OVP (POR)                                                  |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x14 = PMMSWPOR (POR)                                                  |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x16 = WDT time out (PUC)                                              |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x18 = WDT password violation (PUC)                                    |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x1A = Flash password violation (PUC)                                  |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x1C = Reserved                                                        |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x1E = PERF peripheral/configuration area fetch (PUC)                  |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x20 = PMM password violation (PUC)                                    |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      |        | \- 0x22 to 0x3E = Reserved                                                |           |            |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 5      | Reset counter                                                             | uint16    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 6      | Last valid telecommand (uplink packet ID)                                 | uint8     | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 7      | Hardware version                                                          | uint8     | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 8      | Firmware version (ex.: “v1.2.3” = 0x00010203)                             | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 9      | Mode (“Normal” = 0, “Hibernation” = 1, “Stand-by” = 2)                    | uint8     | R/W        |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 10     | Timestamp of the last mode change                                         | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 11     | Mode duration in sec.                                                     | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 12     | Initial hibernation executed                                              | boolean   | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 13     | Initial hibernation time counter (minutes)                                | uint8     | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 14     | Antenna deployment executed                                               | boolean   | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 15     | Antenna deployment counter                                                | uint8     | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 16     | Satellite’s latitude in degress                                           | int16     | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 17     | Satellite’s longitude in degress                                          | int16     | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 18     | Satellite’s altitude in kilometers                                        | int16     | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 19     | Last written flash page in OBDH sector                                    | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 20     | Last written flash page in EPS sector                                     | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 21     | Last written flash page in TTC 0 sector                                   | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 22     | Last written flash page in TTC 1 sector                                   | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 23     | Last written flash page in Antenna sector                                 | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 24     | Last written flash page in EDC sector                                     | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 25     | Last written flash page in Payload X sector                               | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 26     | Last written flash page in SBCD packets sector                            | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 27     | Manual mode enabled                                                       | boolean   | R/W        |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 28     | Main EDC ID (see the system IDs table)                                    | uint8     | R/W        |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 29     | General telemetry enabled                                                 | boolean   | R/W        |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 30     | Reset device (Resets OBDH when “01h” is written into it)                  | boolean   | W          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 31     | Timestamp of the last TLE line set update in sec.                         | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 32     | Timestamp of the last OBDH sensor’s reading in sec.                       | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 33     | Main payload state (Active payload ID or 0 if disabled)                   | uint32    | R/W        |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 34     | Secondary payload state (Active payload ID or 0 if disabled)              | uint32    | R/W        |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 35     | Remaining hibernation time in sec.                                        | uint32    | R/W        |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 36     | Binary format TLE line                                                    | uint8[50] | W          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 37     | Timestamp used in the last position determination in sec.                 | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+
      | 38     | Timestamp from the last telecommand reception in sec.                     | uint32    | R          |
      +--------+---------------------------------------------------------------------------+-----------+------------+

Telemetry
---------

All telemetry data available to downloaded from OBDH is expected to be serialized in big endian ordering, meaning that a data field named “param” with type *uint16*, is serialized as the first byte being the 8 most significant bits of “param” and the second byte is the 8 least significant bits of it. This behavior happens to all data types with length bigger than 1 byte.

The :ref:`anx:telemetry-apx` presents the available telemetry information for each subsystem, keep in mind that this only illustrates the timestamp and data fields of the complete downlink packet, there are still IDs, callsigns, etc. To see a complete representation of the packets look at the :numref:`tab:downlink-packets`.

.. _sec:telecommands:

Telecommands
------------

The :numref:`tab:packets-struct` summarizes all types of telemetry and telecommand packets received by TTC device that OBDH can handle, with the ID number, structure, length, and access type of each packet.

.. container:: landscape

   .. container::

      .. table:: Telecommunication packets and their content.
         :name: tab:packets-struct
         :class: packet-summary

         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         | **Link** | **Packet Name**     | **ID**      | **Source Callsign** | **Data (up to 212 bytes)**                        | **Size (bytes)** | **Access** |
         +==========+=====================+=============+=====================+===================================================+==================+============+
         | Downlink | EPS data            | 00h         | “ ” + “PY0EFS”      | EPS data                                          | 46               | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         | (VHF)    | Message broadcast   | 01h         |                     | Requester + dst. callsign + message               | 22 to 60         | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Ping answer         | 02h         |                     | Requester callsign                                | 15               | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | General telemetry   | 10h         | “ ” + “PY0EFS”      | OBDH/EPS data                                     | 78               | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Data request answer | 11h         |                     | Requester callsign + data ID + ts. + data         | 20 to 220        | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         | Downlink | Payload data        | 12h         |                     | Payload ID + payload data                         | 9 to 220         | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         | (UHF)    | TC feedback         | 13h         |                     | Req. callsign + TC packet ID + timestamp          | 20               | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Parameter value     | 14h         |                     | Req. callsign + Sub. ID + Param. ID + Param. Val. | 21               | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Packet broadcast    | 15h         |                     | Data of “Transmit packet” TC                      | 8 to 60          | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         | Uplink   | Ping request        | 40h         | Any Callsign        | None                                              | 8                | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Data request        | 41h         |                     | Data ID + Start ts. + End ts. + Hash              | 37               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Broadcast Message   | 42h         |                     | Dst. callsign + message                           | 15 to 53         | Public     |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Enter hibernation   | 43h         |                     | Hibernation in hours + Hash                       | 30               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Leave hibernation   | 44h         |                     | Hash                                              | 28               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Activate module     | 45h         |                     | Module ID + Hash                                  | 29               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Deactivate module   | 46h         |                     | Module ID + Hash                                  | 29               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Activate payload    | 47h         |                     | Payload ID + Hash                                 | 29               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Deactivate payload  | 48h         |                     | Payload ID + Hash                                 | 29               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Erase memory        | 49h         |                     | Memory ID + Hash                                  | 29               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Force reset         | 4Ah         |                     | Hash                                              | 28               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Get payload data    | 4Bh         |                     | Payload ID + Args. + Hash                         | 41               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Set parameter       | 4Ch         |                     | Subsystem ID + Param. ID + Param. value + Hash    | 34               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Get parameter       | 4Dh         |                     | Subsystem ID + Parameter ID + Hash                | 30               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Transmit packet     | 4Eh         |                     | Req. callsign + Any sequence of bytes + Hash      | 29 to 73         | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+
         |          | Update TLE          | 4Fh         |                     | Binary TLE line + Hash                            | 78               | Private    |
         +----------+---------------------+-------------+---------------------+---------------------------------------------------+------------------+------------+

The ID of the subsystems, modules, memories and payloads used in the packets are highlighted in :numref:`tab:system-ids`.

.. container:: compacttable70

   .. table:: IDs of the satellite.
      :name: tab:system-ids
      :widths: 22 25 53

      +---------------+---------------+----------------------+
      | **Type**      | **ID Number** | **Description**      |
      +===============+===============+======================+
      | Subsystem     | 0             | OBDH                 |
      +---------------+---------------+----------------------+
      |               | 1             | TTC 1                |
      +---------------+---------------+----------------------+
      |               | 2             | TTC 2                |
      +---------------+---------------+----------------------+
      |               | 3             | EPS                  |
      +---------------+---------------+----------------------+
      | Module        | 1             | Battery heater       |
      +---------------+---------------+----------------------+
      |               | 2             | Beacon               |
      +---------------+---------------+----------------------+
      |               | 3             | Periodic telemetry   |
      +---------------+---------------+----------------------+
      | Payload       | 1             | EDC 1                |
      +---------------+---------------+----------------------+
      |               | 2             | EDC 2                |
      +---------------+---------------+----------------------+
      |               | 3             | Payload X            |
      +---------------+---------------+----------------------+
      |               | 4             | Radiation instrument |
      +---------------+---------------+----------------------+
      | Data          | 0             | OBDH data            |
      +---------------+---------------+----------------------+
      |               | 1             | EPS data             |
      +---------------+---------------+----------------------+
      |               | 2             | TTC 0 data           |
      +---------------+---------------+----------------------+
      |               | 3             | TTC 1 data           |
      +---------------+---------------+----------------------+
      |               | 4             | Antenna data         |
      +---------------+---------------+----------------------+
      |               | 5             | SBCD packets         |
      +---------------+---------------+----------------------+
      |               | 6             | Payload Info         |
      +---------------+---------------+----------------------+
      | Memory        | 0             | NOR memory           |
      +---------------+---------------+----------------------+
      |               | 1             | FRAM memory          |
      +---------------+---------------+----------------------+

.. _sec:hmac:

Authentication
~~~~~~~~~~~~~~

All the telecommands classified as private use an HMAC authentication scheme. Every type of private telecommand has a unique 16-digit ASCII character key that with the telecommand sequence (or message) generates an 160-bits (20-bytes) hash sequence to be transmitted together with the packet payload. The used hash algorithm is the SHA-1. The :numref:`fig:hmac-diagram` illustrates this authentication method.

.. figure:: img/hmac-diagram.*
   :align: center
   :name: fig:hmac-diagram

   Diagram of the used HMAC scheme.

Telecommand Descriptions
~~~~~~~~~~~~~~~~~~~~~~~~

A detailed description of each telecommand, including packet structure, execution and feedback is highlighted on :ref:`anx:packets`.

Operating System
----------------

The FreeRTOS 10 :cite:`freertos` is being used as an operating system. FreeRTOS is a market-leading real-time operating system (RTOS) for microcontrollers and small microprocessors. Distributed freely under the MIT open-source license, FreeRTOS includes a kernel and a growing set of IoT libraries suitable for use across all industry sectors. FreeRTOS is built with an emphasis on reliability and ease of use.

The main configuration parameters of the operating system in this project are available in :numref:`tab:freertos-config`.

.. container::

   .. table:: FreeRTOS main configuration parameters.
      :name: tab:freertos-config
      :widths: 55 25 20

      ======================== ========= ========
      **Parameter**            **Value** **Unit**
      ======================== ========= ========
      Version                  v10.2.0   \-
      Tick rate (Hz)           1000      Hz
      CPU clock (HZ)           32        MHz
      Max. priorities          5         \-
      Heap size                40960     bytes
      Max. length of task name 20        \-
      ======================== ========= ========

More details of the used configuration parameters can be seen in the file `firmware/config/FreeRTOSConfig.h <https://github.com/spacelab-ufsc/obdh2/blob/master/firmware/config/FreeRTOSConfig.h>`__ from :cite:`obdh2-repo`.

Hardware Abstraction Layer (HAL)
--------------------------------

As the Hardware Abstraction Layer (HAL), the DriverLib :cite:`driverlib` from Texas Instruments is begin used. It is the official API to access the registers of the MSP430 microcontrollers.

The DriverLib is meant to provide a “software” layer to the programmer to facilitate a higher programming level than direct register accesses. By using the high-level software APIs provided by DriverLib, users can create powerful and intuitive code that is highly portable between devices within the MSP430 platform and different families in the MSP430/MSP432 platforms.
