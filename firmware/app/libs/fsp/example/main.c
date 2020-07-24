#include <stdio.h>
#include <fsp/fsp.h>

int main(int argc, char **argv)
{
    //-- TRANSMITTER ---------------------------------------------------------------
    
    fsp_init(FSP_ADR_OBDH);                                                                 // OBDH module as host
    
    fsp_packet_t fsp;
    
    uint8_t obdh_data[] = "Test";                                                           // Data packet with "Test" as payload
    uint8_t obdh_data_len = sizeof(obdh_data)-1;
    
    fsp_gen_data_pkt(obdh_data, obdh_data_len, FSP_ADR_TTC, FSP_PKT_WITHOUT_ACK, &fsp);     // Packet to TTC module without ACK
    
    uint8_t pkt[FSP_PKT_MAX_LENGTH];
    uint8_t pkt_len;
    
    fsp_encode(&fsp, pkt, &pkt_len);                                                        // Encoding the packet (From FPSPacket struct to an array of bytes)
    
    printf("Encoded packet:\n");
    uint8_t i = 0;
    for(i=0; i<pkt_len; i++)
    {
        printf("%u, ", pkt[i]);
    }
    
    printf("\n\n");
    
    //-- RECEIVER ------------------------------------------------------------------
    
    fsp_init(FSP_ADR_TTC);                                                                  // TTC module as host
    
    fsp_packet_t fsp_d;
    
    i = 0;
    while(i < 255)                                                                          // "Timeout"
    {
        switch(fsp_decode(pkt[i++], &fsp_d))
        {
            case FSP_PKT_NOT_READY:
                break;
            case FSP_PKT_READY:
                printf("Decoded packet (Payload):\n");
                for(i=0; i<fsp_d.length; i++)
                {
                    printf("%c", fsp_d.payload[i]);
                }
                
                printf("\n\n");
                
                return 0;
                
                break;
            case FSP_PKT_INVALID:
                printf("Invalid packet!\n\n");
                break;
            case FSP_PKT_WRONG_ADR:
                printf("This packet is not for this host!\n\n");
                break;
            case FSP_PKT_ERROR:
                printf("Error decoding the packet!\n\n");
                break;
            default:
                printf("Unknown error!\n\n");
                break;
        }
    }
    
    printf("\n\n");
    
    return 0;
}
