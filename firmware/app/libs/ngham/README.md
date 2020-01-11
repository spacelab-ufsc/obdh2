NGHam protocol
==============

NGHam, short for Next Generation Ham Radio, is a set of protocols for packet radio communication. Its usage is similar to the existing AX.25 protocol. The standard covers three sub-protocols:
- NGHam radio protocol
- NGHam serial port protocol (comparable with KISS for AX.25)
- NGHam extensions - a modular extension using the payload for addressing, position, voice, statistics etc.

Main features:

- **Very robust:** Allows decoding with lower SNR compared to 1200 baud AFSK AX.25 due to the use of a sync word with good autocorrelation properties and Reed Solomon FEC over the whole remaining packet.
- **High throughput:** As the sync word eliminates the need for a long preamble and FEC increases chance of successful packet reception, the throughput will be higher than for 9600 baud AX.25.
- **Better spectral efficiency:** Significantly better spectral efficiency than 1200 baud AFSK, and somewhat better than 9600 baud G3RUH modulation due to the reduced deviation.
- **More efficient channel access:** CSMA is the default access method, but TDMA with automatic time-synchronization is being developed and is tested.
- **Clear distinction between communication layers:** The NGHam radio protocol has no addressing or other higher level information, and can be used without NGHam Extension.

[Youtube video: NGHam demonstration with Owl VHF and TDMA](http://youtu.be/_96td-Y-LLA)


![NGHam radio protocol (NGHam RF) block schematic](https://github.com/skagmo/ngham/blob/master/documentation/img/ngham_block_v4.png?raw=true)

![NGHam protocol stack explanation](https://github.com/skagmo/ngham/blob/master/documentation/img/ngham_protocol_stack.png?raw=true)

Content:

| File/folder       | Description |
|:-----------       |:----------- |
| documentation/    | More documentation. |
| fec-3.0.1/        | FEC library by Phil Karn for Reed Solomon. (http://www.ka9q.net/code/fec/) |
| platform/         | User provided - MUST BE CUSTOMIZED. |
| ccsds_scrambler.h | Pre-generated array from scrambling polynomial. |
| crc_ccitt.h       | CRC-code used in NGHam. |
| ngham.h           | The main NGHam encoder/decoder functions. |
| ngham_packets.h   | Declarations of the TX/RX packet structs etc. |
| ngham_paths.h     | User provided - MUST BE CUSTOMIZED. |
| ngham_spp.h       | Serial port protocol for communication with the host.  |


Not finished:

| File/folder       | Description |
|:-----------       |:----------- |
| ngham_extension.h        | An extension of the payload field in the NGHam radio protocol (enabled by a flag in the NGHam header).  |

Usage (for NGHam RF protocol):

1. Provide your own sync word correlator and GMSK (de)modulator. 
2. The demodulator should start outputting data only after sync word is detected.
3. Build all files in the root except "ngham_spp.c". From the folder "fec-3.0.1", build "decode_rs_char.c", "encode_rs_char.c" and "init_rs_char.c".
3. "ngham_paths.h" and the code in "platform/" must be customized.

```
// Initialize Reed Solomon arrays - only do this once!
ngham_init_arrays();

// Initialize state machine
ngham_init();

// Send a packet containing the payload "TEST"
tx_pkt_t p;
p.pl[0] = 'T';
p.pl[1] = 'E';
p.pl[2] = 'S';
p.pl[3] = 'T';
p.pl_len = 4;
p.ngham_flags = 0;
ngham_encode(&p);

// Receive packet. If a complete packet is received, function ngham_action_handle_packet will be called.
while (demodulator_data_available()) ngham_decode(get_byte_from_demodulator());
```

--
Jon Petter Skagmo, LA3JPA, 2014
