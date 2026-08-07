.. packets.rst

   Copyright The OBDH 2.0 Contributors.

   OBDH 2.0 Documentation

   This work is licensed under the Creative Commons Attribution-ShareAlike 4.0
   International License. To view a copy of this license,
   visit http://creativecommons.org/licenses/by-sa/4.0/.

.. _anx:packets:

Telecommunication Description
=============================

This appendix describes the telecommands supported by OBDH, including the packet format, responses, etc.

Downlink
--------

In :numref:`tab:downlink-packets` the content of the downlink packets are available. These can be downloaded through telecommands, see :ref:`sec:uplink-descr` for more information on how to do so. The format for the “data” field of *Get Payload Data* and *Data Request* are shown in :ref:`anx:telemetry-apx`.

.. container::

   .. table:: Downlink packets.
      :name: tab:downlink-packets
      :widths: 20 12 53 15

      +---------------------+--------------+------------------------------------------------------+--------------------+
      | **Packet**          | **Position** | **Content**                                          | **Length [bytes]** |
      +=====================+==============+======================================================+====================+
      | Message broadcast   | 0            | Packet ID (01h)                                      | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 1            | Source callsign (“ PY0EFS”)                          | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 8            | Requester callsign                                   | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 15           | Destination callsign                                 | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 22           | Message                                              | up to 38           |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     |              |                                                      | up to 60           |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      | Ping answer         | 0            | Packet ID (02h)                                      | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 1            | Source callsign (“ PY0EFS”)                          | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 8            | Requester callsign                                   | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     |              |                                                      | 15                 |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      | Data request answer | 0            | Packet ID (11h)                                      | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 1            | Source callsign (“ PY0EFS”)                          | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 8            | Requester callsign                                   | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 15           | Data type ID                                         | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 16           | Timestamp                                            | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 20           | Data                                                 | Var.               |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     |              |                                                      | 20 (min.)          |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      | Payload Data        | 0            | Packet ID (12h)                                      | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 1            | Source callsign (“ PY0EFS”)                          | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 8            | Requester callsign                                   | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 15           | Data type ID                                         | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 16           | Timestamp                                            | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 20           | Data                                                 | Var.               |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     |              |                                                      | 20 (min.)          |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      | TC feedback         | 0            | Packet ID (13h)                                      | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 1            | Source callsign (“ PY0EFS”)                          | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 8            | Requester callsign                                   | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 15           | TC packet ID                                         | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 16           | Timestamp                                            | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     |              |                                                      | 20                 |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      | Parameter value     | 0            | Packet ID (14h)                                      | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 1            | Source callsign (“ PY0EFS”)                          | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 8            | Requester callsign                                   | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 15           | Subsystem ID                                         | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 16           | Parameter ID                                         | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 17           | Parameter value                                      | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     |              |                                                      | 21                 |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      | Packet Broadcast    | 0            | Packet ID (15h)                                      | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 1            | Source callsign (“ PY0EFS”)                          | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 8            | Requester callsign                                   | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 15           | Message                                              | up to 45           |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     |              |                                                      | up to 60           |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 0            | Packet ID (10h)                                      | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 1            | Source callsign (“ PY0EFS”)                          | 7                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 8            | System time in seconds (Unix epoch)                  | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 12           | Temperature of the OBDH :math:`\mu`\ C in Kelvin     | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 14           | Input current of the OBDH in mA                      | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 16           | Input voltage of the OBDH in mV                      | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 18           | Last reset cause of the OBDH                         | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 19           | Reset counter of the OBDH                            | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 21           | Last valid telecommand (uplink packet ID)            | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 22           | Temperature of the radio 1 in Kelvin                 | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 24           | RSSI of the last valid TC from radio 1               | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 26           | Temperature of the antenna in Kelvin                 | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 28           | Antenna status                                       | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 30           | Temperature of the EPS :math:`\mu`\ C in K           | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 32           | EPS circuitry and Beacon MCU current in mA           | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 34           | Last reset cause of the EPS                          | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 35           | Reset counter (EPS)                                  | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 37           | -Y and +X sides solar panel voltage in mV            | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 39           | -X and +Z sides solar panel voltage in mV            | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 41           | -Z and +Y sides solar panel voltage in mV            | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 43           | -Y side solar panel current in mA                    | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 45           | +Y side solar panel current in mA                    | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 47           | -X side solar panel current in mA                    | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 49           | +X side solar panel current in mA                    | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 51           | -Z side solar panel current in mA                    | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 53           | +Z side solar panel current in mA                    | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 55           | MPPT 1 duty cycle in %                               | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 56           | MPPT 2 duty cycle in %                               | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 57           | MPPT 3 duty cycle in %                               | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 58           | Main power bus voltage in mV                         | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 60           | Batteries voltage in mV                              | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 62           | Batteries current in mA                              | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 64           | Batteries average current in mA                      | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 66           | Batteries accumulated current in mA                  | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 68           | Battery monitor IC temperature in K                  | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 70           | Battery heater 1 duty cycle in %                     | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 71           | Battery heater 2 duty cycle in %                     | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 72           | Main EDC (01h = EDC_1, 02h = EDC_2)                  | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 73           | Main payload state (Payload ID or 0)                 | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 74           | Secondary payload state (Payload ID or 0)            | 1                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 75           | Number of used Data Log flash pages                  | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 79           | Number of used flash pages in SBCD packets sector    | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 83           | Timestamp of the last OBDH sensor’s reading in sec.  | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 87           | Timestamp of the last position determination in sec. | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 91           | Satellite’s latitude in degrees                      | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 93           | Satellite’s longitude in degrees                     | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 95           | Satellite’s altitude in km                           | 2                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 97           | Timestamp of the last TLE line update in sec.        | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     | 101          | Timestamp of the last TC reception in sec.           | 4                  |
      +---------------------+--------------+------------------------------------------------------+--------------------+
      |                     |              |                                                      | 105                |
      +---------------------+--------------+------------------------------------------------------+--------------------+

.. _sec:uplink-descr:

Uplink
------

As shown in :numref:`tab:packets-struct`, there are 15 supported telecommands. Below there is a description of each one.

- **Ping Request:** It is a simple command to test the communication with the satellite. When the satellite receives a ping packet, it will respond with another ping packet (with another packet ID, as defined in the downlink packets list). There are no additional parameters in the ping packet, just the packet ID and the source callsign (or address). It is also a public telecommand, anyone can send a ping request telecommand to a satellite.

- **Data Request:** It is a command to download data from the satellite. This command allows a ground station to get specific parameters from a given period (stored in the non-volatile memory of the onboard computer of the satellite). The list of possible data packets varies according to the satellite, see :numref:`tab:system-ids` for already implemented data IDs. The required fields of this telecommand are the data ID (1 byte), the start memory page and the end memory page. The memory pages are relative to the first page of module’s sector, meaning that if there is 15 pages of data from EPS and the operator wants to download all available pages, the start page should be 0 and end page should be 14. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Broadcast Message:** The "broadcast message" is another public telecommand, no authentication or key is required to send this telecommand to a satellite. This command has the purpose of making a satellite transmit a custom message back to Earth. This can be useful for communication tasks, like a station sending data to another. There are two parameters in this telecommand: the destination callsign (or address), and the content of the message, which can be any sequence of ASCII characters or any byte value. There is a limit of 38 characters in the message field.

- **Enter Hibernation:** This telecommand activates the hibernation mode in a satellite. During the hibernation mode, no transmissions are made by the satellite; it keeps just listening for new incoming packets (reception). The satellite will stay in hibernation mode for a custom period (1 to 65536 hours), or until a "Leave Hibernation" mode is received. Beyond the packet ID and the source callsign (or address), the number of hours (2 bytes long) is also transmitted. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Leave Hibernation:** This telecommand complements the "enter hibernation" telecommand by deactivating the hibernation mode in the satellite. When a satellite receives this telecommand, it enables the transmission again immediately. This is also a private telecommand; a specific key is required to send it. There is no additional content to this telecommand packet, just the packet ID and the source callsign (or address). This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Activate Module:** It activates an internal module of the satellite. Each module has a unique ID that is passed as an argument of this telecommand’s packet, see :numref:`tab:system-ids`. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Deactivate Module:** It disables an internal module of the satellite. Each module has a unique ID that is passed as an argument of this telecommand’s packet, see :numref:`tab:system-ids`. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Activate Payload:** This one is similar to the telecommand "Activate Module", but in this case is used for activating payloads of the satellite, see :numref:`tab:system-ids` for already implemented payloads. Each satellite should have a list of IDs of the set of payloads. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Deactivate Payload:** It is the same as the "Deactivate Module" telecommand, but for payloads. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Erase Memory:** It erases all the content presented in the specified non-volatile memory of the onboard computer of a satellite. No additional content is required in a erase memory telecommand packet, just the packet ID, the source callsign (or address) and the memory ID, see :numref:`tab:system-ids`. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Force Reset:** It performs a general reset of the satellite modules. When received, the OBDH resets other subsystem, then reset itself. There is no additional content in this packet, just the packet ID and the source callsign (or address). This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Get Payload Data:** It allows a ground station to download data from a specific payload of the satellite. The required fields are the payload ID, and optionally, arguments to be passed to the payload. The IDs and arguments vary according to the satellite. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Set Parameter:** It allows the configuration of specific parameters of a given subsystem of the satellite. The required fields are the ID of the subsystem to set (1 byte), the ID of the parameter to set (1 byte), and the new value of the parameter (4 bytes long). The possible IDs (subsystem and parameter) vary according to the satellite. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Get Parameter:** This telecommand complements the "Set Parameter" telecommand. It has the purpose of reading specific parameters of a given subsystem. The required fields are the subsystem’s ID (1 byte) and the parameter ID (1 byte). The possible IDs (subsystem and parameter) vary according to the satellite. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Transmit Packet:** This command has the purpose of making a satellite transmit a custom message back to Earth. This can be useful for communication tasks, like a station sending data to another. It differs from "Broadcast Message" in frequency band and the removal of destination callsign. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

- **Update TLE:** This telecommand provides a way to update the satellite TLE lines, which are used for position determination, more about the format on the `libpredict implementation <https://github.com/c-porto/libpredict/blob/master/include/predict/predict.h>`__ used for position determination. This is a private telecommand and requires a key for authentication, see :ref:`sec:hmac` for more information about authentication.

The :numref:`tab:uplink-packets` presents the content of the uplink packets.

.. container::

   .. table:: Uplink packets.
      :name: tab:uplink-packets
      :widths: 20 12 53 15

      +--------------------+--------------+-------------------------+--------------------+
      | **Packet**         | **Position** | **Content**             | **Length [bytes]** |
      +====================+==============+=========================+====================+
      | Ping request       | 0            | Packet ID (40h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 8                  |
      +--------------------+--------------+-------------------------+--------------------+
      | Data request       | 0            | Packet ID (41h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Data type ID            | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | Start memory page       | 4                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 13           | End memory page         | 4                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 17           | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 37                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Broadcast message  | 0            | Packet ID (42h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Destination callsign    | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 15           | Message                 | up to 38           |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | up to 53           |
      +--------------------+--------------+-------------------------+--------------------+
      | Enter hibernation  | 0            | Packet ID (43h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Hibernation in hours    | 2                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 10           | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 30                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Leave hibernation  | 0            | Packet ID (44h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 28                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Activate module    | 0            | Packet ID (45h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Module ID               | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 29                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Deactivate module  | 0            | Packet ID (46h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Module ID               | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 29                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Activate payload   | 0            | Packet ID (47h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Payload ID              | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 29                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Deactivate payload | 0            | Packet ID (48h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Payload ID              | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 29                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Erase memory       | 0            | Packet ID (49h)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Memory ID               | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 29                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Force reset        | 0            | Packet ID (4Ah)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 28                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Get payload data   | 0            | Packet ID (4Bh)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Payload ID              | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | Payload arguments       | 12                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 21           | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 41                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Set parameter      | 0            | Packet ID (4Ch)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Subsystem ID            | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | Parameter ID            | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 10           | Parameter value         | 4                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 14           | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 34                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Get parameter      | 0            | Packet ID (4Dh)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Subsystem ID            | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | Parameter ID            | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 10           | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 30                 |
      +--------------------+--------------+-------------------------+--------------------+
      | Transmit Packet    | 0            | Packet ID (4Eh)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 9            | Message                 | up to 45           |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | Var.         | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | up to 73           |
      +--------------------+--------------+-------------------------+--------------------+
      | Update TLE         | 0            | Packet ID (4Fh)         | 1                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 1            | Ground station callsign | 7                  |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 8            | Binary TLE line         | 50                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    | 58           | HMAC hash               | 20                 |
      +--------------------+--------------+-------------------------+--------------------+
      |                    |              |                         | 78                 |
      +--------------------+--------------+-------------------------+--------------------+
